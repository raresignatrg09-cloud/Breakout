#pragma once
#include <SFML/Graphics.hpp>
#include "Config.cpp"
#include <vector>

enum class powerUpType
{
	None,
	ExpandPaddle,
	ShrinkPaddle,
	ExtraLife,
	MultiBall,
	StickyPaddle
};

struct BrickTile
{
	sf::Vector2f position;
	powerUpType powerUp = powerUpType::None;
	bool active = true;
};

class Brick
{
public:
	Brick();
	void draw(sf::RenderWindow& window);
	const std::vector<std::vector<BrickTile>>& getBricks() const { return brickGrid; }
	std::vector<std::vector<BrickTile>>& getBricks() { return brickGrid; }
	sf::FloatRect getBrickBounds(int row, int col) const;
	void deActivateBrick(int row, int col) { brickGrid[row][col].active = false; }
	bool allDestroyed() const
	{
		for (const auto& row : brickGrid)
			for (const auto& tile : row)
				if (tile.active)
					return false;
		return true;
	}
private:
	std::vector<std::vector<BrickTile>> brickGrid;

	int rows;
	int cols;
};

