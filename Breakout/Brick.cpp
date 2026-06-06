#include "Brick.h"
#include <stdexcept>

Brick::Brick()
	: rows(12), cols(10)
{
	//load textures for different brick types
	loadTextures("green", "assets/sprites/block_green.png");
	loadTextures("pink", "assets/sprites/block_pink.png");
	loadTextures("blue", "assets/sprites/block_blue.png");
	loadTextures("brown", "assets/sprites/block_brown.png");

	//configure sprite (used for drawing bricks)
	m_sprite = std::make_unique<sf::Sprite>(m_textures["green"]);

	float offsetX = (WinConfig::WINDOW_WIDTH - (cols * BrickConfig::BRICK_WIDTH)) / 2.f;
	float offsetY = 50.f;

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
	float offsetX = 
		(WinConfig::WINDOW_WIDTH - (cols * BrickConfig::BRICK_WIDTH)) / 2.f;
	float offsetY = 50.f;

	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			const auto& tile = brickGrid[row][col];

			if (!tile.active)
				continue;

			m_sprite->setTexture(m_textures.at(tile.textureName));

			m_sprite->setPosition({
				offsetX + col * BrickConfig::BRICK_WIDTH,
				offsetY + row * BrickConfig::BRICK_HEIGHT
				});

			window.draw(*m_sprite);
		}
	}
}

sf::FloatRect Brick::getBounds(int row, int col) const
{
	if (row < 0 || row >= rows || col < 0 || col >= cols)
		return {};

	float offsetX = 
		(WinConfig::WINDOW_WIDTH - (cols * BrickConfig::BRICK_WIDTH)) / 2.f;
	float offsetY = 50.f;

	return {
		{
			offsetX + col * BrickConfig::BRICK_WIDTH,
			offsetY + row * BrickConfig::BRICK_HEIGHT
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

void Brick::loadTextures(const std::string& name, const std::string& path)
{
	sf::Texture texture;

	if(!texture.loadFromFile(path))
		throw std::runtime_error("Failed to load brick texture!");

	m_textures.emplace(name, std::move(texture));
}

void Brick::initializeGrid()
{
	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			brickGrid[row][col].active = true;

			if (row < 3)
				brickGrid[row][col].textureName = "brown";
			else if (row < 6)
				brickGrid[row][col].textureName = "blue";
			else if (row < 9)
				brickGrid[row][col].textureName = "pink";
			else
				brickGrid[row][col].textureName = "green";

			int powerUpChance = std::rand() % 100;

			if (powerUpChance < 15)
				brickGrid[row][col].powerUp = powerUpType::ExpandPaddle;
			else if (powerUpChance < 10)
				brickGrid[row][col].powerUp = powerUpType::ShrinkPaddle;
			else if (powerUpChance < 20)
				brickGrid[row][col].powerUp = powerUpType::ExtraLife;
			else if (powerUpChance < 50)
				brickGrid[row][col].powerUp = powerUpType::MultiBall;
			else if (powerUpChance < 15)
				brickGrid[row][col].powerUp = powerUpType::StickyPaddle;
			else
				brickGrid[row][col].powerUp = powerUpType::None;
		}
	}
}
