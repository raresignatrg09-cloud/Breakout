#include "Game.h"

#include <algorithm>

Game::Game()
	:m_window(sf::VideoMode({ WinConfig::WINDOW_WIDTH, WinConfig::WINDOW_HEIGHT }), WinConfig::WINDOW_TITLE)
{
	m_window.setFramerateLimit(60);

	if (!m_texture.loadFromFile("assets/sprites/space.jpg"))
		throw std::runtime_error("Failed to load background texture");
	
	std::cout << m_texture.getSize().x << " "
		<< m_texture.getSize().y << '\n';

	m_bgSprite = std::make_unique<sf::Sprite>(m_texture);
	
	m_bgSprite->setPosition({ 0.f, 0.f });

	std::cout << m_bgSprite->getLocalBounds().size.x << " "
		<< m_bgSprite->getLocalBounds().size.y << '\n';

	m_textManager.loadFont("font", "assets/fonts/font.ttf");

	m_soundManager.loadSound("brick_hit", "assets/sounds/brick_hit.wav");
	m_soundManager.loadSound("paddle_hit", "assets/sounds/paddle_hit.wav");
	m_soundManager.setVolume(50.f);

	m_musicManager.loadMusic("background", "assets/sounds/background.ogg");
	m_musicManager.play("background");
	m_musicManager.setLooping(true);
	m_musicManager.setVolume(10.f);

	m_statisticsManager.load("stats.json");

	m_scenes.push_back(std::make_unique<MainMenuScene>(*this));
}


void Game::run()
{
	while (m_window.isOpen())
	{
		sf::Time dt = m_clock.restart();
		processEvents();
		update(dt);
		render();

		m_statisticsManager.save("stats.json");
	}
}

void Game::popScene()
{
	if (!m_scenes.empty())
		m_scenes.pop_back();
}

Scene* Game::getCurrentScene()
{
	return m_scenes.back().get();
}

void Game::processEvents()
{
	while (const std::optional event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			m_window.close();

		if (getCurrentScene())
			getCurrentScene()->handleEvent(*event);
	}
}

void Game::update(sf::Time deltaTime)
{
	m_soundManager.update();

	if (getCurrentScene())
		getCurrentScene()->update(deltaTime);
}

void Game::render()
{
	m_window.clear();

	m_window.draw(*m_bgSprite);

	for(auto& scene:m_scenes)
		scene->render(m_window);

	m_textManager.draw(m_window);

	m_window.display();
}

void Game::createText(const std::string& id, const std::string& font, const std::string& string, unsigned int characterSize, sf::Vector2f position, sf::Color color, sf::Text::Style style)
{
	m_textManager.createText(id, font, string, characterSize, position);
	m_textManager.setColor(id, color);
	m_textManager.setStyle(id, style);
}