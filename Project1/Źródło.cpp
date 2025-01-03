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

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML Blue Ball Animation"); // Utworzenie okna gry
    sf::Event event; // Obiekt do obs�ugi zdarze�

    BlueBall blueBall(400, 10, 500, 500); // Utworzenie obiektu niebieskiej pi�ki
    sf::Clock clock; // Zegar do kontroli czasu animacji

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close(); // Zamkni�cie okna gry
        }

        window.clear(sf::Color::Green); // Czyszczenie okna kolorem zielonym
        window.draw(blueBall.getBall()); // Rysowanie pi�ki
        window.display(); // Wy�wietlanie zawarto�ci okna

        if (clock.getElapsedTime().asMilliseconds() > 5.0f) {
            blueBall.animate(); // Aktualizacja pozycji pi�ki
            clock.restart(); // Restart zegara
        }
    }

    return 0; // Zako�czenie programu
}
