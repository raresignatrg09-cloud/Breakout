#include "Game.h"

#include <algorithm>

Game::Game()
	:m_window(sf::VideoMode({WinConfig::WINDOW_WIDTH, WinConfig::WINDOW_HEIGHT}), WinConfig::WINDOW_TITLE),
	m_paddle(),
	m_bricks()
{
	m_window.setFramerateLimit(60);

	m_balls.reserve(10);
	spawnBall();

	m_textManager.loadFont("score","assets/font/font.ttf");
	m_textManager.loadFont("lives", "assets/font/font.ttf");

	m_textManager.createText("score", "score", "Score: 0", 24, { 10.f, 10.f });
	m_textManager.createText("lives", "lives", "Lives: 3", 24, { WinConfig::WINDOW_WIDTH - 150.f, 10.f });

	m_textManager.get("score").setFillColor(sf::Color::White);
	m_textManager.get("lives").setFillColor(sf::Color::White);

	m_textManager.get("score").setCharacterSize(24);
	m_textManager.get("lives").setCharacterSize(24);

	m_textManager.get("score").setStyle(sf::Text::Bold);
	m_textManager.get("lives").setStyle(sf::Text::Bold);
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

		// Key pressed event
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->code == sf::Keyboard::Key::Escape)
				m_window.close();

			if (keyPressed->code == sf::Keyboard::Key::W || keyPressed->code == sf::Keyboard::Key::Space)
				m_balls[0].setLaunched(true);
		}
	}
}

void Game::update(sf::Time deltaTime)
{
    m_paddle.update(deltaTime);
	updateBalls(deltaTime);
	updatePowerUps(deltaTime);
    removeDeadBalls();
	updateUI();
	checkWinOrLose();
}

void Game::updatePowerUps(sf::Time deltaTime)
{
    for (auto it = m_powerUps.begin(); it != m_powerUps.end();)
    {
        it->update(deltaTime);

        bool remove = false;

        if (it->getGlobalBounds()
            .findIntersection(m_paddle.getGlobalBounds())
            .has_value())
        {
            switch (it->getType())
            {
            case powerUpType::ExpandPaddle:
                m_paddle.setPowerUpDuration(sf::seconds(10.f));
                m_paddle.expand();
                break;

            case powerUpType::ShrinkPaddle:
                m_paddle.setPowerUpDuration(sf::seconds(10.f));
                m_paddle.shrink();
                break;

            case powerUpType::ExtraLife:
                m_lives = std::min(m_lives + 1, 3u);
                break;

            case powerUpType::MultiBall:
				createMultiBall();
                break;

            case powerUpType::StickyPaddle:
                // TODO
                break;

            default:
                break;
            }

            remove = true;
        }

        if (it->isOffScreen(m_window.getSize()))
            remove = true;

        if (remove)
            it = m_powerUps.erase(it);
        else
            ++it;
    }

}

void Game::updateBalls(sf::Time deltaTime)
{
    for (auto& ball : m_balls)
    {
        ball.update(deltaTime, m_window.getSize());

        if (!ball.isLaunched())
            ball.followPaddle(m_paddle);

        if (ball.getGlobalBounds()
            .findIntersection(m_paddle.getGlobalBounds()))
        {
            ball.handlePaddleCollision(m_paddle);
        }

        auto result = ball.handleBrickCollision(m_bricks);

        if (result.collided)
        {
            m_score += 100;

            if (result.powerUp != powerUpType::None)
                m_powerUps.emplace_back(
                    result.powerUp,
                    result.position
                );
        }
    }
}

void Game::createMultiBall()
{
    if (m_balls.size() < 8)
    {
        Ball newBall = m_balls.front();

        uint8_t r = static_cast<float>(std::rand() % 256);
        uint8_t g = static_cast<float>(std::rand() % 256);
        uint8_t b = static_cast<float>(std::rand() % 256);

        newBall.setColor({ r,b,g,255 });

        auto velocity = newBall.getVelocity();
        velocity.x *= -1.f;

        newBall.setVelocity(velocity);

        m_balls.push_back(newBall);
    }
}

void Game::spawnBall()
{
    m_balls.emplace_back(
        sf::Vector2f(
            WinConfig::WINDOW_WIDTH / 2.f,
            m_paddle.getPosition().y - BallConfig::radius * 2
        )
    );
}

void Game::checkWinOrLose()
{
    if (m_lives == 0)
    {
        std::cout << "Game Over!\n";
        resetGame();
    }

    if (m_bricks.isCleared())
    {
        std::cout << "You Win!\n";
        resetGame();
    }
}

void Game::removeDeadBalls()
{
    for (auto it = m_balls.begin(); it != m_balls.end();)
    {
        if (it->isOutOfBounds())
            it = m_balls.erase(it);
        else
            ++it;
    }

    if (m_balls.empty())
    {
        m_lives--;

        if (m_lives > 0)
            spawnBall();
    }
}

void Game::updateUI()
{
    m_textManager.setString(
        "score",
        "Score: " + std::to_string(m_score)
    );

    m_textManager.setString(
        "lives",
        "Lives: " + std::to_string(m_lives)
    );
}

void Game::render()
{
	m_window.clear();

	m_textManager.draw(m_window);
	m_bricks.draw(m_window);
	m_paddle.render(m_window);

    for(auto& ball : m_balls)
	    ball.render(m_window);

	for (auto& powerUp : m_powerUps)
		powerUp.render(m_window);

	m_window.display();
}

void Game::resetGame()
{
	m_lives = 3;
	m_paddle.reset();
	m_balls.clear();
	spawnBall();
	m_bricks.reset();
}