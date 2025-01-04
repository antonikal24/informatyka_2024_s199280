#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// Klasa reprezentuj�ca niebiesk� pi�eczk�
class BlueBall {
private:
    sf::Vector2f position; // Pozycja pi�ki
    float xVel = 1, yVel = 1; // Pr�dko�� pi�ki w osi X i Y
    sf::CircleShape ball; // Obiekt graficzny reprezentuj�cy pi�k�
    sf::Vector2f windowSize; // Rozmiar okna gry

public:
    // Konstruktor klasy
    BlueBall(float x_in, float y_in, float windowWidth, float windowHeight) {
        position.x = x_in;
        position.y = y_in;
        windowSize.x = windowWidth;
        windowSize.y = windowHeight;

        ball.setRadius(20.0f); // Ustawienie promienia pi�ki
        ball.setFillColor(sf::Color::Blue); // Nadanie koloru niebieskiego
        ball.setPosition(position); // Ustawienie pocz�tkowej pozycji
    }

    // Funkcja przesuwaj�ca pi�k� o zadane warto�ci
    void move(float x_in, float y_in) {
        sf::Vector2f displacement;
        displacement.x = x_in;
        displacement.y = y_in;
        ball.move(displacement); // Przesuni�cie graficzne
        position = ball.getPosition(); // Aktualizacja pozycji
    }

    // Funkcja zwracaj�ca obiekt graficzny pi�ki
    sf::CircleShape getBall() {
        return ball;
    }

    // Funkcja sprawdzaj�ca kolizje pi�ki ze �cianami okna
    void checkWallCollisions() {
        if (position.x <= 0 || position.x + ball.getRadius() * 2 >= windowSize.x)
            xVel = -xVel; // Zmiana kierunku ruchu w osi X
        if (position.y <= 0 || position.y + ball.getRadius() * 2 >= windowSize.y)
            yVel = -yVel; // Zmiana kierunku ruchu w osi Y
    }

    // Funkcja animuj�ca ruch pi�ki
    void animate() {
        checkWallCollisions(); // Sprawdzenie kolizji
        move(xVel, yVel); // Przesuni�cie pi�ki
    }

};

// Klasa reprezentuj�ca paletk� gracza
class Paddle {
private:
    sf::RectangleShape paddle; // Obiekt graficzny paletki
    float speed = 0.5f; // Pr�dko�� poruszania si� paletki
    sf::Vector2f windowSize; // Rozmiar okna gry

public:
    // Konstruktor klasy
    Paddle(float windowWidth, float windowHeight) {
        windowSize.x = windowWidth;
        windowSize.y = windowHeight;

        paddle.setSize(sf::Vector2f(100.0f, 20.0f)); // Ustawienie rozmiaru paletki
        paddle.setFillColor(sf::Color::Blue); // Nadanie koloru niebieskiego
        paddle.setPosition(windowWidth / 2 - 50, windowHeight - 50); // Ustawienie pocz�tkowej pozycji
    }

    // Funkcja zwracaj�ca obiekt graficzny paletki
    sf::RectangleShape getPaddle() {
        return paddle;
    }

    // Funkcja poruszaj�ca paletk� w lewo lub w prawo
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
    sf::Event event; // Obiekt do obs�ugi zdarze�

    BlueBall blueBall(400, 10, 500, 500); // Utworzenie obiektu niebieskiej pi�ki
    Paddle paddle(500, 500); // Utworzenie obiektu paletki

    sf::RectangleShape redBar; // Czerwony pasek oznaczaj�cy pora�k�
    redBar.setSize(sf::Vector2f(500.0f, 10.0f));
    redBar.setFillColor(sf::Color::Red);
    redBar.setPosition(0, 490); // Pasek na dole okna

    sf::Clock clock; // Zegar do kontroli czasu animacji

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close(); // Zamkni�cie okna gry
        }

        // Obs�uga klawiatury do poruszania paletk�
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            paddle.moveLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            paddle.moveRight();
        }

        window.clear(sf::Color::Black); // Czyszczenie okna kolorem czarnym
        window.draw(blueBall.getBall()); // Rysowanie pi�ki
        window.draw(paddle.getPaddle()); // Rysowanie paletki
        window.draw(redBar); // Rysowanie czerwonego paska
        window.display(); // Wy�wietlanie zawarto�ci okna

        if (clock.getElapsedTime().asMilliseconds() > 5.0f) {
            blueBall.animate(); // Aktualizacja pozycji pi�ki
            clock.restart(); // Restart zegara
        }
    }

    return 0;
}
