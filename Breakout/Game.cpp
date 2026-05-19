#include "Game.h"

Game::Game()
	:m_window(sf::VideoMode({WinConfig::WINDOW_WIDTH, WinConfig::WINDOW_HEIGHT}), WinConfig::WINDOW_TITLE),
	m_paddle(),
	m_ball(),
	m_bricks()
{
	m_window.setFramerateLimit(60);
}

void Game::run()
{
	while (m_window.isOpen())
	{
		sf::Time dt = m_clock.restart();
		processEvents();
		update(dt);
		render();
	}
}

void Game::processEvents()
{
	while (const std::optional event = m_window.pollEvent())
	{
		// Close window event
		if (event->is<sf::Event::Closed>())
			m_window.close();
	}
}

void Game::update(sf::Time deltaTime)
{
	m_paddle.update(deltaTime);
	m_ball.update(deltaTime, m_window.getSize());

	if (m_ball.getGlobalBounds().findIntersection(m_paddle.getGlobalBounds()))
		m_ball.handlePaddleCollision(m_paddle);

	m_ball.handleBrickCollision(m_bricks);

	if (m_ball.isOutOfBounds())
	{
		m_ball.reset();
		lives--;
		std::cout << "Lives: " << lives << std::endl;
	}

	if (lives <= 0)
	{
		std::cout << "Game Over!" << std::endl;
		resetGame();
	}

	if (m_bricks.allDestroyed())
	{
		std::cout << "You Win!" << std::endl;
		resetGame();
	}
}

void Game::render()
{
	m_window.clear();
	m_bricks.draw(m_window);
	m_paddle.render(m_window);
	m_ball.render(m_window);
	m_window.display();
}

void Game::resetGame()
{
	lives = 3;
	m_paddle.reset();
	m_ball.reset();
	m_bricks = Brick();
}