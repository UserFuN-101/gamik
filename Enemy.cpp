#define _USE_MATH_DEFINES // Добавлено
#include "Enemy.h"
#include <cmath> // Для atan2
#include <iostream>
#include <random>

Enemy::Enemy(float x, float y, EnemyType type, const sf::Texture& normalTexture, const sf::Texture& fastTexture, const sf::Texture& shooterTexture) : m_position(x, y), m_type(type), m_isAlive(true), m_shootTimer(0.0f), m_shootInterval(2.0f), m_health(0), m_speed(0.0f) {
    if (type == EnemyType::Normal) {
        m_speed = 50.0f;
        m_health = 1;
        m_sprite.setTexture(normalTexture);
    }
    else if (type == EnemyType::Fast) {
        m_speed = 100.0f;
        m_health = 1;
        m_sprite.setTexture(fastTexture);
    }
    else if (type == EnemyType::Shooter) {
        m_speed = 30.0f; // Замедлим стрелков
        m_health = 2;
        m_sprite.setTexture(shooterTexture);
    }
    m_sprite.setPosition(m_position);
}

Enemy::~Enemy() {
    //  Если нужно освободить ресурсы, сделай это здесь
}

void Enemy::update(float deltaTime, sf::Vector2f player1Position, sf::Vector2f player2Position, std::vector<Bullet>& bullets) {
    if (!m_isAlive) return;

    sf::Vector2f targetPosition;

    // Выбор цели
    sf::Vector2f direction1 = player1Position - m_position;
    sf::Vector2f direction2 = player2Position - m_position;

    float distance1 = std::sqrt(direction1.x * direction1.x + direction1.y * direction1.y);
    float distance2 = std::sqrt(direction2.x * direction2.x + direction2.y * direction2.y);

    targetPosition = (distance1 < distance2) ? player1Position : player2Position;

    sf::Vector2f direction = targetPosition - m_position;
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (m_type == EnemyType::Normal || m_type == EnemyType::Fast) {
        if (magnitude > 0) {
            direction /= magnitude;
            m_position += direction * m_speed * deltaTime;
        }
    }
    else if (m_type == EnemyType::Shooter) {
        // Стрелок держится на расстоянии
        float desiredDistance = 200.0f;
        if (magnitude < desiredDistance) {
            direction = -direction;
        }
        if (magnitude > 0) {
            direction /= magnitude;
            m_position += direction * m_speed * deltaTime;
        }

        m_shootTimer += deltaTime;
        if (m_shootTimer >= m_shootInterval) {
            float angle = getRandomAngle();
            BulletType bulletType = BulletType::Explosive; // Для стрелков - Explosive
            bullets.emplace_back(m_position.x, m_position.y, angle, bulletType);
            m_shootTimer = 0.0f;
        }
    }

    // Ограничение по границам экрана (для всех типов врагов)
    if (m_position.x < 0) m_position.x = 0;
    if (m_position.x > 800 - m_sprite.getGlobalBounds().width) m_position.x = 800 - m_sprite.getGlobalBounds().width;
    if (m_position.y < 0) m_position.y = 0;
    if (m_position.y > 600 - m_sprite.getGlobalBounds().height) m_position.y = 600 - m_sprite.getGlobalBounds().height;

    m_sprite.setPosition(m_position);
}

void Enemy::render(sf::RenderWindow& window) const {
    if (m_isAlive) {
        window.draw(m_sprite);
    }
}

sf::FloatRect Enemy::getBounds() const {
    return m_sprite.getGlobalBounds();
}

bool Enemy::isAlive() const {
    return m_isAlive;
}

void Enemy::takeDamage(int damage) {
    m_health -= damage;
    if (m_health <= 0) {
        m_isAlive = false;
        m_sprite.setColor(sf::Color::Transparent); // Делаем врага невидимым
    }
}

float Enemy::getRandomAngle() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0, 2.0f * static_cast<float>(M_PI)); // или 2.0f * M_PI_F
    return distrib(gen);
}

EnemyType Enemy::getType() const {
    return m_type;
}