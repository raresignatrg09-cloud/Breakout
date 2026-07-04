#include "SoundManager.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

void SoundManager::loadSound(const std::string& name, const std::string& path)
{
	sf::SoundBuffer buffer;

	if (!buffer.loadFromFile(path))
		throw std::runtime_error("Failed to load sound: " + path);

	m_soundBuffers.emplace(name, std::move(buffer));
}

void SoundManager::play(const std::string& name)
{
	auto buffer = m_soundBuffers.find(name);
	if (buffer == m_soundBuffers.end())
		return;

	for (auto& sound : m_sounds)
	{
		if (sound.getStatus() == sf::Sound::Status::Stopped)
		{
			sound = sf::Sound(buffer->second);
			sound.setVolume(m_volume);
			sound.play();
			return;
		}
	}

	m_sounds.emplace_back(buffer->second);
	m_sounds.back().setVolume(m_volume);
	m_sounds.back().play();
}

void SoundManager::update()
{
	m_sounds.erase(
		std::remove_if(
			m_sounds.begin(),
			m_sounds.end(),
			[](const sf::Sound& sound)
			{
				return sound.getStatus() == sf::Sound::Status::Stopped;
			}
		),
		m_sounds.end()
	);
}

void SoundManager::setVolume(float volume)
{
	m_volume = volume;

	for (auto& s : m_sounds)
		s.setVolume(volume);
}
