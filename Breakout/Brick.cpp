#include "Brick.h"
#include <stdexcept>
#include <iostream>

Brick::Brick(sf::Vector2u windowSize)
	: rows(8), cols(12), m_windowSize(windowSize)
{
	//load textures for different brick types
	loadTextures("green", "assets/sprites/block_green.png");
	loadTextures("pink", "assets/sprites/block_pink.png");
	loadTextures("blue", "assets/sprites/block_blue.png");
	loadTextures("brown", "assets/sprites/block_brown.png");

	//configure sprite (used for drawing bricks)
	m_sprite = std::make_unique<sf::Sprite>(m_textures["green"]);

	auto texSize = m_textures.at("green").getSize();

	m_sprite->setScale({
		BrickConfig::BRICK_WIDTH / static_cast<float>(texSize.x),
		BrickConfig::BRICK_HEIGHT / static_cast<float>(texSize.y)
		});

	//initialize brick grid
	brickGrid.resize(rows, std::vector<BrickTile>(cols));

	initializeGrid();
}

void Brick::draw(sf::RenderWindow& window)
{
	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			const auto& tile = brickGrid[row][col];

			if (!tile.active)
				continue;

			m_sprite->setTexture(m_textures.at(tile.textureName));

			m_sprite->setPosition({
				getOffsetX() + col * (BrickConfig::BRICK_WIDTH + BrickConfig::BRICK_SPACING),
				getOffsetY() + row * (BrickConfig::BRICK_HEIGHT + BrickConfig::BRICK_SPACING)
				});

			int activeCount = 0;

			
			window.draw(*m_sprite);
		}
	}
}

sf::FloatRect Brick::getBounds(int row, int col) const
{
	if (row < 0 || row >= rows || col < 0 || col >= cols)
		return {};

	return {
		{
			getOffsetX() + col * (BrickConfig::BRICK_WIDTH + BrickConfig::BRICK_SPACING),
			getOffsetY() + row * (BrickConfig::BRICK_HEIGHT + BrickConfig::BRICK_SPACING)
		},
		{
			BrickConfig::BRICK_WIDTH,
			BrickConfig::BRICK_HEIGHT
		}
	};
}

void Brick::deactivateBrick(int row, int col)
{
	if (row < 0 || row >= rows || col < 0 || col >= cols)
		throw std::out_of_range("Brick position out of range!");

	brickGrid[row][col].active = false;
}

bool Brick::isCleared() const
{
	for (const auto& row : brickGrid)
		for (const auto& tile : row)
			if (tile.active)
				return false;

	return true;
}

void Brick::reset()
{
	brickGrid.assign(rows, std::vector<BrickTile>(cols));

	initializeGrid();
}

float Brick::getOffsetX() const
{
	return (m_windowSize.x - getTotalWidth()) / 2.f;
}

float Brick::getTotalWidth() const
{
	return cols * BrickConfig::BRICK_WIDTH +
		(cols - 1) * BrickConfig::BRICK_SPACING;
}

void Brick::loadTextures(const std::string& name, const std::string& path)
{
	sf::Texture texture;

	if(!texture.loadFromFile(path))
		throw std::runtime_error("Failed to load brick texture!");

	m_textures[name] = std::move(texture);
}

void Brick::initializeGrid()
{
	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			brickGrid[row][col].active = true;

			if (row < 2)
				brickGrid[row][col].textureName = "brown";
			else if (row < 4)
				brickGrid[row][col].textureName = "blue";
			else if (row < 6)
				brickGrid[row][col].textureName = "pink";
			else
				brickGrid[row][col].textureName = "green";

			int powerUpChance = std::rand() % 100;

			if (powerUpChance < PowerUpConfig::expandChance)
				brickGrid[row][col].powerUp = powerUpType::ExpandPaddle;
			else if (powerUpChance < PowerUpConfig::shrinkChance)
				brickGrid[row][col].powerUp = powerUpType::ShrinkPaddle;
			else if (powerUpChance < PowerUpConfig::extraLifeChance)
				brickGrid[row][col].powerUp = powerUpType::ExtraLife;
			else if (powerUpChance < PowerUpConfig::multiBallChance)
				brickGrid[row][col].powerUp = powerUpType::MultiBall;
			//else if (powerUpChance < PowerUpConfig::stickyPaddleChance)
				//brickGrid[row][col].powerUp = powerUpType::StickyPaddle;
			else
				brickGrid[row][col].powerUp = powerUpType::None;
		}
	}
}