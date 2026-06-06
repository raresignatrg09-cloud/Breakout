#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include "Config.hpp"

enum class PaddleState
{
	Normal,
	Shrunk,
	Expanded
};

class Paddle
{
public:
	Paddle();
	void update(sf::Time deltaTime);
	void render(sf::RenderWindow& window);
	sf::FloatRect getGlobalBounds() const { return m_shape.getGlobalBounds(); }
	sf::Vector2f getPosition() const { return m_shape.getPosition(); }
	void reset();

	void shrink();
	void expand();
	void normal();

	void setPowerUpDuration(sf::Time duration) { m_powerUpTime = duration; }

private:
	void move(sf::Time deltaTime);
	void outOfBoundsCheck();
	void setPaddleAppearance(float width, const std::string& textureName);
	void loadTextures(const std::string& name, const std::string& path);

private:
	sf::RectangleShape m_shape;
	float m_speed;
	sf::Vector2f m_velocity;
	std::unordered_map<std::string, sf::Texture> m_textures;
	std::unique_ptr<sf::Sprite> m_sprite;

	PaddleState m_size{ PaddleState::Normal };
	sf::Time m_powerUpTime{ sf::Time::Zero };
};