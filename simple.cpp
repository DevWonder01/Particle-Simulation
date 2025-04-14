#include <iostream>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float VISCOSITY = 0.99f; // Dampening factor (optional)
const int NUM_OF_BODIES = 5;
const float STEP_SIZE = -1.0f; // Average step size

struct RigidBody
{
    sf::CircleShape sphere;
    sf::Vector2f velocity;
    sf::Vector2f mass;

    RigidBody(sf::Vector2f position, float radius, sf::Color color, sf::Vector2f _mass) : velocity(0.0f, 0.0f)
    {
        sphere.setRadius(radius);
        sphere.setFillColor(color);
        sphere.setPosition(position);
        sphere.setOrigin(radius, radius); // Set origin to center for easier rotation/scaling
        mass = _mass;
    }

    sf::Vector2f force(sf::Vector2f _mass, sf::Vector2f acceleration)
    {
        return sf::Vector2f(_mass.x * acceleration.x, _mass.y * acceleration.y);
    }

    void update(float deltaTime)
    {
        // Apply gravity
        const float GRAVITY = 2.0f; // Gravity constant
        velocity.y += GRAVITY * deltaTime;

        // Apply viscosity (optional)
        // velocity *= VISCOSITY;

        sf::Vector2f appliedForce = force(mass, sf::Vector2f(0, GRAVITY));
        sphere.move(appliedForce * deltaTime);

        // Boundary conditions (simple wrapping)
        sf::Vector2f position = sphere.getPosition();

        // Check for collision with the floor
        if (position.y + sphere.getRadius() >= WINDOW_HEIGHT)
        {
            sphere.setPosition(position.x, WINDOW_HEIGHT - sphere.getRadius()); // Set position to the floor
            velocity.y = -velocity.y * 0.8f;                                    // Reverse velocity and reduce it to simulate energy loss
        }

        // Check for collision with the walls
        if (position.x - sphere.getRadius() <= 0)
        {
            sphere.setPosition(sphere.getRadius(), position.y);
            velocity.x = -velocity.x * 0.8f; // Reverse velocity and reduce it to simulate energy loss
        }
        if (position.x + sphere.getRadius() >= WINDOW_WIDTH)
        {
            sphere.setPosition(WINDOW_WIDTH - sphere.getRadius(), position.y);
            velocity.x = -velocity.x * 0.8f; // Reverse velocity and reduce it to simulate energy loss
        }

        // Print position to the console
        std::cout << "Position: X = " << position.x << ", Y = " << position.y << std::endl;
    }

    void draw(sf::RenderWindow &window) const
    {
        window.draw(sphere);
    }
};

bool checkCollision(
    RigidBody &body1,
    RigidBody &body2)
{
    float dx = body1.sphere.getPosition().x - body2.sphere.getPosition().x;
    float dy = body1.sphere.getPosition().y - body2.sphere.getPosition().y;

    float distance = dx * dx * dy * dy;

    float radiusTotal = body1.sphere.getRadius() + body2.sphere.getRadius();

    return distance <= radiusTotal * radiusTotal * radiusTotal;
};

void resolveParticleCollision(
    RigidBody &body1,
    RigidBody &body2)
{
    float dx = body1.sphere.getPosition().x - body2.sphere.getPosition().x;
    float dy = body1.sphere.getPosition().y - body2.sphere.getPosition().y;

    float distance = dx * dx * dy * dy;

    if (distance == 0.0)
        return;

    float normalX = dx / std::sqrt(distance);
    float normaly = dy / std::sqrt(distance);

    // relative velocity
    float relVx = body1.velocity.x - body2.velocity.y;
    float relVy = body1.velocity.x - body2.velocity.y;

    float velAlongNormal = relVx * normalX + relVy * normaly;

    if (velAlongNormal > 0)
        return;

    float restituion = 1.;

    float j = -(1 + restituion) * velAlongNormal;

    float impulseX = j * normalX;
    float impluseY = j * normaly;

    body1.velocity.x += impulseX / body1.mass.x;
    body1.velocity.y += impluseY / body1.mass.y;
    body2.velocity.x -= impulseX / body2.mass.x;
    body2.velocity.y -= impluseY / body2.mass.y;

    float penetration = (body1.sphere.getRadius() + body2.sphere.getRadius()) - std::sqrt(distance);

    float k = 3.0f; // Penetration constant

    float inverseMass1 = 1.0f / (body1.mass.x + body1.mass.y);
    float inverseMass2 = 1.0f / (body2.mass.x + body2.mass.y);

    float particleXcorrection = normalX * penetration * k / (inverseMass1 + inverseMass2);

    float particleYcorrection = normalX * penetration * k / (inverseMass1 + inverseMass2);

    body1.sphere.move(particleXcorrection * inverseMass1, particleYcorrection * inverseMass1);
    body2.sphere.move(particleXcorrection * inverseMass1, particleYcorrection * inverseMass1);
}

void handleCollision(
    std::vector<RigidBody>& bodies)
{
    for (int i = 0; i < NUM_OF_BODIES; ++i)
    {
        for (int j = 0; j < NUM_OF_BODIES; ++j)
        {
            if (checkCollision(bodies[i], bodies[j]))
            {
                resolveParticleCollision(bodies[i], bodies[2]);
            }
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "RigidBody Rendering");

    std::vector<RigidBody> bodies;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> distrib_x(0, WINDOW_WIDTH);
    std::uniform_real_distribution<> distrib_y(0, WINDOW_HEIGHT);
    std::uniform_real_distribution<> distrib_radius(20, 50);
    std::uniform_real_distribution<> distrib_mass(1, 10);
    std::uniform_int_distribution<> distrib_color(0, 255);

    // Initialize rigid bodies
    for (int i = 0; i < NUM_OF_BODIES; ++i)
    {
        sf::Vector2f position(distrib_x(gen), distrib_y(gen));
        float radius = distrib_radius(gen);
        sf::Color color(distrib_color(gen), distrib_color(gen), distrib_color(gen));
        sf::Vector2f mass(distrib_mass(gen), distrib_mass(gen));

        bodies.emplace_back(position, radius, color, mass);
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
        for (auto &body : bodies)
        {
            body.update(deltaTime);
        }

        // handleCollision(bodies);


        for (auto &body : bodies)
        {
            body.draw(window);
        }

        window.display();
    }

    return 0;
}