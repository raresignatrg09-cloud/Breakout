#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>

class Menu
{
public:
    struct Item
    {
        std::string text;
        sf::Vector2f position;
        sf::FloatRect bounds;

        std::function<void()> onClick;

        bool highlighted = false;
    };

    void addItem(const std::string& text,
        const sf::Vector2f& position,
        const sf::FloatRect& bounds,
        std::function<void()> callback);

    void moveUp();
    void moveDown();

    void updateHighlight();
    bool updateMouseHover(sf::Vector2f mousePosition);
    bool click(sf::Vector2f mousePosition);

    void setSelectedIndex(std::size_t index);
    void setBounds(std::size_t index, const sf::FloatRect& bounds);

    void clickSelected();

    std::size_t getSelectedIndex() const;

    const std::vector<Item>& getItems() const;

    void clear();

private:
	std::vector<Item> m_items;
	std::size_t m_selectedIndex{ 0 };
};