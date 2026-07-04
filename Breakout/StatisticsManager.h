#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <string>

struct Statistics
{
    int highestScore = 0;
    int gamesPlayed = 0;
    int gamesWon = 0;
	int gamesLost = 0;

    int bricksDestroyed = 0;
    int ballsLost = 0;
    int powerUpsCollected = 0;

    std::uint64_t totalPlayTime = 0;
};

class StatisticsManager
{
public:
	bool load(const std::string& filename);
	bool save(const std::string& filename) const;

	Statistics& getStatistics() { return m_statistics; }
	const Statistics& getStatistics() const { return m_statistics; }

    void increaseGamesPlayed();
    void increaseBricksDestroyed();
	void increaseBallsLost();
	void increaseGamesWon();
	void increaseGamesLost();
    void addPowerUpCollected();
    void updateHighestScore(int score);
    void addPlayTime(sf::Time time);

private:
	Statistics m_statistics;

    sf::Time m_accumulator = sf::Time::Zero;
};

