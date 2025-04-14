#ifndef BONUS_H
#define BONUS_H

#include <SFML/Graphics.hpp>

enum class BonusType {
    Speed,
    Shield,
    AttackUp
};

class Bonus {
public:
    Bonus(float x, float y, BonusType type); // Добавляем тип бонуса
    ~Bonus();

    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    bool isActive() const;
    void deactivate();
    BonusType getType() const; // Получаем тип бонуса

private:
    sf::CircleShape m_shape;
    float m_x, m_y;
    float m_duration;
    float m_timer;
    bool m_isActive;
    BonusType m_type; // Тип бонуса
};

#endif