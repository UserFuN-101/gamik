#include "Bonus.h"

Bonus::Bonus(float x, float y, BonusType type) : m_x(x), m_y(y), m_duration(5.0f), m_timer(0.0f), m_isActive(true), m_type(type) { // Инициализация типа бонуса
    m_shape.setRadius(16);
    switch (type) {
    case BonusType::Speed:
        m_shape.setFillColor(sf::Color::Blue);
        break;
    case BonusType::Shield:
        m_shape.setFillColor(sf::Color::Cyan);
        break;
    case BonusType::AttackUp:
        m_shape.setFillColor(sf::Color::Magenta);
        break;
    }
    m_shape.setPosition(m_x, m_y);
}

Bonus::~Bonus() {

}

void Bonus::update(float deltaTime) {
    if (!m_isActive) return;

    m_timer += deltaTime;
    if (m_timer >= m_duration) {
        m_isActive = false;
    }
}

void Bonus::render(sf::RenderWindow& window) const {
    if (m_isActive) {
        window.draw(m_shape);
    }
}

sf::FloatRect Bonus::getBounds() const {
    return m_shape.getGlobalBounds();
}

bool Bonus::isActive() const {
    return m_isActive;
}

void Bonus::deactivate() {
    m_isActive = false;
}

BonusType Bonus::getType() const {
    return m_type;
}