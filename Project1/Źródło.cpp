#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

class Paddle {
private:
    sf::RectangleShape paddle;
    float speed = 0.5f;
    sf::Vector2f windowSize;

public:
    Paddle(float windowWidth, float windowHeight) {
        windowSize.x = windowWidth;
        windowSize.y = windowHeight;

        paddle.setSize(sf::Vector2f(100.0f, 20.0f));
        paddle.setFillColor(sf::Color::Blue);
        paddle.setPosition(windowWidth / 2 - 50, windowHeight - 50);
    }

    sf::RectangleShape getPaddle() {
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

    sf::FloatRect getBounds() {
        return paddle.getGlobalBounds();
    }
};

class Ball {
private:
    sf::CircleShape ball;
    sf::Vector2f velocity;
    float speed = 0.05f;

public:
    Ball(float windowWidth, float windowHeight) {
        ball.setRadius(10.0f);
        ball.setFillColor(sf::Color::White);
        resetPosition(windowWidth, windowHeight);
    }

    sf::CircleShape getBall() {
        return ball;
    }

    void move() {
        ball.move(velocity);
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

    sf::FloatRect getBounds() {
        return ball.getGlobalBounds();
    }
    
    void resetPosition(float windowWidth, float windowHeight) {
        ball.setPosition(windowWidth / 2 - ball.getRadius(), windowHeight / 2 - ball.getRadius());
        velocity = sf::Vector2f(speed, -speed);
    }

    void reverseY() {
        velocity.y = -velocity.y;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML Paddle Game");
    sf::Event event;

    Paddle paddle(500, 500);
    Ball ball(500, 500);

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

	// Tworzenie bloków
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            sf::RectangleShape block;
            block.setSize(sf::Vector2f(blockWidth, blockHeight));
            block.setFillColor(sf::Color::Green);
            block.setPosition(j * (blockWidth + padding), i * (blockHeight + padding));
            blocks.push_back(block);
        }
    }

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
    gameOverText.setPosition(100, 200);
    gameOverText.setString("");

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!gameOver) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                paddle.moveLeft();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                paddle.moveRight();
            }

            ball.move();
            ball.bounceWindowBounds(500, 500);

            // Kolizja pi³ki z paletk¹
            if (ball.getBounds().intersects(paddle.getBounds())) {
                ball.bouncePaddle();
            }

            // Kolizja pi³ki z blokami
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

            // Kolizja z doln¹ lini¹ (koniec gry)
            if (ball.getBounds().intersects(redBar.getGlobalBounds())) {
                gameOver = true;
                gameOverText.setString("Koniec gry!\nZniszczone bloki: " + std::to_string(blocksDestroyed));
            }

            scoreText.setString("Blocks destroyed: " + std::to_string(blocksDestroyed));
        }

        window.clear(sf::Color::Black);

        if (!gameOver) {
            window.draw(paddle.getPaddle());
            window.draw(ball.getBall());
            for (const auto& block : blocks) {
                window.draw(block);
            }
            window.draw(redBar);
            window.draw(scoreText);
        }
        else {
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}