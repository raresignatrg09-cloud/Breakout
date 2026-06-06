#include "PowerUps.h"

PowerUps::PowerUps(powerUpType type, sf::Vector2f position)
	: m_type(type)	
{
	m_shape.setRadius(15.f);
	m_shape.setOrigin({ m_shape.getRadius(), m_shape.getRadius() });
	m_shape.setPosition(position);

    switch (m_type)
    {
    case powerUpType::ExpandPaddle:
        m_shape.setFillColor(sf::Color::Green);
        break;

    case powerUpType::ShrinkPaddle:
        m_shape.setFillColor(sf::Color::Red);
        break;

    case powerUpType::ExtraLife:
        m_shape.setFillColor(sf::Color::Yellow);
        break;

    case powerUpType::MultiBall:
        m_shape.setFillColor(sf::Color::Cyan);
        break;

    case powerUpType::StickyPaddle:
        m_shape.setFillColor(sf::Color::Magenta);
        break;

    default:
        m_shape.setFillColor(sf::Color::White);
        break;
    }
}

void PowerUps::update(sf::Time deltaTime)
{
	m_shape.move({0.f, m_fallSpeed * deltaTime.asSeconds()});
}

void PowerUps::render(sf::RenderWindow & window)
{
	window.draw(m_shape);
}

bool PowerUps::isOffScreen(const sf::Vector2u & windowSize) const
{
	return m_shape.getPosition().y > windowSize.y;
}

