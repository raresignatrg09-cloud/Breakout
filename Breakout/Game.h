#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <memory>
#include <iostream>

#include "TextManager.h"
#include "SoundManager.h"
#include "MusicManager.h"
#include "StatisticsManager.h"
#include "Config.hpp"

#include "Scene.h"
#include "MainMenuScene.h"

class Game
{
public:
    Game();
    void run();

    template<typename T, typename... Args>
    void changeScene(Args&&... args);

    template<typename T, typename... Args>
    void pushScene(Args&&... args);

    void popScene();

    Scene* getCurrentScene();

    sf::RenderWindow& getWindow()       noexcept{ return m_window; }
    TextManager&      getTextManager()  noexcept { return m_textManager; }
    SoundManager&     getSoundManager() noexcept { return m_soundManager; }
    MusicManager&     getMusicManager() noexcept { return m_musicManager; }
	StatisticsManager& getStatisticsManager() noexcept { return m_statisticsManager; }

    void createText(const std::string& id, const std::string& font, const std::string& string,
        unsigned int characterSize, sf::Vector2f position, sf::Color color = sf::Color::White,
        sf::Text::Style style = sf::Text::Style::Regular);

private:
    // Core loop
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

private:
    // Window / timing
    sf::RenderWindow m_window;
    sf::Clock m_clock;
    std::vector<std::unique_ptr<Scene>> m_scenes;

	sf::Texture m_texture;
	std::unique_ptr<sf::Sprite> m_bgSprite;

	// Managers
    TextManager m_textManager;
	SoundManager m_soundManager;
	MusicManager m_musicManager;
	StatisticsManager m_statisticsManager;
};


template<typename T, typename ...Args>
inline void Game::changeScene(Args && ...args)
{
    m_scenes.clear();

    m_scenes.push_back(
        std::make_unique<T>(*this, std::forward<Args>(args)...)
    );
}

template<typename T, typename ...Args>
inline void Game::pushScene(Args && ...args)
{
    m_scenes.push_back(
        std::make_unique<T>(*this, std::forward<Args>(args)...)
    );
}