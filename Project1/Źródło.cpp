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

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML Blue Ball Animation"); // Utworzenie okna gry
    sf::Event event; // Obiekt do obs³ugi zdarzeñ

    BlueBall blueBall(400, 10, 500, 500); // Utworzenie obiektu niebieskiej pi³ki
    sf::Clock clock; // Zegar do kontroli czasu animacji

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close(); // Zamkniêcie okna gry
        }

        window.clear(sf::Color::Green); // Czyszczenie okna kolorem zielonym
        window.draw(blueBall.getBall()); // Rysowanie pi³ki
        window.display(); // Wyœwietlanie zawartoœci okna

        if (clock.getElapsedTime().asMilliseconds() > 5.0f) {
            blueBall.animate(); // Aktualizacja pozycji pi³ki
            clock.restart(); // Restart zegara
        }
    }

    return 0; // Zakoñczenie programu
}
