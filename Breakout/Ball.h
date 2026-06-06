#pragma once
#include <SFML/Graphics.hpp>

#include "Config.hpp"
#include "Paddle.h"
#include "Brick.h"

struct BrickCollisionResult
{
	bool collided = false;
	powerUpType powerUp = powerUpType::None;
	sf::Vector2f position;
};

class Ball
{
public:
	Ball(sf::Vector2f position);

	void update(sf::Time deltaTime, const sf::Vector2u& windowSize);
	void render(sf::RenderWindow& window);

	void reset();

	bool isLaunched() const { return m_isLaunched; }
	bool isOutOfBounds() const;

	sf::FloatRect getGlobalBounds() const { return m_shape.getGlobalBounds(); }
	
	void handlePaddleCollision(const Paddle& paddle);
	
	BrickCollisionResult handleBrickCollision(Brick& brick);
	void normalizeVelocity();
	
	void setLaunched(bool launched) { m_isLaunched = launched; }
	
	void followPaddle(const Paddle& paddle);

	sf::Vector2f getVelocity() const { return m_velocity; }
	void setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }

	void setColor(const sf::Color& color) { m_shape.setFillColor(color); }

private:
	void bounceX() { m_velocity.x *= -1.f; }
	void bounceY() { m_velocity.y *= -1.f; }

private:
	sf::CircleShape m_shape;

	sf::Vector2f m_velocity;
	sf::Vector2f m_initPosition;

	float m_speed = BallConfig::speed;
	bool  m_isLaunched = false;
};