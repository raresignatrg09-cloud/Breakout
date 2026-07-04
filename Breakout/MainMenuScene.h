#pragma once
#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "Menu.h"

class MainMenuScene : public Scene
{
public:
	MainMenuScene(Game& game);
	~MainMenuScene() override;

	void handleEvent(const sf::Event& event) override;
	void update(sf::Time deltaTime) override;
	void render(sf::RenderWindow&) override;

	void updateSelection();

private:
	Menu m_menu;

	static constexpr std::string_view MENU_PREFIX = "main_menu_";
};