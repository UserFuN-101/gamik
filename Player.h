#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
public:
    Player(float x, float y, const sf::Texture& playerTexture, const sf::Texture& weaponTexture, const sf::Texture& shieldTexture);
    ~Player();

    void update(float deltaTime, sf::Keyboard::Key upKey, sf::Keyboard::Key leftKey, sf::Keyboard::Key downKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key shieldKey, sf::Keyboard::Key attackKey);
    void render(sf::RenderWindow& window) const; // Добавлено const

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    int getHealth() const;
    void takeDamage(int damage);
    bool isAlive() const;
    void attack();
    bool isAttacking() const;
    void activateSpeedBonus(float duration, float speedMultiplier);
    void activateShieldBonus(float duration);
    void activateAttackUpBonus(float duration, float attackMultiplier);
    bool hasShield() const;
    float getAttackMultiplier() const;
    void activateShieldPassiveBonus(float duration);

private:
    sf::Sprite m_sprite;
    sf::Sprite m_weaponSprite;
    sf::Sprite m_shieldSprite;
    float m_speed;
    float m_baseSpeed;
    sf::Vector2f m_position;
    int m_health;
    bool m_isAlive;

    bool m_isAttacking;
    float m_attackTimer;
    float m_attackCooldown;
    float m_attackDuration;

    bool m_hasSpeedBonus;
    float m_speedBonusTimer;
    float m_speedBonusDuration;
    float m_speedBonusMultiplier;

    sf::RectangleShape m_attackEffect; // Добавляем прямоугольник для вспышки
    float m_attackEffectTimer;          // Таймер для вспышки
    float m_attackEffectDuration;       // Длительность вспышки

    bool m_hasShield; // Флаг наличия щита
    float m_shieldTimer;
    float m_shieldDuration;
    float m_shieldStrength; // Прочность щита
    float m_shieldRegenTimer; // Таймер восстановления щита
    float m_shieldRegenRate; // Скорость восстановления щита
    float m_shieldPassiveRegenTimer; // Таймер пассивного восстановления щита
    float m_shieldPassiveRegenDuration; // Длительность пассивного восстановления щита

    float m_attackMultiplier; // Множитель урона
    float m_attackUpTimer;
    float m_attackUpDuration;
};

#endif