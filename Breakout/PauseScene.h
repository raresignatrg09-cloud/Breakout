#pragma once

#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "Menu.h"

class PauseScene:public Scene
{
public:
	PauseScene(Game& game);
	~PauseScene();

	void handleEvent(const sf::Event& event) override;
	void update(sf::Time deltaTime) override;
	void render(sf::RenderWindow& window) override;

	void updateSelection();

private:
	Menu m_menu;

	sf::RectangleShape overlay;

	static constexpr std::string_view MENU_PREFIX = "pause_menu_";
};

