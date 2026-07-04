#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

class MusicManager
{
public:
    void loadMusic(const std::string& name,
        const std::string& path);

    void play(const std::string& name);
    void stop();

    void setLooping(bool looping);
    void setVolume(float volume);

private:
    std::unordered_map<std::string, std::string> m_paths;
    sf::Music m_music;
};