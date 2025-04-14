#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Bonus.h"
#include "Bullet.h"
#include <vector>

class Game {
public:
    Game(int windowWidth, int windowHeight, const std::string& windowTitle);
    ~Game();

    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    sf::RenderWindow m_window;
    sf::Clock m_clock;
    Player m_player1;
    Player m_player2;
    std::vector<Enemy> m_enemies;
    std::vector<Bonus> m_bonuses;
    std::vector<Bullet> m_bullets;
    float m_enemySpawnTimer;
    float m_enemySpawnInterval;
    float m_levelTimer;
    int m_currentLevel;
    float m_bonusSpawnTimer;
    float m_bonusSpawnInterval;
    int m_scorePlayer1;
    int m_scorePlayer2;

    float m_comboTimer;
    float m_comboInterval;
    int m_comboMultiplierPlayer1;
    int m_comboMultiplierPlayer2;

    sf::Font font; // Добавили объявление переменной font
    sf::Texture m_normalEnemyTexture;
    sf::Texture m_fastEnemyTexture;
    sf::Texture m_shooterEnemyTexture;
    sf::Texture m_player1Texture;
    sf::Texture m_player2Texture;
    sf::Texture m_weaponTexture;
    sf::Texture m_shieldTexture;

    void spawnEnemy();
    void spawnBonus();
    void handleCollisions();
    bool isGameOver();
};

#endif