#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

enum class BulletType {
    Normal,
    Fast,
    Explosive
};

class Bullet {
public:
    Bullet(float x, float y, float angle, BulletType type); // Добавляем тип пули
    ~Bullet();

    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    bool isAlive() const;
    BulletType getType() const;

private:
    sf::RectangleShape m_shape;
    float m_speed;
    sf::Vector2f m_position;
    float m_angle;
    bool m_isAlive;
    BulletType m_type; // Добавляем тип пули
};

#endif