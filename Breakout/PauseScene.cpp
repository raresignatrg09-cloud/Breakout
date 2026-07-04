#include "PauseScene.h"
#include "OptionsScene.h"
#include "Game.h"

PauseScene::PauseScene(Game& game)
	:Scene(game)
{
	auto& tm = m_game.getTextManager();
	auto windowSize = m_game.getWindow().getSize();

	m_menu.addItem(
		"Resume",
		{ static_cast<float>(windowSize.x) / 2.f, 250.f },
		{ {static_cast<float>(windowSize.x) / 2.f, 250.f}, {250.f, 50.f} },
		[this]() { m_game.popScene(); });

	m_menu.addItem(
		"Options",
		{ static_cast<float>(windowSize.x) / 2.f, 310.f },
		{ {static_cast<float>(windowSize.x) / 2.f, 310.f}, {250.f, 50.f} },
		[this]() { m_game.pushScene<OptionsScene>(); });

	m_menu.addItem(
		"Main Menu",
		{ static_cast<float>(windowSize.x) / 2.f, 370.f },
		{ {static_cast<float>(windowSize.x) / 2.f, 370.f}, {250.f, 50.f} },
		[this]() { m_game.changeScene<MainMenuScene>(); });

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

	overlay.setSize({ static_cast<float>(windowSize.x),static_cast<float>(windowSize.y) });
	overlay.setPosition({ 0.0f,0.0f });
	overlay.setFillColor({ 0,0,0,150 });
}

PauseScene::~PauseScene()
{
	overlay.setFillColor(sf::Color::Transparent);

	auto& tm = m_game.getTextManager();

	for (std::size_t i = 0; i < m_menu.getItems().size(); ++i)
		tm.removeText(std::string(MENU_PREFIX) + std::to_string(i));

	m_menu.clear();
}

void PauseScene::handleEvent(const sf::Event & event)
{
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->code == sf::Keyboard::Key::Escape)
			m_game.getWindow().close();

		if (keyPressed->code == sf::Keyboard::Key::Up || keyPressed->code == sf::Keyboard::Key::W)
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
		sf::Vector2f mousePos =
			m_game.getWindow().mapPixelToCoords(mouseMoved->position);

		if (m_menu.updateMouseHover(mousePos))
			updateSelection();
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

void PauseScene::update(sf::Time deltaTime)
{
	updateSelection();
	m_game.getTextManager().update(deltaTime);
}

void PauseScene::render(sf::RenderWindow& window)
{
	m_game.getWindow().draw(overlay);
}

void PauseScene::updateSelection()
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