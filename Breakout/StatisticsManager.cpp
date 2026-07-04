#include "StatisticsManager.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool StatisticsManager::load(const std::string& filename)
{
	std::ifstream file(filename);  

	if (!file.is_open())
		return false;

	json j;
	file >> j;

    m_statistics.highestScore = j.value("highestScore", 0);
    m_statistics.gamesPlayed = j.value("gamesPlayed", 0);
	m_statistics.gamesLost = j.value("gamesLost", 0);
    m_statistics.gamesWon = j.value("gamesWon", 0);
    m_statistics.bricksDestroyed = j.value("bricksDestroyed", 0);
    m_statistics.ballsLost = j.value("ballsLost", 0);
    m_statistics.powerUpsCollected = j.value("powerUpsCollected", 0);
    m_statistics.totalPlayTime = j.value("totalPlayTime", 0ULL);

    return true;
}

bool StatisticsManager::save(const std::string& filename) const
{
    json j;

    j["highestScore"] = m_statistics.highestScore;
    j["gamesPlayed"] = m_statistics.gamesPlayed;
	j["gamesLost"] = m_statistics.gamesLost; 
	j["gamesLost"] = m_statistics.gamesLost;
    j["gamesWon"] = m_statistics.gamesWon;
    j["bricksDestroyed"] = m_statistics.bricksDestroyed;
    j["ballsLost"] = m_statistics.ballsLost;
    j["powerUpsCollected"] = m_statistics.powerUpsCollected;
    j["totalPlayTime"] = m_statistics.totalPlayTime;

    std::ofstream file(filename);

    if (!file.is_open())
        return false;

    file << j.dump(4);
    file.close();

    return true;
}

void StatisticsManager::increaseGamesPlayed()
{
    m_statistics.gamesPlayed++;
}

void StatisticsManager::increaseBricksDestroyed()
{
    m_statistics.bricksDestroyed++;
}

void StatisticsManager::increaseBallsLost()
{
	m_statistics.ballsLost++;
}

void StatisticsManager::increaseGamesWon()
{
	m_statistics.gamesWon++;
}

void StatisticsManager::increaseGamesLost()
{
	m_statistics.gamesLost++;
}

void StatisticsManager::addPowerUpCollected()
{
    m_statistics.powerUpsCollected++;
}

void StatisticsManager::updateHighestScore(int score)
{
	m_statistics.highestScore = std::max(m_statistics.highestScore, score);
}

void StatisticsManager::addPlayTime(sf::Time time)
{
	m_accumulator += time;

	while (m_accumulator.asSeconds() >= 1.0f)
	{
		m_statistics.totalPlayTime++;
		m_accumulator -= sf::seconds(1.0f);
	}
}
