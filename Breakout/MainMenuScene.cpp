#include "MainMenuScene.h"
#include "Game.h"
#include "GameplayScene.h"
#include "OptionsScene.h"
#include "StatisticsMenu.h"

MainMenuScene::MainMenuScene(Game& game)
	:Scene(game)
{
	auto& tm = m_game.getTextManager();
	auto windowSize = m_game.getWindow().getSize();

	tm.clearTexts();

	m_menu.addItem(
		"Start Game",
		{ static_cast<float>(windowSize.x) / 2.f, 250.f },
		{ {static_cast<float>(windowSize.x) / 2.f, 250.f}, {250.f, 50.f} },
		[this]() { m_game.changeScene<GameplayScene>(); });

	m_menu.addItem(
		"Options",
		{ static_cast<float>(windowSize.x) / 2.f, 310.f },
		{ {static_cast<float>(windowSize.x) / 2.f, 310.f}, {250.f, 50.f} },
		[this]() { m_game.changeScene<OptionsScene>(); });

	m_menu.addItem(
		"Statistics",
		{ static_cast<float>(windowSize.x) / 2.f, 370.f },
		{ {static_cast<float>(windowSize.x) / 2.f, 370.f}, {250.f, 50.f} },
		[this]() { m_game.changeScene<StatisticsMenu>(); });

	m_menu.addItem(
		"Exit",
		{ static_cast<float>(windowSize.x) / 2.f, 430.f },
		{ {static_cast<float>(windowSize.x) / 2.f, 430.f}, {250.f, 50.f} },
		[this]() { m_game.getWindow().close(); });

	for (std::size_t i = 0; i < m_menu.getItems().size(); ++i)
	{
		const auto& item = m_menu.getItems()[i];

		std::string id = std::string(MENU_PREFIX) + std::to_string(i);

		tm.createText(id, "font", item.text, 40, item.position);
		tm.setAlignment(id, TextManager::Alignment::Center);
		tm.setShadow(id, sf::Color(0, 0, 0, 150), { 3.f, 3.f });
		tm.animateScale(id, 1.f, 1.1f, sf::seconds(0.5f), false);

		auto bounds = tm.get(id).getGlobalBounds();

		m_menu.setBounds(i, bounds);
	}

	tm.createText("title", "font", "Breakout Game", 70, { static_cast<float>(windowSize.x) / 2.f, 100.f });
	tm.setAlignment("title", TextManager::Alignment::Center);
	tm.setShadow("title", sf::Color(0, 0, 0, 150), { 3.f, 3.f });
}

MainMenuScene::~MainMenuScene()
{
	m_menu.clear();

	m_game.getTextManager().clearTexts();
}

void MainMenuScene::handleEvent(const sf::Event & event)
{
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->code == sf::Keyboard::Key::Escape)
			m_game.getWindow().close();

		if (keyPressed->code == sf::Keyboard::Key::Up||keyPressed->code==sf::Keyboard::Key::W)
		{
			m_menu.moveUp();
			updateSelection();
		}
		
		if (keyPressed->code == sf::Keyboard::Key::Down || keyPressed->code == sf::Keyboard::Key::S)
		{
			m_menu.moveDown();
			updateSelection();
		}

		if (keyPressed->code == sf::Keyboard::Key::Enter)
			m_menu.clickSelected();
	}

	if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
	{
		sf::Vector2f mousePos = m_game.getWindow().mapPixelToCoords(mouseMoved->position);

		m_menu.updateMouseHover(mousePos);
	}

	if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mouseButton->button == sf::Mouse::Button::Left)
		{
			sf::Vector2f mousePos = m_game.getWindow().mapPixelToCoords(mouseButton->position);
			m_menu.click(mousePos);
		}
	}
}

void MainMenuScene::update(sf::Time deltaTime)
{
	updateSelection();

	m_game.getTextManager().update(deltaTime);
}

void MainMenuScene::render(sf::RenderWindow& window)
{
}

void MainMenuScene::updateSelection()
{
	auto& tm = m_game.getTextManager();

	for (std::size_t i = 0; i < m_menu.getItems().size(); ++i)
	{
		tm.setColor(
			std::string(MENU_PREFIX) + std::to_string(i),
			m_menu.getItems()[i].highlighted
			? sf::Color::Yellow
			: sf::Color::White
		);

		tm.animateScale(
			std::string(MENU_PREFIX) + std::to_string(i),
			m_menu.getItems()[i].highlighted ? 1.1f : 1.f,
			m_menu.getItems()[i].highlighted ? 1.2f : 1.f,
			sf::seconds(0.5f),
			false
		);
	}
}
