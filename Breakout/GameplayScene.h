#pragma once
#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"

class GameplayScene : public Scene
{
public:
	GameplayScene(Game& game);
	void handleEvent(const sf::Event& event) override;
	void update(sf::Time deltaTime) override;
	void render(sf::RenderWindow& window) override;

private:
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

	void InitUI();

private:
	Paddle m_paddle;
	std::vector<Ball> m_balls;
	Brick m_bricks;
	std::vector<PowerUps> m_powerUps;

	int m_lives{ 3 };
	int m_score{ 0 };

	bool m_gameOver{ false };

	sf::Time m_accumulator{ sf::Time::Zero };
};

