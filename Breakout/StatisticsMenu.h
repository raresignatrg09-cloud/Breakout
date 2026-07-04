#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"

class StatisticsMenu : public Scene
{
public:
	StatisticsMenu(Game& game);
	~StatisticsMenu() override;

	void handleEvent(const sf::Event& event) override;
	void update(sf::Time deltaTime) override;
	void render(sf::RenderWindow& window) override;

private:
	void updateStatisticsDisplay();
	void initUI();
	std::string formatPlayTime(std::uint64_t totalPlayTime);
};
