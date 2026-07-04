#pragma once
#include <SFML/Graphics.hpp>

class Game;

class Scene
{
public:
	Scene(Game& game) : m_game(game) {}
	virtual ~Scene() = default;

	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void update(sf::Time deltaTime) = 0;
	virtual void render(sf::RenderWindow& window) = 0;

protected:
	Game& m_game;
};

