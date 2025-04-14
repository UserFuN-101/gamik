#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"

enum class EnemyType {
    Normal,
    Fast,
    Shooter
};

class Enemy {
public:
    Enemy(float x, float y, EnemyType type, const sf::Texture& normalTexture, const sf::Texture& fastTexture, const sf::Texture& shooterTexture);
    ~Enemy();

    void update(float deltaTime, sf::Vector2f player1Position, sf::Vector2f player2Position, std::vector<Bullet>& bullets);
    void render(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    bool isAlive() const;
    void takeDamage(int damage);
    float getRandomAngle();
    EnemyType getType() const; // ƒобавл€ем метод дл€ получени€ типа врага

private:
    sf::Sprite m_sprite;
    float m_speed;
    sf::Vector2f m_position;
    int m_health;
    bool m_isAlive;
    EnemyType m_type;
    float m_shootTimer;
    float m_shootInterval;
};

#endif