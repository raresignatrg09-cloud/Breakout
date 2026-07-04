#include "Menu.h"

void Menu::addItem(const std::string& text, const sf::Vector2f& position, const sf::FloatRect& bounds, std::function<void()> callback)
{
	m_items.push_back({ text, position, bounds, std::move(callback) });

	if (m_items.size() == 1)
		updateHighlight();
}

void Menu::moveUp()
{
	if (m_selectedIndex > 0)
	{
		--m_selectedIndex;
		updateHighlight();
	}
}

void Menu::moveDown()
{
	if (m_selectedIndex + 1 < m_items.size())
	{
		++m_selectedIndex;
		updateHighlight();
	}
}

void Menu::updateHighlight()
{
	for (std::size_t i = 0; i < m_items.size(); ++i)
		m_items[i].highlighted = (i == m_selectedIndex);
}

bool Menu::updateMouseHover(sf::Vector2f mousePosition)
{
	for (std::size_t i = 0; i < m_items.size(); ++i)
	{
		if (m_items[i].bounds.contains(mousePosition))
		{
			if (m_selectedIndex != i)
			{
				m_selectedIndex = i;
				updateHighlight();
			}

			return true;
		}
	}

	return false;
}

bool Menu::click(sf::Vector2f mousePosition)
{
	for (auto& item:m_items)
	{
		if (item.bounds.contains(mousePosition))
		{
			item.onClick();
			return true;
		}
	}

	return false;
}

void Menu::setSelectedIndex(std::size_t index)
{
	if (index < m_items.size())
	{
		m_selectedIndex = index;
		updateHighlight();
	}
}

void Menu::setBounds(std::size_t index, const sf::FloatRect& bounds)
{
	if (index < m_items.size())
		m_items.at(index).bounds = bounds;
}

void Menu::clickSelected()
{
	if (!m_items.empty())
		m_items[m_selectedIndex].onClick();
}

std::size_t Menu::getSelectedIndex() const
{
	return m_selectedIndex;
}

const std::vector<Menu::Item>& Menu::getItems() const
{
	return m_items;
}

void Menu::clear()
{
	m_items.clear();
	m_selectedIndex = 0;
}