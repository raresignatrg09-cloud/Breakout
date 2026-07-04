#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

class SoundManager
{
public:
	SoundManager() = default;
    void loadSound(const std::string& name, const std::string& path);
    void play(const std::string& name);
    void update();

    void setVolume(float volume);

private:
    std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;

    std::vector<sf::Sound> m_sounds;

    float m_volume = 100.f;
};