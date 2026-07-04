#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>

#include "Config.hpp"
#include "PowerUps.h"

struct BrickTile
{
	bool active = true;

	std::string textureName = "green";
	powerUpType powerUp = powerUpType::None;
};

class Brick
{
public:
	Brick(sf::Vector2u windowSize);

	void draw(sf::RenderWindow& window);

	const std::vector<std::vector<BrickTile>>& getBricks() const { return brickGrid; }
	std::vector<std::vector<BrickTile>>& getBricks() { return brickGrid; }

	sf::FloatRect getBounds(int row, int col) const;

	void deactivateBrick(int row, int col);

	bool isCleared() const;

	void reset();

private:
	void loadTextures(const std::string& name, const std::string& path);
	void initializeGrid();

	float getOffsetX() const;
	float getOffsetY() const { return 50.f; }
	float getTotalWidth() const;

private:
	std::vector<std::vector<BrickTile>> brickGrid;

	std::map<std::string, sf::Texture> m_textures;
	std::unique_ptr<sf::Sprite> m_sprite;

	sf::Vector2u m_windowSize;

	int rows;
	int cols;
};

