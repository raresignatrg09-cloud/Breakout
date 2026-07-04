#include "MusicManager.h"

#include <stdexcept>

void MusicManager::loadMusic(const std::string& name, const std::string& path)
{
	m_paths[name] = path;
}

void MusicManager::play(const std::string& name)
{
	auto it = m_paths.find(name);

	if (it == m_paths.end())
		throw std::runtime_error("Music not found: " + name);

	if (!m_music.openFromFile(it->second))
		throw std::runtime_error("Failed to load music: " + it->second);

	m_music.play();
}

void MusicManager::stop()
{
	m_music.stop();
}

void MusicManager::setLooping(bool looping)
{
	m_music.setLooping(looping);
}

void MusicManager::setVolume(float volume)
{
	m_music.setVolume(volume);
}