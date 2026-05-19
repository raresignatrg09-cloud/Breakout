#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"

#include "Config.cpp"

class Game
{
public:
	Game();
	void run();
private:
	void processEvents();
	void update(sf::Time deltaTime);
	void render();
	void resetGame();

private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	Paddle m_paddle;
	Ball m_ball;
	Brick m_bricks;

	float lives = 3;
};