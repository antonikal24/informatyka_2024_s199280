#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

struct GameStats {
    int level;
    int blocksDestroyed;
};

class Paddle {
private:
    sf::RectangleShape paddle;
    float speed = 0.4f;
    sf::Vector2f windowSize;

public:
    Paddle(float windowWidth, float windowHeight) {
        windowSize.x = windowWidth;
        windowSize.y = windowHeight;

        paddle.setSize(sf::Vector2f(100.0f, 20.0f));
        paddle.setFillColor(sf::Color::Blue);
        paddle.setPosition(windowWidth / 2 - 50, windowHeight - 50);
    }

    sf::RectangleShape getPaddle() const {
        return paddle;
    }

    void moveLeft() {
        if (paddle.getPosition().x > 0)
            paddle.move(-speed, 0);
    }

    void moveRight() {
        if (paddle.getPosition().x + paddle.getSize().x < windowSize.x)
            paddle.move(speed, 0);
    }

    sf::FloatRect getBounds() const {
        return paddle.getGlobalBounds();
    }

    void resetPosition() {
        paddle.setPosition(windowSize.x / 2 - 50, windowSize.y - 50);
    }
};

class Ball {
private:
    sf::CircleShape ball;
    sf::Vector2f velocity;
    float baseSpeed = 0.05f;
    float speedFactor = 1.0f;

public:
    Ball(float windowWidth, float windowHeight) {
        ball.setRadius(10.0f);
        ball.setFillColor(sf::Color::White);
        resetPosition(windowWidth, windowHeight);
    }

    sf::CircleShape getBall() const {
        return ball;
    }

    void move() {
        ball.move(velocity * speedFactor);
    }

    void bounceWindowBounds(float windowWidth, float windowHeight) {
        if (ball.getPosition().x <= 0 || ball.getPosition().x + ball.getRadius() * 2 >= windowWidth)
            velocity.x = -velocity.x;
        if (ball.getPosition().y <= 0)
            velocity.y = -velocity.y;
    }

    void bouncePaddle() {
        velocity.y = -velocity.y;
    }

    sf::FloatRect getBounds() const {
        return ball.getGlobalBounds();
    }

    void resetPosition(float windowWidth, float windowHeight) {
        ball.setPosition(windowWidth / 2 - ball.getRadius(), windowHeight / 2 - ball.getRadius());
        velocity = sf::Vector2f(baseSpeed, -baseSpeed);
    }

    void reverseY() {
        velocity.y = -velocity.y;
    }

    void setSpeedFactor(float factor) {
        speedFactor = factor;
    }
};
//Funkcja resetuj¹ca grê
void resetGame(Paddle& paddle, std::vector<Ball>& balls, std::vector<sf::RectangleShape>& blocks, int& blocksDestroyed, int rows, int cols, float blockWidth, float blockHeight, float padding, int level) {
    paddle.resetPosition();
    blocks.clear();
    blocksDestroyed = 0;

    //Tworzenie bloków
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            sf::RectangleShape block;
            block.setSize(sf::Vector2f(blockWidth, blockHeight));
            block.setFillColor(sf::Color::Green);
            block.setPosition(j * (blockWidth + padding), i * (blockHeight + padding));
            blocks.push_back(block);
        }
    }

    balls.clear();
    for (int i = 0; i < level; ++i) {
        balls.emplace_back(500, 500);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "Paddle Game");
    sf::Event event;

    Paddle paddle(500, 500);
    std::vector<Ball> balls;

    sf::RectangleShape redBar;
    redBar.setSize(sf::Vector2f(500.0f, 10.0f));
    redBar.setFillColor(sf::Color::Red);
    redBar.setPosition(0, 490);

    std::vector<sf::RectangleShape> blocks;
    int rows = 5;
    int cols = 10;
    float blockWidth = 48.0f;
    float blockHeight = 18.0f;
    float padding = 2.0f;
    int blocksDestroyed = 0;
    bool gameOver = false;
    bool isPaused = false;
    bool confirmExit = false;
    float gameSpeed = 1.0f;
    int currentLevel = 1;
    int maxLevel = 3;

    resetGame(paddle, balls, blocks, blocksDestroyed, rows, cols, blockWidth, blockHeight, padding, currentLevel);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(36);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(50, 150);
    gameOverText.setString("");

    sf::Text pauseText;
    pauseText.setFont(font);
    pauseText.setCharacterSize(20);
    pauseText.setFillColor(sf::Color::Yellow);
    pauseText.setPosition(150, 150);
    pauseText.setString("Gra zatrzymana\nNacisnij F1\nzeby wznowic\n +/- do zmiany predkosci\n C - Czarny, Z - Zolty,\n P - Pomaranczowy, F - Fioletowy");

    sf::Text confirmExitText;
    confirmExitText.setFont(font);
    confirmExitText.setCharacterSize(24);
    confirmExitText.setFillColor(sf::Color::Cyan);
    confirmExitText.setPosition(50, 200);
    confirmExitText.setString("Czy napewno chcesz opuscic?\n (y-tak/n-nie/m-menu)");

    sf::Text speedText;
    speedText.setFont(font);
    speedText.setCharacterSize(15);
    speedText.setFillColor(sf::Color::White);
    speedText.setPosition(150, 290);

    sf::Text levelSelectionText;
    levelSelectionText.setFont(font);
    levelSelectionText.setCharacterSize(20);
    levelSelectionText.setFillColor(sf::Color::Yellow);
    levelSelectionText.setPosition(150, 310);
    levelSelectionText.setString("Wybierz poziom:\n1 - Poziom 1\n2 - Poziom 2\n3 - Poziom 3");

    sf::Text statsText;
    statsText.setFont(font);
    statsText.setCharacterSize(20);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(50, 150);

    bool gameStarted = false;
    bool showStats = false;

    sf::Text welcomeText;
    welcomeText.setFont(font);
    welcomeText.setCharacterSize(30);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setPosition(50, 200);
    welcomeText.setString("Witaj w grze!\nNacisnij Enter zeby rozpoczac\nNacisnij Tab zeby sprawdzic statystyki");

    std::vector<GameStats> gameStats;
    sf::Color backgroundColor = sf::Color::Black;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (!gameStarted) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        gameStarted = true;
                    }
                    if (event.key.code == sf::Keyboard::Tab) {
                        showStats = true;
                    }
                }
                else {
                    if (!confirmExit) {
                        if (event.key.code == sf::Keyboard::F1) {
                            isPaused = !isPaused;
                        }
                        if (event.key.code == sf::Keyboard::Escape) {
                            confirmExit = true;
                            isPaused = true;
                        }
                        if (isPaused) {
                            if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) {
                                gameSpeed += 0.1f;
                            }
                            if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Dash) {
                                gameSpeed = std::max(0.1f, gameSpeed - 0.1f);
                            }
                            //wybór poziomu
                            if (event.key.code == sf::Keyboard::Num1) {
                                currentLevel = 1;
                                resetGame(paddle, balls, blocks, blocksDestroyed, rows, cols, blockWidth, blockHeight, padding, currentLevel);
                                isPaused = false;
                            }
                            if (event.key.code == sf::Keyboard::Num2) {
                                currentLevel = 2;
                                resetGame(paddle, balls, blocks, blocksDestroyed, rows, cols, blockWidth, blockHeight, padding, currentLevel);
                                isPaused = false;
                            }
                            if (event.key.code == sf::Keyboard::Num3) {
                                currentLevel = 3;
                                resetGame(paddle, balls, blocks, blocksDestroyed, rows, cols, blockWidth, blockHeight, padding, currentLevel);
                                isPaused = false;
                            }

                            if (event.key.code == sf::Keyboard::C) {
                                backgroundColor = sf::Color::Black;
                            }
                            if (event.key.code == sf::Keyboard::Z) {
                                backgroundColor = sf::Color::Yellow;
                            }
                            if (event.key.code == sf::Keyboard::P) {
                                backgroundColor = sf::Color(255, 165, 0);
                            }
                            if (event.key.code == sf::Keyboard::F) {
                                backgroundColor = sf::Color(128, 0, 128);
                            }
                        }

                        if (gameOver && event.key.code == sf::Keyboard::R) {
                            currentLevel = 1;
                            resetGame(paddle, balls, blocks, blocksDestroyed, rows, cols, blockWidth, blockHeight, padding, currentLevel);
                            gameOver = false;
                        }
                    }
                    else {
                        if (event.key.code == sf::Keyboard::Y) {
                            window.close();
                        }
                        if (event.key.code == sf::Keyboard::N) {
                            confirmExit = false;
                            isPaused = false;
                        }
                        if (event.key.code == sf::Keyboard::M) {
                            gameStarted = false;
                            confirmExit = false;
                        }
                    }
                }
            }
        }

        for (auto& ball : balls) {
            ball.setSpeedFactor(gameSpeed);
        }

        if (gameStarted) {
            if (!isPaused && !gameOver && !confirmExit) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    paddle.moveLeft();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    paddle.moveRight();
                }

                for (auto& ball : balls) {
                    ball.move();
                    ball.bounceWindowBounds(500, 500);
                    //Kolizja pi³i z paletk¹ 
                    if (ball.getBounds().intersects(paddle.getBounds())) {
                        ball.bouncePaddle();
                    }

                    //Kolizja pi³ki z blokami
                    for (auto it = blocks.begin(); it != blocks.end();) {
                        if (ball.getBounds().intersects(it->getGlobalBounds())) {
                            it = blocks.erase(it);
                            ball.reverseY();
                            blocksDestroyed++;
                        }
                        else {
                            ++it;
                        }
                    }
                    //Kolizja z doln¹ lini¹ (koniec gry)
                    if (ball.getBounds().intersects(redBar.getGlobalBounds())) {
                        //Zbieranie statystyk po przegranej
                        gameStats.push_back({ currentLevel, blocksDestroyed });
                        gameOver = true;
                        gameOverText.setString("Game Over!\nBlocks destroyed: " + std::to_string(blocksDestroyed) + "\nNacinij R ¿eby zrestartowac");
                    }
                }
                //Sprawdzanie czy wszytskie bloki zosta³y zniszczone
                if (blocks.empty() && currentLevel < maxLevel) {
                    currentLevel++;
                    resetGame(paddle, balls, blocks, blocksDestroyed, rows, cols, blockWidth, blockHeight, padding, currentLevel);
                }
                //Sprawdzanie czy wszystkie bloki zosta³y zniszone i czy gracz wygra³
                else if (blocks.empty() && currentLevel == maxLevel) {
                    //zbiieranie statystk po wykranej
                    gameStats.push_back({ currentLevel, blocksDestroyed });
                    gameOver = true;
                    gameOverText.setString("Wygra³eœ!\nBlocks destroyed: " + std::to_string(blocksDestroyed) + "\nNacisnij R ¿eby zrestartowac");
                }
            }
        }


        window.clear(backgroundColor);

        if (!gameStarted) {
            if (showStats) {
                statsText.setString("Statystyki:\n");
                for (size_t i = 0; i < gameStats.size(); ++i) {
                    statsText.setString(statsText.getString() + "Level: " + std::to_string(gameStats[i].level) + ", Bloki zniszoczne: " + std::to_string(gameStats[i].blocksDestroyed) + "\n");
                }
                window.draw(statsText);
            }
            else {
                window.draw(welcomeText);
            }
        }
        else {
            if (!confirmExit && !gameOver) {
                for (const auto& block : blocks) {
                    window.draw(block);
                }

                for (const auto& ball : balls) {
                    window.draw(ball.getBall());
                }

                window.draw(paddle.getPaddle());
                window.draw(redBar);

                if (isPaused) {
                    window.draw(pauseText);
                    speedText.setString("Aktualna predkosc: " + std::to_string(gameSpeed));
                    window.draw(speedText);
                    window.draw(levelSelectionText);
                }

                scoreText.setString("Bloki zniszczone: " + std::to_string(blocksDestroyed) + "\nPoziom: " + std::to_string(currentLevel));
                window.draw(scoreText);
            }

            if (confirmExit) {
                window.draw(confirmExitText);
            }

            if (gameOver) {
                window.draw(gameOverText);
            }
        }

        window.display();
    }

    return 0;
}

