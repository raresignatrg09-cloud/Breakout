#include "Brick.h"

Brick::Brick()
{
	rows = 10;
	cols = 10;

	float offsetX = (WinConfig::WINDOW_WIDTH - (cols * BrickConfig::BRICK_WIDTH)) / 2.f;
	float offsetY = 50.f;

	brickGrid.resize(rows, std::vector<BrickTile>(cols));
	
	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			brickGrid[row][col].position = sf::Vector2f(
				offsetX + col * BrickConfig::BRICK_WIDTH,
				offsetY + row * BrickConfig::BRICK_HEIGHT
			);
		}
	}
}

void Brick::draw(sf::RenderWindow & window)
{
	sf::RectangleShape brickShape;

	for (int row = 0; row < rows; ++row)
	{
		for(int col = 0; col < cols; ++col)
		{
			const BrickTile& tile = brickGrid[row][col];
			
			if (tile.active)
			{
				brickShape.setPosition(tile.position);
				brickShape.setSize(sf::Vector2f(BrickConfig::BRICK_WIDTH - 1.f, BrickConfig::BRICK_HEIGHT - 1.f));
				brickShape.setFillColor(sf::Color::Red);
				window.draw(brickShape);
			}
		}
	}
}

sf::FloatRect Brick::getBrickBounds(int row, int col) const
{
	const BrickTile& tile = brickGrid[row][col];

	return sf::FloatRect(
		tile.position,
		sf::Vector2f(BrickConfig::BRICK_WIDTH, BrickConfig::BRICK_HEIGHT)
	);
}