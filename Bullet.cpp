#include "Bullet.h"
#include <cmath> // Для sin и cos

Bullet::Bullet(float x, float y, float angle, BulletType type) : m_position(x, y), m_angle(angle), m_isAlive(true), m_type(type) {
    m_shape.setSize(sf::Vector2f(5, 5));
    m_shape.setFillColor(sf::Color::Magenta);
    if (type == BulletType::Normal) {
        m_speed = 200.0f;
    }
    else if (type == BulletType::Fast) {
        m_speed = 300.0f;
        m_shape.setFillColor(sf::Color::Yellow);
    }
    else if (type == BulletType::Explosive) {
        m_speed = 150.0f;
        m_shape.setFillColor(sf::Color::Red);
    }
    m_shape.setPosition(m_position);
}

Bullet::~Bullet() {
    // Освобождение ресурсов (если нужно)
}

void Bullet::update(float deltaTime) {
    m_position.x += m_speed * cos(m_angle) * deltaTime;
    m_position.y += m_speed * sin(m_angle) * deltaTime;
    m_shape.setPosition(m_position);

    // Проверка выхода за границы экрана
    if (m_position.x < 0 || m_position.x > 800 || m_position.y < 0 || m_position.y > 600) {
        m_isAlive = false;
    }
}

void Bullet::render(sf::RenderWindow& window) const {
    if (m_isAlive) {
        window.draw(m_shape);
    }
}

sf::FloatRect Bullet::getBounds() const {
    return m_shape.getGlobalBounds();
}

bool Bullet::isAlive() const {
    return m_isAlive;
}

BulletType Bullet::getType() const {
    return m_type;
}