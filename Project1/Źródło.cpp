#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// Klasa reprezentuj¹ca niebiesk¹ pi³eczkê
class BlueBall {
private:
    sf::Vector2f position; // Pozycja pi³ki
    float xVel = 1, yVel = 1; // Prêdkoœæ pi³ki w osi X i Y
    sf::CircleShape ball; // Obiekt graficzny reprezentuj¹cy pi³kê
    sf::Vector2f windowSize; // Rozmiar okna gry

public:
    // Konstruktor klasy
    BlueBall(float x_in, float y_in, float windowWidth, float windowHeight) {
        position.x = x_in;
        position.y = y_in;
        windowSize.x = windowWidth;
        windowSize.y = windowHeight;

        ball.setRadius(20.0f); // Ustawienie promienia pi³ki
        ball.setFillColor(sf::Color::Blue); // Nadanie koloru niebieskiego
        ball.setPosition(position); // Ustawienie pocz¹tkowej pozycji
    }

    // Funkcja przesuwaj¹ca pi³kê o zadane wartoœci
    void move(float x_in, float y_in) {
        sf::Vector2f displacement;
        displacement.x = x_in;
        displacement.y = y_in;
        ball.move(displacement); // Przesuniêcie graficzne
        position = ball.getPosition(); // Aktualizacja pozycji
    }

    // Funkcja zwracaj¹ca obiekt graficzny pi³ki
    sf::CircleShape getBall() {
        return ball;
    }

    // Funkcja sprawdzaj¹ca kolizje pi³ki ze œcianami okna
    void checkWallCollisions() {
        if (position.x <= 0 || position.x + ball.getRadius() * 2 >= windowSize.x)
            xVel = -xVel; // Zmiana kierunku ruchu w osi X
        if (position.y <= 0 || position.y + ball.getRadius() * 2 >= windowSize.y)
            yVel = -yVel; // Zmiana kierunku ruchu w osi Y
    }

    // Funkcja animuj¹ca ruch pi³ki
    void animate() {
        checkWallCollisions(); // Sprawdzenie kolizji
        move(xVel, yVel); // Przesuniêcie pi³ki
    }

};

// Klasa reprezentuj¹ca paletkê gracza
class Paddle {
private:
    sf::RectangleShape paddle; // Obiekt graficzny paletki
    float speed = 0.5f; // Prêdkoœæ poruszania siê paletki
    sf::Vector2f windowSize; // Rozmiar okna gry

public:
    // Konstruktor klasy
    Paddle(float windowWidth, float windowHeight) {
        windowSize.x = windowWidth;
        windowSize.y = windowHeight;

        paddle.setSize(sf::Vector2f(100.0f, 20.0f)); // Ustawienie rozmiaru paletki
        paddle.setFillColor(sf::Color::Blue); // Nadanie koloru niebieskiego
        paddle.setPosition(windowWidth / 2 - 50, windowHeight - 50); // Ustawienie pocz¹tkowej pozycji
    }

    // Funkcja zwracaj¹ca obiekt graficzny paletki
    sf::RectangleShape getPaddle() {
        return paddle;
    }

    // Funkcja poruszaj¹ca paletk¹ w lewo lub w prawo
    void moveLeft() {
        if (paddle.getPosition().x > 0)
            paddle.move(-speed, 0);
    }

    void moveRight() {
        if (paddle.getPosition().x + paddle.getSize().x < windowSize.x)
            paddle.move(speed, 0);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML Blue Ball Animation"); // Utworzenie okna gry
    sf::Event event; // Obiekt do obs³ugi zdarzeñ

    BlueBall blueBall(400, 10, 500, 500); // Utworzenie obiektu niebieskiej pi³ki
    Paddle paddle(500, 500); // Utworzenie obiektu paletki

    sf::RectangleShape redBar; // Czerwony pasek oznaczaj¹cy pora¿kê
    redBar.setSize(sf::Vector2f(500.0f, 10.0f));
    redBar.setFillColor(sf::Color::Red);
    redBar.setPosition(0, 490); // Pasek na dole okna

    sf::Clock clock; // Zegar do kontroli czasu animacji

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close(); // Zamkniêcie okna gry
        }

        // Obs³uga klawiatury do poruszania paletk¹
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            paddle.moveLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            paddle.moveRight();
        }

        window.clear(sf::Color::Black); // Czyszczenie okna kolorem czarnym
        window.draw(blueBall.getBall()); // Rysowanie pi³ki
        window.draw(paddle.getPaddle()); // Rysowanie paletki
        window.draw(redBar); // Rysowanie czerwonego paska
        window.display(); // Wyœwietlanie zawartoœci okna

        if (clock.getElapsedTime().asMilliseconds() > 5.0f) {
            blueBall.animate(); // Aktualizacja pozycji pi³ki
            clock.restart(); // Restart zegara
        }
    }

    return 0;
}
