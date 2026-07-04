#include "StatisticsMenu.h"
#include "MainMenuScene.h"
#include "Game.h"

StatisticsMenu::StatisticsMenu(Game& game)
	: Scene(game)
{
	initUI();
	updateStatisticsDisplay();
}

StatisticsMenu::~StatisticsMenu()
{
	auto& tm = m_game.getTextManager();

	const char* ids[] =
	{
		"title",
		"highest_score",
		"games_played",
		"games_won",
		"games_lost",
		"bricks_destroyed",
		"balls_lost",
		"powerups_collected",
		"total_play_time"
	};

	for (const auto& id : ids)
		tm.removeText(id);
}

void StatisticsMenu::handleEvent(const sf::Event & event)
{
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
		if (keyPressed->code == sf::Keyboard::Key::Escape)
			m_game.changeScene<MainMenuScene>();
}

void StatisticsMenu::update(sf::Time deltaTime)
{}

void StatisticsMenu::render(sf::RenderWindow & window)
{}

void StatisticsMenu::updateStatisticsDisplay()
{
	auto& tm = m_game.getTextManager();
	const auto& stats = m_game.getStatisticsManager().getStatistics();

	tm.setString("highest_score",
		"Highest Score: " + std::to_string(stats.highestScore));

	tm.setString("games_played",
		"Games Played: " + std::to_string(stats.gamesPlayed));

	tm.setString("games_won",
		"Games Won: " + std::to_string(stats.gamesWon));

	tm.setString("games_lost",
		"Games Lost: " + std::to_string(stats.gamesLost));

	tm.setString("bricks_destroyed",
		"Bricks Destroyed: " + std::to_string(stats.bricksDestroyed));

	tm.setString("balls_lost",
		"Balls Lost: " + std::to_string(stats.ballsLost));

	tm.setString("powerups_collected",
		"Power-ups Collected: " + std::to_string(stats.powerUpsCollected));

	tm.setString("total_play_time",
		"Total Play Time: " + formatPlayTime(stats.totalPlayTime));
}

void StatisticsMenu::initUI()
{
	auto& tm = m_game.getTextManager();
	auto windowSize = m_game.getWindow().getSize();

	const float centerX = windowSize.x / 2.f;
	const float startY = 100.f;
	const float spacing = 50.f;

	tm.createText("title", "font", "Statistics", 48, { centerX, startY });
	tm.createText("highest_score", "font", "Highest Score: 0", 24, { centerX, startY + spacing });
	tm.createText("games_played", "font", "Games Played: 0", 24, { centerX, startY + 2 * spacing });
	tm.createText("games_won", "font", "Games Won: 0", 24, { centerX, startY + 3 * spacing });
	tm.createText("games_lost", "font", "Games Lost: 0", 24, { centerX, startY + 4 * spacing });
	tm.createText("bricks_destroyed", "font", "Bricks Destroyed: 0", 24, { centerX, startY + 5 * spacing });
	tm.createText("balls_lost", "font", "Balls Lost: 0", 24, { centerX, startY + 6 * spacing });
	tm.createText("powerups_collected", "font", "Power-ups Collected: 0", 24, { centerX, startY + 7 * spacing });
	tm.createText("total_play_time", "font", "Total Play Time: 0s", 24, { centerX, startY + 8 * spacing });

	const char* ids[] = {
		"title", "highest_score", "games_played", "games_won",
		"games_lost", "bricks_destroyed", "balls_lost",
		"powerups_collected", "total_play_time"
	};

	for (const auto& id : ids)
	{
		tm.setAlignment(id, TextManager::Alignment::Center);
		tm.setShadow(id, sf::Color(0, 0, 0, 150), { 3.f, 3.f });
	}
}

std::string StatisticsMenu::formatPlayTime(std::uint64_t totalPlayTime)
{
	const std::uint64_t days = totalPlayTime / 86400;
	totalPlayTime %= 86400;

	const std::uint64_t hours = totalPlayTime / 3600;
	totalPlayTime %= 3600;

	const std::uint64_t minutes = totalPlayTime / 60;
	const std::uint64_t seconds = totalPlayTime % 60;

	std::string result;

	if (days > 0)
		result += std::to_string(days) + "d ";

	if (hours > 0)
		result += std::to_string(hours) + "h ";

	if (minutes > 0)
		result += std::to_string(minutes) + "m ";

	if (seconds > 0 || result.empty())
		result += std::to_string(seconds) + "s";

	return result;
}
