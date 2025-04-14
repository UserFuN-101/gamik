#include "Game.h"
#include <iostream>
#include <cstdlib> // Для rand() и srand()
#include <ctime>   // Для time()

Game::Game(int windowWidth, int windowHeight, const std::string& windowTitle)
    : m_window(sf::VideoMode(windowWidth, windowHeight), windowTitle),
    m_player1(100, 100, m_player1Texture, m_weaponTexture, m_shieldTexture),
    m_player2(700, 500, m_player2Texture, m_weaponTexture, m_shieldTexture),
    m_enemySpawnTimer(0.0f),
    m_enemySpawnInterval(2.0f),
    m_levelTimer(0.0f),
    m_currentLevel(1),
    m_bonusSpawnTimer(0.0f),
    m_bonusSpawnInterval(10.0f),
    m_scorePlayer1(0),
    m_scorePlayer2(0),
    m_comboTimer(0.0f),
    m_comboInterval(1.0f),
    m_comboMultiplierPlayer1(1),
    m_comboMultiplierPlayer2(1)
{
    m_enemies.reserve(10);
    m_bullets.reserve(50); // Резервируем место для пуль
    srand(static_cast<unsigned int>(time(0)));

    // Загрузка шрифта
    if (!font.loadFromFile("../assets/fonts/arial.ttf")) {
        std::cerr << "Failed to load font \"../assets/fonts/arial.ttf\"!" << std::endl;
    }

    // Загрузка текстур
    if (!m_normalEnemyTexture.loadFromFile("../assets/images/vrag1.png")) {
        std::cerr << "Failed to load texture \"../assets/images/vrag1.png\"!" << std::endl;
    }
    if (!m_fastEnemyTexture.loadFromFile("../assets/images/vrag2.png")) {
        std::cerr << "Failed to load texture \"../assets/images/vrag2.png\"!" << std::endl;
    }
    if (!m_shooterEnemyTexture.loadFromFile("../assets/images/vrag3.png")) {
        std::cerr << "Failed to load texture \"../assets/images/vrag3.png\"!" << std::endl;
    }
    if (!m_player1Texture.loadFromFile("../assets/images/player1.png")) {
        std::cerr << "Failed to load texture \"../assets/images/player1.png\"!" << std::endl;
    }
    if (!m_player2Texture.loadFromFile("../assets/images/player2.png")) {
        std::cerr << "Failed to load texture \"../assets/images/player2.png\"!" << std::endl;
    }
    if (!m_weaponTexture.loadFromFile("../assets/images/gun.png")) {
        std::cerr << "Failed to load texture \"../assets/images/gun.png\"!" << std::endl;
    }
    if (!m_shieldTexture.loadFromFile("../assets/images/shit.png")) {
        std::cerr << "Failed to load texture \"../assets/images/shit.png\"!" << std::endl;
    }
}

Game::~Game() {

}

void Game::run() {
    while (m_window.isOpen()) {
        float deltaTime = m_clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();

        if (isGameOver()) {
            std::cout << "Game Over!" << std::endl;
            std::cout << "Level: " << m_currentLevel << std::endl;
            std::cout << "P1 Score: " << m_scorePlayer1 << std::endl;
            std::cout << "P2 Score: " << m_scorePlayer2 << std::endl;

            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("Game Over!");
            gameOverText.setCharacterSize(40);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(m_window.getSize().x / 2.0f - gameOverText.getGlobalBounds().width / 2.0f, m_window.getSize().y / 2.0f - 50);
            m_window.draw(gameOverText);

            sf::Text player1ScoreText;
            player1ScoreText.setFont(font);
            player1ScoreText.setString("P1 Score: " + std::to_string(m_scorePlayer1));
            player1ScoreText.setCharacterSize(30);
            player1ScoreText.setFillColor(sf::Color::White);
            player1ScoreText.setPosition(m_window.getSize().x / 2.0f - 150, m_window.getSize().y / 2.0f + 10);
            m_window.draw(player1ScoreText);

            sf::Text player2ScoreText;
            player2ScoreText.setFont(font);
            player2ScoreText.setString("P2 Score: " + std::to_string(m_scorePlayer2));
            player2ScoreText.setCharacterSize(30);
            player2ScoreText.setFillColor(sf::Color::White);
            player2ScoreText.setPosition(m_window.getSize().x / 2.0f + 50, m_window.getSize().y / 2.0f + 10);
            m_window.draw(player2ScoreText);
            m_window.display(); // Обязательно отобразить изменения

            sf::Event event;
            while (true) {
                while (m_window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        m_window.close();
                        return;
                    }
                    if (event.type == sf::Event::KeyPressed) {
                        return; // Завершаем игру при нажатии любой клавиши
                    }
                }
            }
        }
    }
}

void Game::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
    }
}

void Game::update(float deltaTime) {
    m_player1.update(deltaTime, sf::Keyboard::W, sf::Keyboard::A, sf::Keyboard::S, sf::Keyboard::D, sf::Keyboard::F, sf::Keyboard::Space);
    m_player2.update(deltaTime, sf::Keyboard::Up, sf::Keyboard::Left, sf::Keyboard::Down, sf::Keyboard::Right, sf::Keyboard::RControl, sf::Keyboard::RShift);

    m_enemySpawnTimer += deltaTime;
    if (m_enemySpawnTimer >= m_enemySpawnInterval) {
        spawnEnemy();
        m_enemySpawnTimer = 0.0f;
    }

    for (auto& enemy : m_enemies) {
        enemy.update(deltaTime, m_player1.getPosition(), m_player2.getPosition(), m_bullets); // Передаем bullets
    }

    for (auto& bullet : m_bullets) {
        bullet.update(deltaTime);
    }

    m_bonusSpawnTimer += deltaTime;
    if (m_bonusSpawnTimer >= m_bonusSpawnInterval) {
        spawnBonus();
        m_bonusSpawnTimer = 0.0f;
    }

    handleCollisions();

    // Удаляем пули, которые вышли за пределы экрана
    for (auto it = m_bullets.begin(); it != m_bullets.end(); ) {
        if (!it->isAlive()) {
            it = m_bullets.erase(it);
        }
        else {
            ++it;
        }
    }

    m_levelTimer += deltaTime;
    if (m_levelTimer >= 60.0f) {
        m_currentLevel++;
        m_levelTimer = 0.0f;
        m_enemySpawnInterval = std::max(0.5f, m_enemySpawnInterval - 0.2f); // Уменьшаем интервал появления врагов
        std::cout << "Level up! Level: " << m_currentLevel << std::endl;
    }

    // Обновление таймеров комбо
    if (m_comboTimer > 0.0f) {
        m_comboTimer -= deltaTime;
    }
    else {
        m_comboMultiplierPlayer1 = 1;
        m_comboMultiplierPlayer2 = 1;
    }
}

void Game::render() {
    m_window.clear();

    m_player1.render(m_window);
    m_player2.render(m_window);

    for (const auto& enemy : m_enemies) {
        enemy.render(m_window);
    }

    for (const auto& bonus : m_bonuses) {
        bonus.render(m_window);
    }

    for (const auto& bullet : m_bullets) {
        bullet.render(m_window);
    }

    // Отображение информации об игроках (здоровье)
    sf::Text player1HealthText;
    player1HealthText.setFont(font);
    player1HealthText.setString("P1 Health: " + std::to_string(m_player1.getHealth()));
    player1HealthText.setCharacterSize(20);
    player1HealthText.setFillColor(sf::Color::White);
    player1HealthText.setPosition(10, 10);
    m_window.draw(player1HealthText);

    sf::Text player2HealthText;
    player2HealthText.setFont(font);
    player2HealthText.setString("P2 Health: " + std::to_string(m_player2.getHealth()));
    player2HealthText.setCharacterSize(20);
    player2HealthText.setFillColor(sf::Color::White);
    player2HealthText.setPosition(10, 40);
    m_window.draw(player2HealthText);

    sf::Text levelText;
    levelText.setFont(font);
    levelText.setString("Level: " + std::to_string(m_currentLevel));
    levelText.setCharacterSize(20);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(10, 70);
    m_window.draw(levelText);

    // Отображение очков
    sf::Text player1ScoreText;
    player1ScoreText.setFont(font);
    player1ScoreText.setString("P1 Score: " + std::to_string(m_scorePlayer1));
    player1ScoreText.setCharacterSize(20);
    player1ScoreText.setFillColor(sf::Color::White);
    player1ScoreText.setPosition(10, 100);
    m_window.draw(player1ScoreText);

    sf::Text player2ScoreText;
    player2ScoreText.setFont(font);
    player2ScoreText.setString("P2 Score: " + std::to_string(m_scorePlayer2));
    player2ScoreText.setCharacterSize(20);
    player2ScoreText.setFillColor(sf::Color::White);
    player2ScoreText.setPosition(10, 130);
    m_window.draw(player2ScoreText);

    m_window.display();
}

void Game::spawnEnemy() {
    float x = static_cast<float>(rand() % m_window.getSize().x);
    float y = static_cast<float>(rand() % m_window.getSize().y);
    EnemyType type = EnemyType::Normal; // По умолчанию обычные враги
    if (rand() % 10 < 3) {
        type = EnemyType::Fast; // 30% шанс быстрых
    }
    else if (rand() % 10 < 5) {
        type = EnemyType::Shooter; // 20% шанс стреляющих
    }
    m_enemies.emplace_back(x, y, type, m_normalEnemyTexture, m_fastEnemyTexture, m_shooterEnemyTexture);
}

void Game::spawnBonus() {
    float x = static_cast<float>(rand() % m_window.getSize().x);
    float y = static_cast<float>(rand() % m_window.getSize().y);
    BonusType type = static_cast<BonusType>(rand() % 3); // Случайный тип бонуса
    m_bonuses.emplace_back(x, y, type); // Создаем бонус с указанным типом
}

void Game::handleCollisions() {
    // Проверка столкновений с бонусами
    for (auto it = m_bonuses.begin(); it != m_bonuses.end(); ) {
        if (it->isActive()) {
            if (m_player1.isAlive() && m_player1.getBounds().intersects(it->getBounds())) {
                BonusType type = it->getType();
                switch (type) {
                case BonusType::Speed:
                    m_player1.activateSpeedBonus(5.0f, 1.5f); // Увеличиваем скорость на 50% на 5 секунд
                    break;
                case BonusType::Shield:
                    m_player1.activateShieldPassiveBonus(10.0f); // Пассивное восстановление щита на 5% в секунду в течение 10 секунд
                    break;
                case BonusType::AttackUp:
                    m_player1.activateAttackUpBonus(5.0f, 2.0f); // Увеличиваем урон в 2 раза на 5 секунд
                    break;
                }
                it->deactivate();
            }
            if (m_player2.isAlive() && m_player2.getBounds().intersects(it->getBounds())) {
                BonusType type = it->getType();
                switch (type) {
                case BonusType::Speed:
                    m_player2.activateSpeedBonus(5.0f, 1.5f); // Увеличиваем скорость на 50% на 5 секунд
                    break;
                case BonusType::Shield:
                    m_player2.activateShieldPassiveBonus(10.0f); // Пассивное восстановление щита на 5% в секунду в течение 10 секунд
                    break;
                case BonusType::AttackUp:
                    m_player2.activateAttackUpBonus(5.0f, 2.0f); // Увеличиваем урон в 2 раза на 5 секунд
                    break;
                }
                it->deactivate();
            }
        }
        if (!it->isActive()) {
            it = m_bonuses.erase(it);
        }
        else {
            ++it;
        }
    }

    // Проверка столкновений с пулями
    for (auto it = m_bullets.begin(); it != m_bullets.end(); ) {
        if (it->isAlive()) {
            if (m_player1.isAlive() && m_player1.getBounds().intersects(it->getBounds())) {
                if (!m_player1.hasShield()) {
                    m_player1.takeDamage(1);

                    // Проверяем тип пули, если Explosive, то наносим урон по области
                    if (it->getType() == BulletType::Explosive) {
                        // Взрыв! Наносим урон в небольшом радиусе
                        float explosionRadius = 50.0f;
                        if (std::sqrt(std::pow(m_player1.getPosition().x - it->getBounds().left, 2) +
                            std::pow(m_player1.getPosition().y - it->getBounds().top, 2)) < explosionRadius) {
                            m_player1.takeDamage(1);
                        }
                    }
                }
                it = m_bullets.erase(it); // Удаляем пулю
            }
            else if (m_player2.isAlive() && m_player2.getBounds().intersects(it->getBounds())) {
                if (!m_player2.hasShield()) {
                    m_player2.takeDamage(1);
                    // Проверяем тип пули, если Explosive, то наносим урон по области
                    if (it->getType() == BulletType::Explosive) {
                        // Взрыв! Наносим урон в небольшом радиусе
                        float explosionRadius = 50.0f;
                        if (std::sqrt(std::pow(m_player2.getPosition().x - it->getBounds().left, 2) +
                            std::pow(m_player2.getPosition().y - it->getBounds().top, 2)) < explosionRadius) {
                            m_player2.takeDamage(1);
                        }
                    }
                }
                it = m_bullets.erase(it); // Удаляем пулю
            }
            else {
                ++it;
            }
        }
        else {
            it = m_bullets.erase(it);
        }
    }

    // Простая проверка столкновений с врагами
    for (auto it = m_enemies.begin(); it != m_enemies.end(); ) {
        bool collidedWithPlayer1 = m_player1.isAlive() && m_player1.getBounds().intersects(it->getBounds());
        bool collidedWithPlayer2 = m_player2.isAlive() && m_player2.getBounds().intersects(it->getBounds());

        if (collidedWithPlayer1 || collidedWithPlayer2) {
            if (m_player1.isAttacking() && collidedWithPlayer1) {
                it->takeDamage(static_cast<int>(1 * m_player1.getAttackMultiplier())); // Учитываем множитель урона
                std::cout << "Player 1 attacks enemy!" << std::endl;
            }
            else if (m_player2.isAttacking() && collidedWithPlayer2) {
                it->takeDamage(static_cast<int>(1 * m_player2.getAttackMultiplier())); // Учитываем множитель урона
                std::cout << "Player 2 attacks enemy!" << std::endl;
            }
            else {
                // Столкновение! (например, уменьшить здоровье игрока)
                if (collidedWithPlayer1) {
                    if (!m_player1.hasShield()) {
                        m_player1.takeDamage(1);
                    }
                }
                if (collidedWithPlayer2) {
                    if (!m_player2.hasShield()) {
                        m_player2.takeDamage(1);
                    }
                }
            }
            if (!it->isAlive()) {
                int enemyScore = 10; // Обычный враг
                if (it->getType() == EnemyType::Fast) {
                    enemyScore = 20;
                }
                else if (it->getType() == EnemyType::Shooter) {
                    enemyScore = 30;
                }

                if (collidedWithPlayer1 && m_player1.isAttacking()) {
                    m_scorePlayer1 += enemyScore * m_comboMultiplierPlayer1;
                    m_comboTimer = m_comboInterval;
                    m_comboMultiplierPlayer1 = std::min(5, m_comboMultiplierPlayer1 + 1); // Максимальный множитель - 5
                    std::cout << "Player 1 Combo x" << m_comboMultiplierPlayer1 << std::endl;
                }
                else if (collidedWithPlayer2 && m_player2.isAttacking()) {
                    m_scorePlayer2 += enemyScore * m_comboMultiplierPlayer2;
                    m_comboTimer = m_comboInterval;
                    m_comboMultiplierPlayer2 = std::min(5, m_comboMultiplierPlayer2 + 1);
                    std::cout << "Player 2 Combo x" << m_comboMultiplierPlayer2 << std::endl;
                }
                it = m_enemies.erase(it);
            }
            else {
                ++it;
            }

        }
        else {
            ++it;
        }
    }
}

bool Game::isGameOver() {
    return (!m_player1.isAlive() && !m_player2.isAlive());
}