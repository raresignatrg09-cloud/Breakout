#pragma once
#include <SFML/Graphics.hpp>
#include "Config.cpp"

class Paddle
{
public:
	Paddle();
	void update(sf::Time deltaTime);
	void render(sf::RenderWindow& window);
	sf::FloatRect getGlobalBounds() const { return m_shape.getGlobalBounds(); }
	sf::Vector2f getPosition() const { return m_shape.getPosition(); }
	void reset();

private:
	void move(sf::Time deltaTime);
	void outOfBoundsCheck();
	void applyFriction(sf::Time deltaTime);

private:
	sf::RectangleShape m_shape;
	float m_speed;
	sf::Vector2f m_velocity;
};

