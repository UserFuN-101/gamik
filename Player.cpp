#include "Player.h"
#include <iostream>

Player::Player(float x, float y, const sf::Texture& playerTexture, const sf::Texture& weaponTexture, const sf::Texture& shieldTexture) : m_baseSpeed(200.0f), m_speed(m_baseSpeed), m_position(x, y), m_health(3), m_isAlive(true), m_isAttacking(false), m_attackTimer(0.0f), m_attackCooldown(4.0f), m_attackDuration(1.0f), m_hasSpeedBonus(false), m_speedBonusTimer(0.0f), m_speedBonusDuration(0.0f), m_speedBonusMultiplier(1.0f), m_attackEffectTimer(0.0f), m_attackEffectDuration(0.1f), m_hasShield(false), m_shieldTimer(0.0f), m_shieldDuration(0.0f), m_shieldStrength(100.0f), m_shieldRegenTimer(0.0f), m_shieldRegenRate(1.0f), m_shieldPassiveRegenTimer(0.0f), m_shieldPassiveRegenDuration(0.0f), m_attackMultiplier(1.0f), m_attackUpTimer(0.0f), m_attackUpDuration(0.0f) // Инициализация таймеров
{
    m_sprite.setTexture(playerTexture);
    m_sprite.setPosition(m_position);

    m_weaponSprite.setTexture(weaponTexture);
    m_weaponSprite.setOrigin(m_weaponSprite.getLocalBounds().width / 2, m_weaponSprite.getLocalBounds().height / 2);

    m_shieldSprite.setTexture(shieldTexture);
    m_shieldSprite.setOrigin(m_shieldSprite.getLocalBounds().width / 2, m_shieldSprite.getLocalBounds().height / 2);

    m_attackEffect.setSize(sf::Vector2f(40, 40));
    m_attackEffect.setFillColor(sf::Color::White);  // Цвет вспышки
    m_attackEffect.setOrigin(m_attackEffect.getSize().x / 2.0f, m_attackEffect.getSize().y / 2.0f);  // Центрируем
}

Player::~Player() {

}

void Player::update(float deltaTime, sf::Keyboard::Key upKey, sf::Keyboard::Key leftKey, sf::Keyboard::Key downKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key shieldKey, sf::Keyboard::Key attackKey) {
    if (!m_isAlive) return;

    if (m_isAttacking) {
        m_attackTimer += deltaTime;
        if (m_attackTimer >= m_attackDuration) {
            m_isAttacking = false;
            m_attackTimer = 0.0f;
            m_speed = m_baseSpeed; // Возвращаем обычную скорость после атаки
        }
    }
    else {
        if (m_attackTimer > 0.0f) {
            m_attackTimer += deltaTime;
            if (m_attackTimer >= m_attackCooldown) {
                m_attackTimer = 0.0f;
            }
        }
    }

    if (m_hasSpeedBonus) {
        m_speedBonusTimer += deltaTime;
        if (m_speedBonusTimer >= m_speedBonusDuration) {
            m_hasSpeedBonus = false;
            m_speedBonusTimer = 0.0f;
            m_speed = m_baseSpeed; // Возвращаем базовую скорость
        }
    }

    if (m_hasShield) {
        m_shieldTimer += deltaTime;
        if (m_shieldTimer >= m_shieldDuration) {
            m_hasShield = false;
            m_shieldTimer = 0.0f;
        }
        m_shieldStrength -= 2.0f * deltaTime; // Щит теряет 2% прочности в секунду
        if (m_shieldStrength <= 0) {
            m_hasShield = false;
            m_shieldRegenTimer = 10.0f; // Начинаем восстановление через 10 секунд
        }
    }
    else {
        if (m_shieldRegenTimer > 0) {
            m_shieldRegenTimer -= deltaTime;
            if (m_shieldRegenTimer <= 0) {
                m_shieldRegenRate = 0.5f; // Восстановление начинается с 0.5% в секунду
            }
        }
        else if (m_shieldStrength < 100) {
            m_shieldStrength += m_shieldRegenRate * deltaTime;
            if (m_shieldStrength >= 100) {
                m_shieldStrength = 100;
                m_shieldRegenRate = 1.0f; // Возвращаем нормальную скорость восстановления
            }
        }
    }

    if (m_shieldPassiveRegenTimer > 0) {
        m_shieldPassiveRegenTimer -= deltaTime;
        if (m_shieldPassiveRegenTimer > 0) {
            m_shieldStrength += 5.0f * deltaTime; // Пассивное восстановление 5% в секунду
            if (m_shieldStrength > 100) {
                m_shieldStrength = 100;
            }
        }
    }

    if (m_attackUpTimer > 0) {
        m_attackUpTimer -= deltaTime;
        if (m_attackUpTimer <= 0) {
            m_attackMultiplier = 1.0f;
            m_attackUpTimer = 0.0f;
        }
    }

    if (sf::Keyboard::isKeyPressed(leftKey)) {
        m_position.x -= m_speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(rightKey)) {
        m_position.x += m_speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(upKey)) {
        m_position.y -= m_speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(downKey)) {
        m_position.y += m_speed * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(attackKey) && !m_isAttacking && m_attackTimer == 0.0f) {
        attack();
    }
    if (sf::Keyboard::isKeyPressed(shieldKey) && !m_hasShield && m_shieldStrength > 0) {
        m_hasShield = true;
        m_shieldTimer = 0.0f;
        m_shieldDuration = 3.0f; // Щит активен 3 секунды
    }

    m_sprite.setPosition(m_position);
    m_weaponSprite.setPosition(m_position.x + m_sprite.getLocalBounds().width / 2, m_position.y + m_sprite.getLocalBounds().height / 2);
    m_shieldSprite.setPosition(m_position.x + m_sprite.getLocalBounds().width / 2, m_position.y + m_sprite.getLocalBounds().height / 2);
}

void Player::render(sf::RenderWindow& window) const {
    if (m_isAlive) {
        window.draw(m_sprite);

        // Отрисовка оружия
        window.draw(m_weaponSprite);

        // Отрисовка щита
        if (m_hasShield) {
            window.draw(m_shieldSprite);
        }

        // Отрисовка эффекта атаки
        if (m_isAttacking) {
            sf::Transform transform;
            transform.translate(m_position.x + m_sprite.getLocalBounds().width / 2, m_position.y + m_sprite.getLocalBounds().height / 2);
            transform.translate(-m_attackEffect.getSize().x / 2.0f, -m_attackEffect.getSize().y / 2.0f);
            window.draw(m_attackEffect, transform);
        }
    }
}

sf::FloatRect Player::getBounds() const {
    return m_sprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return m_position;
}

int Player::getHealth() const {
    return m_health;
}

void Player::takeDamage(int damage) {
    if (m_hasShield) {
        m_shieldStrength -= 30.0f; // Щит теряет 30% прочности при попадании
        if (m_shieldStrength <= 0) {
            m_hasShield = false;
            m_shieldRegenTimer = 10.0f; // Начинаем восстановление через 10 секунд
        }
        std::cout << "Shield blocked damage! Shield strength: " << m_shieldStrength << std::endl;
        return; // Если есть щит, урон не наносится
    }
    m_health -= damage;
    if (m_health <= 0) {
        m_isAlive = false;
        m_sprite.setColor(sf::Color::Black); // Отображаем мертвого игрока черным
    }
    std::cout << "Player took damage. Health: " << m_health << std::endl;
}

bool Player::isAlive() const {
    return m_isAlive;
}

void Player::attack() {
    m_isAttacking = true;
    m_attackTimer = 0.0f;
    m_speed = 50.0f; // Замедляем игрока во время атаки
    std::cout << "Player attacks!" << std::endl;
}

bool Player::isAttacking() const {
    return m_isAttacking;
}

void Player::activateSpeedBonus(float duration, float speedMultiplier) {
    m_hasSpeedBonus = true;
    m_speedBonusTimer = 0.0f;
    m_speedBonusDuration = duration;
    m_speedBonusMultiplier = speedMultiplier;
    m_speed = m_baseSpeed * m_speedBonusMultiplier;
    std::cout << "Speed bonus activated!" << std::endl;
}

void Player::activateShieldBonus(float duration) {
    m_hasShield = true;
    m_shieldTimer = 0.0f;
    m_shieldDuration = duration;
    std::cout << "Shield bonus activated!" << std::endl;
}

void Player::activateAttackUpBonus(float duration, float attackMultiplier) {
    m_attackMultiplier = attackMultiplier;
    m_attackUpTimer = duration;
    m_attackUpDuration = duration;
    std::cout << "Attack Up bonus activated!" << std::endl;
}

bool Player::hasShield() const {
    return m_hasShield;
}

float Player::getAttackMultiplier() const {
    return m_attackMultiplier;
}

void Player::activateShieldPassiveBonus(float duration) {
    m_shieldPassiveRegenTimer = duration;
    m_shieldPassiveRegenDuration = duration;
    std::cout << "Shield passive bonus activated!" << std::endl;
}