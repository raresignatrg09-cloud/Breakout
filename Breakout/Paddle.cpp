#include "Paddle.h"
#include <iostream>

Paddle::Paddle()
	: m_speed(PaddleConfig::PADDLE_SPEED),
	m_velocity(0.f, 0.f)
{
	m_shape.setSize({ PaddleConfig::PADDLE_WIDTH, PaddleConfig::PADDLE_HEIGHT });
	m_shape.setOutlineThickness(2.f);
	//m_shape.setOutlineColor(sf::Color::Red);
	m_shape.setFillColor(sf::Color::Blue);

	m_shape.setPosition({
		WinConfig::WINDOW_WIDTH / 2.f - m_shape.getSize().x / 2.f,
		WinConfig::WINDOW_HEIGHT - m_shape.getSize().y - 10.f
		});

	loadTextures("paddle", "assets/sprites/paddle.png");
	loadTextures("paddle_small", "assets/sprites/paddle_small.png");
	loadTextures("paddle_wide", "assets/sprites/paddle_wide.png");

	m_sprite = std::make_unique<sf::Sprite>(m_textures["paddle"]);

	sf::FloatRect bounds = m_sprite->getLocalBounds();

	m_sprite->setScale({
		PaddleConfig::PADDLE_WIDTH / bounds.size.x,
		PaddleConfig::PADDLE_HEIGHT / bounds.size.y
		});

	m_sprite->setPosition(m_shape.getPosition());
}

void Paddle::update(sf::Time deltaTime)
{
	move(deltaTime);
	outOfBoundsCheck();

	if (m_size != PaddleState::Normal)
	{
		m_powerUpTime -= deltaTime;
		std::cout << "Power-up time remaining: " << m_powerUpTime.asSeconds() << " seconds\n";
		if (m_powerUpTime <= sf::Time::Zero)
		{
			normal();
		}
	}
}

void Paddle::render(sf::RenderWindow& window)
{
	window.draw(m_shape);
	
	window.draw(*m_sprite);
}

void Paddle::reset()
{
	m_shape.setPosition({
		WinConfig::WINDOW_WIDTH / 2.f - m_shape.getSize().x / 2.f,
		WinConfig::WINDOW_HEIGHT - m_shape.getSize().y - 10.f
		});
	
	m_sprite->setPosition(m_shape.getPosition());
	m_velocity = {0.f, 0.f};
}

void Paddle::shrink()
{
	setPaddleAppearance(
		PaddleConfig::PADDLE_WIDTH * 0.75f,
		"paddle_small"
	);

	m_size = PaddleState::Shrunk;
}

void Paddle::expand()
{
	setPaddleAppearance(
		PaddleConfig::PADDLE_WIDTH * 1.25f,
		"paddle_wide"
	);

	m_size = PaddleState::Expanded;
}

void Paddle::normal()
{
	setPaddleAppearance(
		PaddleConfig::PADDLE_WIDTH,
		"paddle"
	);

	m_size = PaddleState::Normal;
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

		m_sprite->move(m_velocity * deltaTime.asSeconds());
}

void Paddle::outOfBoundsCheck()
{
	if (m_shape.getPosition().x < 0.f)
	{
		m_shape.setPosition({ 0.f, m_shape.getPosition().y });
		
		m_sprite->setPosition(m_shape.getPosition());
	}
	else if (m_shape.getPosition().x + m_shape.getSize().x > WinConfig::WINDOW_WIDTH)
	{
		m_shape.setPosition({
			WinConfig::WINDOW_WIDTH - m_shape.getSize().x,
			m_shape.getPosition().y
			});

		m_sprite->setPosition(m_shape.getPosition());
	}
}

void Paddle::setPaddleAppearance(float width, const std::string& textureName)
{
	float centerX = m_shape.getPosition().x + m_shape.getSize().x / 2.f;

    m_shape.setSize({
        width,
        PaddleConfig::PADDLE_HEIGHT
    });

    m_shape.setPosition({
        centerX - width / 2.f,
        m_shape.getPosition().y
    });

    m_sprite->setTexture(m_textures[textureName]);

    auto bounds = m_sprite->getLocalBounds();

    m_sprite->setScale({
        width / bounds.size.x,
        PaddleConfig::PADDLE_HEIGHT / bounds.size.y
    });

    m_sprite->setPosition(m_shape.getPosition());
}

void Paddle::loadTextures(const std::string& name, const std::string& path)
{
	if (!m_textures[name].loadFromFile(path))
		throw std::runtime_error("Failed to load paddle texture!");
}
