#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>

#include "TextManager.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "PowerUps.h"
#include "Config.hpp"

class Game
{
public:
    Game();
    void run();

private:
    // Core loop
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    // Game systems
    void updateBalls(sf::Time deltaTime);
    void updatePowerUps(sf::Time deltaTime);

    // Ball management
    void spawnBall();
    void removeDeadBalls();
    void createMultiBall();

    // Game state
    void updateUI();
    void checkWinOrLose();
    void resetGame();

private:
    // Window / timing
    sf::RenderWindow m_window;
    sf::Clock m_clock;

    // Game objects
    Paddle m_paddle;
    std::vector<Ball> m_balls;
    Brick m_bricks;
    std::vector<PowerUps> m_powerUps;

    // Game state
    unsigned int m_lives{ 3 };
    unsigned int m_score{ 0 };

    // UI
    TextManager m_textManager;
};