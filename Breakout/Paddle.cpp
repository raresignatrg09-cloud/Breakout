#include "Paddle.h"

Paddle::Paddle()
	: m_speed(PaddleConfig::PADDLE_SPEED), m_velocity(0.f, 0.f)
{
	m_shape.setSize({ PaddleConfig::PADDLE_WIDTH, PaddleConfig::PADDLE_HEIGHT });
	m_shape.setFillColor(sf::Color::Blue);
	m_shape.setPosition({WinConfig::WINDOW_WIDTH / 2.f - m_shape.getSize().x / 2.f, WinConfig::WINDOW_HEIGHT - m_shape.getSize().y - 10.f});
}

void Paddle::update(sf::Time deltaTime)
{
	move(deltaTime);
	outOfBoundsCheck();
	applyFriction(deltaTime);
}

void Paddle::render(sf::RenderWindow& window)
{
	window.draw(m_shape);
}

void Paddle::reset()
{
	m_shape.setPosition({WinConfig::WINDOW_WIDTH / 2.f - m_shape.getSize().x / 2.f, 550.f});
	m_velocity = {0.f, 0.f};
}

void Paddle::move(sf::Time deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		m_velocity.x = -m_speed;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		m_velocity.x = m_speed;
	else
		m_velocity.x = 0.f;

	m_shape.move(m_velocity * deltaTime.asSeconds());
}

void Paddle::outOfBoundsCheck()
{
	if (m_shape.getPosition().x < 0.f)
		m_shape.setPosition({0.f, m_shape.getPosition().y});

	else if (m_shape.getPosition().x + m_shape.getSize().x > WinConfig::WINDOW_WIDTH)
		m_shape.setPosition({WinConfig::WINDOW_WIDTH - m_shape.getSize().x, m_shape.getPosition().y});
}

void Paddle::applyFriction(sf::Time deltaTime)
{
	if (m_shape.getPosition().x > 0.f && m_shape.getPosition().x + m_shape.getSize().x < WinConfig::WINDOW_WIDTH)
		m_velocity.x *= 0.9f;
}
