#include "GameplayScene.h"
#include "PauseScene.h"
#include "Game.h"

GameplayScene::GameplayScene(Game& game)
	:Scene(game),
	m_paddle({ WinConfig::WINDOW_WIDTH, WinConfig::WINDOW_HEIGHT }),
	m_bricks({ WinConfig::WINDOW_WIDTH, WinConfig::WINDOW_HEIGHT })
{
    InitUI();
	m_game.getStatisticsManager().increaseGamesPlayed();

	m_balls.reserve(10);
	spawnBall();
}

void GameplayScene::handleEvent(const sf::Event & event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->code == sf::Keyboard::Key::W ||
            keyPressed->code == sf::Keyboard::Key::Space)
        {
            if (!m_gameOver && !m_balls.empty())
            {
                m_balls.front().setLaunched(true);
            }
        }

        if (keyPressed->code == sf::Keyboard::Key::R)
        {
            if (m_gameOver)
            {
                resetGame();
            }
        }

        if (keyPressed->code == sf::Keyboard::Key::Escape)
        {
            m_game.pushScene<PauseScene>();
        }
    }
}

void GameplayScene::update(sf::Time deltaTime)
{
    sf::Time frameTime = deltaTime;
    m_accumulator += frameTime;

    while (m_accumulator.asSeconds() >= WinConfig::FIXED_DT && !m_gameOver)
    {
        sf::Time dt = sf::seconds(WinConfig::FIXED_DT);

        m_paddle.update(dt);
        updateBalls(dt);
        updatePowerUps(dt);
        removeDeadBalls();
        updateUI();

		m_game.getStatisticsManager().addPlayTime(dt);

        m_accumulator -= dt;
    }

    m_game.getSoundManager().update();

    checkWinOrLose();
}

void GameplayScene::render(sf::RenderWindow & window)
{
    m_bricks.draw(m_game.getWindow());
    m_paddle.render(m_game.getWindow());

    for (auto& ball : m_balls)
        ball.render(m_game.getWindow());

    for (auto& powerUp : m_powerUps)
        powerUp.render(m_game.getWindow());

    m_game.getTextManager().draw(m_game.getWindow());
}

void GameplayScene::updateBalls(sf::Time deltaTime)
{
    for (auto& ball : m_balls)
    {
        ball.update(deltaTime, m_game.getWindow().getSize());

        if (!ball.isLaunched())
            ball.followPaddle(m_paddle);

        if (ball.isLaunched() && ball.getGlobalBounds()
            .findIntersection(m_paddle.getGlobalBounds()))
        {
            ball.handlePaddleCollision(m_paddle);
            m_game.getSoundManager().play("paddle_hit");
        }

        auto result = ball.handleBrickCollision(m_bricks);

        if (result.collided)
        {
            m_game.getSoundManager().play("brick_hit");

            m_score += 100;
			m_game.getStatisticsManager().increaseBricksDestroyed();
			m_game.getStatisticsManager().updateHighestScore(m_score);

            if (result.powerUp != powerUpType::None)
                m_powerUps.emplace_back(
                    result.powerUp,
                    result.position
                );
        }
    }
}

void GameplayScene::updatePowerUps(sf::Time deltaTime)
{
    for (auto it = m_powerUps.begin(); it != m_powerUps.end();)
    {
        it->update(deltaTime);

        bool remove = false;

        if (it->getGlobalBounds()
            .findIntersection(m_paddle.getGlobalBounds())
            .has_value())
        {
			m_game.getStatisticsManager().addPowerUpCollected();

            switch (it->getType())
            {
            case powerUpType::ExpandPaddle:
                m_paddle.setPowerUpDuration(sf::seconds(PowerUpConfig::duration));
                m_paddle.expand();
                break;

            case powerUpType::ShrinkPaddle:
                m_paddle.setPowerUpDuration(sf::seconds(PowerUpConfig::duration));
                m_paddle.shrink();
                break;

            case powerUpType::ExtraLife:
                m_lives = std::min(m_lives + 1, 3);
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

        if (it->isOffScreen(m_game.getWindow().getSize()))
            remove = true;

        if (remove)
            it = m_powerUps.erase(it);
        else
            ++it;
    }
}

void GameplayScene::spawnBall()
{
    if (!m_balls.empty())
        return;

    m_balls.emplace_back(
        sf::Vector2f(
            m_game.getWindow().getSize().x / 2.f,
            m_paddle.getPosition().y - BallConfig::radius * 2
        ),
        sf::Vector2u(m_game.getWindow().getSize())
    );
}

void GameplayScene::removeDeadBalls()
{
    for (auto it = m_balls.begin(); it != m_balls.end();)
    {
        if (it->isOutOfBounds())
        {
			m_game.getStatisticsManager().increaseBallsLost();
            it = m_balls.erase(it);
        }
        else
            ++it;
    }

    if (m_balls.empty())
    {
        m_lives = std::max(0, m_lives - 1);

        if (m_lives > 0)
            spawnBall();
    }
}

void GameplayScene::createMultiBall()
{
    if (m_balls.empty())
        return;

    if (m_balls.size() >= 8)
        return;

    Ball newBall = m_balls.front();

    uint8_t r = static_cast<uint8_t>(std::rand() % 256);
    uint8_t g = static_cast<uint8_t>(std::rand() % 256);
    uint8_t b = static_cast<uint8_t>(std::rand() % 256);

    newBall.setColor({ r,b,g,255 });

    auto velocity = newBall.getVelocity();
    velocity.x *= -1.f;

    newBall.setVelocity(velocity);

    m_balls.push_back(newBall);
}

void GameplayScene::updateUI()
{
    m_game.getTextManager().setString(
        "score",
        "Score: " + std::to_string(m_score)
    );

    m_game.getTextManager().setString(
        "lives",
        "Lives: " + std::to_string(m_lives)
    );
}

void GameplayScene::checkWinOrLose()
{
    if (m_lives <= 0)
    {
        m_gameOver = true;

        m_game.getTextManager().setVisible("game_over", true);
        m_game.getTextManager().setVisible("restart", true);

		m_game.getStatisticsManager().increaseGamesLost();
    }
    else if (m_bricks.isCleared())
    {
        m_gameOver = true;

        m_game.getTextManager().setVisible("you_win", true);
        m_game.getTextManager().setVisible("restart", true);

        m_game.getStatisticsManager().increaseGamesWon();
    }
}

void GameplayScene::resetGame()
{
    m_gameOver = false;
    m_lives = 3;
    m_score = 0;

    m_game.getTextManager().setVisible("game_over", false);
    m_game.getTextManager().setVisible("you_win", false);
    m_game.getTextManager().setVisible("restart", false);

    m_paddle.reset();
    m_balls.clear();
    spawnBall();
    m_bricks.reset();
    m_powerUps.clear();

    m_accumulator = sf::Time::Zero;
}

void GameplayScene::InitUI()
{
    m_game.createText("score", "font", "Score: 0", 24, { 10.f, 10.f },
        sf::Color::White, sf::Text::Bold);

    m_game.createText("lives", "font", "Lives: 3", 24,
        { WinConfig::WINDOW_WIDTH - 150.f, 10.f },
        sf::Color::White, sf::Text::Bold);

    m_game.createText("game_over", "font", "Game Over!", 48,
        { m_game.getWindow().getSize().x / 2.f, m_game.getWindow().getSize().y / 2.f },
        sf::Color::Red, sf::Text::Bold);
    m_game.getTextManager().setAlignment("game_over", TextManager::Alignment::Center);
    m_game.getTextManager().setVisible("game_over", false);

    m_game.createText("you_win", "font", "You Win!", 48,
        { m_game.getWindow().getSize().x / 2.f, m_game.getWindow().getSize().y / 2.f },
        sf::Color::Yellow, sf::Text::Bold);
    m_game.getTextManager().setAlignment("you_win", TextManager::Alignment::Center);
    m_game.getTextManager().setVisible("you_win", false);

    m_game.createText("restart", "font", "Press R to restart", 24,
        { m_game.getWindow().getSize().x / 2.f, m_game.getWindow().getSize().y / 2.f + 70.f },
        sf::Color::White, sf::Text::Bold);
    m_game.getTextManager().setAlignment("restart", TextManager::Alignment::Center);
    m_game.getTextManager().setVisible("restart", false);
}
