#include "Ball.h"
#include <cmath>
#include <iostream>

Ball::Ball()
	: m_speed(BallConfig::speed),
	m_velocity(BallConfig::speed, -BallConfig::speed)
{
	m_shape.setRadius(BallConfig::radius);

	m_shape.setOrigin({
		m_shape.getRadius(),
		m_shape.getRadius()
		});

	m_shape.setFillColor(sf::Color::White);

	m_shape.setPosition({
		WinConfig::WINDOW_WIDTH / 2.f,
		WinConfig::WINDOW_HEIGHT / 2.f
		});
}

void Ball::update(sf::Time deltaTime, const sf::Vector2u& windowSize)
{
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
	m_shape.setPosition({
		WinConfig::WINDOW_WIDTH / 2.f,
		WinConfig::WINDOW_HEIGHT / 2.f
		});

	m_velocity = {
		BallConfig::speed,
		-BallConfig::speed
	};
	score = 0;
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

void Ball::handleBrickCollision(Brick& brick)
{
	auto& bricks = brick.getBricks();

	for (int row = 0; row < bricks.size(); ++row)
	{
		for(int col=0;col<bricks[row].size();++col)
		{
			BrickTile& tile = bricks[row][col];
			if (tile.active)
			{
				sf::FloatRect brickBounds = brick.getBrickBounds(row, col);

				sf::FloatRect ballBounds = getGlobalBounds();

				if (ballBounds.findIntersection(brickBounds))
				{
					tile.active = false;
					addScore(1);
					std::cout << "Score: " << getScore() << std::endl;

					sf::Vector2f ballCenter = getGlobalBounds().getCenter();
					sf::Vector2f brickCenter = brickBounds.getCenter();

					float dx = ballCenter.x - brickCenter.x;
					float dy = ballCenter.y - brickCenter.y;

					if (std::abs(dx) > std::abs(dy))
					{
						bounceX();

						if (dx > 0)
							m_shape.move({1.f, 0.f});
						else
						
							m_shape.move({-1.f, 0.f});
					}
					else
					{
						bounceY();

						if (dy > 0)
							m_shape.move({0.f, 1.f});
						else
							m_shape.move({0.f, -1.f});
					}
					return;
				}
			}
		}
	}
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
