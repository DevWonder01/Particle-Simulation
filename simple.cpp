#include <iostream>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float VISCOSITY = 0.99f; // Dampening factor (optional)
const int NUM_OF_BODIES = 500;
const float STEP_SIZE = -1.0f; // Average step size

struct RigidBody
{
    sf::RectangleShape rectangle;
    sf::Vector2f velocity;
    sf::Vector2f mass;

    RigidBody(sf::Vector2f position, sf::Vector2f size, sf::Color color, sf::Vector2f _mass) : velocity(0.0f, 0.0f)
    {
        rectangle.setSize(size);
        rectangle.setFillColor(color);
        rectangle.setPosition(position);
        rectangle.setOrigin(size.x / 2, size.y / 2); // Set origin to center for easier rotation/scaling
        mass = _mass;
    }

    sf::Vector2f force(sf::Vector2f _mass, sf::Vector2f acceleration)
    {
        return sf::Vector2f(_mass.x * acceleration.x, _mass.y * acceleration.y);
    }


    void update(float deltaTime) {
        // Apply gravity
        const float GRAVITY = 2.0f; // Gravity constant
        velocity.y += GRAVITY * deltaTime;

    
        // Calculate force and move the rectangle
        sf::Vector2f appliedForce = force(mass, sf::Vector2f(0, GRAVITY));
        rectangle.move(appliedForce * deltaTime);
    
        // Boundary conditions (simple wrapping)
        sf::Vector2f position = rectangle.getPosition();
    
        // Check for collision with the floor
        if (position.y + rectangle.getSize().y / 2 >= WINDOW_HEIGHT) {
            rectangle.setPosition(position.x, WINDOW_HEIGHT - rectangle.getSize().y / 2); // Set position to the floor
            velocity.y = -velocity.y * 0.8f; // Reverse velocity and reduce it to simulate energy loss
        }
    
        if (position.x < rectangle.getSize().x / 2 >= WINDOW_WIDTH) {
            rectangle.setPosition(WINDOW_WIDTH - rectangle.getSize().x / 2, position.y);
            velocity.x = -velocity.x * 0.8f; // Reverse velocity and reduce it to simulate energy loss
        }

        // Print position to the console
        std::cout << "Position: X = " << position.x << ", Y = " << position.y << std::endl;
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(rectangle);
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "RigidBody Rendering");


    std::vector<RigidBody> bodies;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> distrib(-STEP_SIZE, STEP_SIZE);
    std::uniform_int_distribution<> distrib_color(0, 255);
    std::uniform_int_distribution<> distrib_x(0, WINDOW_WIDTH);
    std::uniform_int_distribution<> distrib_y(0, WINDOW_HEIGHT);
    std::uniform_real_distribution<> distrib_radius(20, 50);
    std::uniform_real_distribution<> distrib_mass(1, 100);
    std::uniform_real_distribution<> distrib_velocity(-1, 1);


    for (int x =0; x < NUM_OF_BODIES; ++x){
        sf::Vector2f positon(distrib_x(gen), distrib_y(gen));  
        float raduis = distrib_radius(gen);
        sf::Color color(distrib_color(gen), distrib_color(gen), distrib_color(gen));
        sf::Vector2f mass(distrib_mass(gen), distrib_mass(gen)); 
        // sf::Vector2f velocity(distrib_velocity(gen), distrib_velocity(gen));
        
        sf::Vector2f size(raduis, raduis);
        bodies.emplace_back(positon,size,color,mass);

    }


    sf::Clock clock;

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


        float deltaTime = clock.restart().asSeconds();
        // Rendering logic
        window.clear(sf::Color::Black);
        for (auto& body:bodies){
            body.update(deltaTime);
            body.draw(window); // Render the rigid body

        }
        window.display();
    }

    return 0;
}