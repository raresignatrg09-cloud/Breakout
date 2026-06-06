#include "Ball.h"
#include <cmath>
#include <iostream>

Ball::Ball(sf::Vector2f position)
	:m_velocity(BallConfig::speed, -BallConfig::speed),
	m_initPosition(position)
{
	m_shape.setRadius(BallConfig::radius);

	m_shape.setFillColor(sf::Color::White);

	m_shape.setPosition(position);
}

void Ball::update(sf::Time deltaTime, const sf::Vector2u& windowSize)
{
	if(isLaunched())
		m_shape.move(m_velocity * deltaTime.asSeconds());
	
	sf::Vector2f position = m_shape.getPosition();
	float radius = m_shape.getRadius();

	if (position.x - radius < 0.f)
	{
		position.x = radius;
		m_velocity.x *= -1.f;
	}
	else if (position.x + radius > windowSize.x)
	{
		position.x = windowSize.x - radius;
		m_velocity.x *= -1.f;
	}

	if (position.y - radius < 0.f)
	{
		position.y = radius;
		m_velocity.y *= -1.f;
	}

	m_shape.setPosition(position);
}

void Ball::render(sf::RenderWindow & window)
{
	window.draw(m_shape);
}

bool Ball::isOutOfBounds() const
{
	return m_shape.getPosition().y - m_shape.getRadius()
	> WinConfig::WINDOW_HEIGHT;
}

void Ball::reset()
{
	m_shape.setPosition(m_initPosition);
	setLaunched(false);

	m_velocity = { BallConfig::speed,-BallConfig::speed };
}

void Ball::handlePaddleCollision(const Paddle& paddle)
{
	float paddleCenter =
		paddle.getPosition().x +
		PaddleConfig::PADDLE_WIDTH / 2.f;

	float ballCenter = m_shape.getPosition().x;

	float relativeIntersectX =
		(ballCenter - paddleCenter) /
		(PaddleConfig::PADDLE_WIDTH / 2.f);

	m_velocity.x = relativeIntersectX * m_speed;

	m_velocity.y = -std::abs(m_speed);

	normalizeVelocity();
}

BrickCollisionResult Ball::handleBrickCollision(Brick& brick)
{
	auto& bricks = brick.getBricks();

	sf::FloatRect ballBounds = getGlobalBounds();
	BrickCollisionResult result;

	for (int row = 0; row < static_cast<int>(bricks.size()); ++row)
	{
		for (int col = 0; col < static_cast<int>(bricks[row].size()); ++col)
		{
			BrickTile& tile = bricks[row][col];

			if (!tile.active)
				continue;

			sf::FloatRect brickBounds = brick.getBounds(row, col);

			auto intersection = ballBounds.findIntersection(brickBounds);

			if (intersection.has_value())
			{
				BrickCollisionResult result;

				result.collided = true;
				result.powerUp = tile.powerUp;
				result.position = brickBounds.getCenter();

				brick.deactivateBrick(row, col);

				// Collision response
				sf::Vector2f ballCenter = ballBounds.getCenter();
				sf::Vector2f brickCenter = brickBounds.getCenter();

				float dx = ballCenter.x - brickCenter.x;
				float dy = ballCenter.y - brickCenter.y;

				if (std::abs(dx) > std::abs(dy))
				{
					bounceX();

					if (dx > 0)
						m_shape.move({ 1.f, 0.f });
					else
						m_shape.move({ -1.f, 0.f });
				}
				else
				{
					bounceY();

					if (dy > 0)
						m_shape.move({ 0.f, 1.f });
					else
						m_shape.move({ 0.f, -1.f });
				}

				return result;
			}
		}
	}

	return {};
}

void Ball::normalizeVelocity()
{
	float length = std::sqrt(
		m_velocity.x * m_velocity.x +
		m_velocity.y * m_velocity.y
	);

	m_velocity /= length;
	m_velocity *= m_speed;
}

void Ball::followPaddle(const Paddle& paddle)
{
	sf::Vector2f paddlePos = paddle.getPosition();
	m_shape.setPosition({
		paddlePos.x + PaddleConfig::PADDLE_WIDTH / 2.f - m_shape.getRadius(),
		paddlePos.y - 2.f * m_shape.getRadius() - 1.f
		});
}