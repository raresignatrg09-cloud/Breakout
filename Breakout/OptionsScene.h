#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"

class OptionsScene:public Scene
{
public:
	OptionsScene(Game& game);

	void handleEvent(const sf::Event& event) override;
	void update(sf::Time deltaTime) override;
	void render(sf::RenderWindow& window) override;

private:

};

