#pragma once
#include <SFML/Graphics.hpp>
#include "Config.cpp"
#include "Paddle.h"
#include "Brick.h"

class Ball
{
public:
	Ball();
	void update(sf::Time deltaTime, const sf::Vector2u& windowSize);
	void render(sf::RenderWindow& window);
	bool isOutOfBounds() const;
	void reset();
	sf::FloatRect getGlobalBounds() const { return m_shape.getGlobalBounds(); }
	void handlePaddleCollision(const Paddle& paddle);
	void handleBrickCollision(Brick& brick);
	void normalizeVelocity();
	void bounceX() { m_velocity.x *= -1.f; }
	void bounceY() { m_velocity.y *= -1.f; }

	int getScore() const { return score; }
	void addScore(int points) { score += points; }

private:
	sf::CircleShape m_shape;
	sf::Vector2f m_velocity;
	float m_speed;

	int score = 0;
};

