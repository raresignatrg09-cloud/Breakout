#pragma once
#include <SFML/Graphics.hpp>
#include "Config.hpp"

enum class powerUpType
{
	None,
	ExpandPaddle,
	ShrinkPaddle,
	ExtraLife,
	MultiBall,
	StickyPaddle
};

class PowerUps
{
public:
	PowerUps(powerUpType type, sf::Vector2f position);

	void update(sf::Time deltaTime);

	void render(sf::RenderWindow& window);

	bool isOffScreen(const sf::Vector2u& windowSize) const;

	[[nodiscard]] powerUpType getType() const { return m_type; }
	sf::FloatRect getGlobalBounds() const { return m_shape.getGlobalBounds(); }

private:
	sf::CircleShape m_shape;
	powerUpType m_type;
	float m_fallSpeed = 150.f;
};

