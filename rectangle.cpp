#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float NUM_OF_PARTICLES = 100; // Fixed typo in the constant name
const float STEP_SIZE = -1.0f; // Average step size
const float VISCOSITY = 0.99f; // Dampening factor (optional)

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Brownian Motion");

    sf::RectangleShape rectangle(sf::Vector2f(100, 100));

    rectangle.setFillColor(sf::Color::Red);
    rectangle.setOutlineColor(sf::Color::Green);
    rectangle.setOutlineThickness(5);

    // Set origin to the center of the rectangle
    rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);

    // Set position to the center of the window
    rectangle.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    rectangle.setRotation(0); // Rotate the rectangle by 0 degrees
    rectangle.setScale(2, 2); // Scale the rectangle by a factor of 2 in both x and y directions

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Rendering logic moved inside the main loop
        window.clear(sf::Color::Black);
        window.draw(rectangle);
        window.display();
    }

    return 0;
}