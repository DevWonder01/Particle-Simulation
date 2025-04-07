#include <iostream>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp> // Include SFML graphics library

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int NUM_PARTICLES = 100;
const float STEP_SIZE = -1.0f; // Average step size
const float VISCOSITY = 0.99f; // Dampening factor (optional)
struct Particle {
  sf::CircleShape shape;
  sf::Vector2f velocity;

  Particle(sf::Vector2f position, float radius, sf::Color color) : velocity(0.0f, 0.0f) {
      shape.setRadius(radius);
      shape.setFillColor(color);
      shape.setPosition(position);
      shape.setOrigin(radius, radius); // Set origin to center for easier rotation/scaling
  }

  void update(float deltaTime) {
      // Apply a random force (Brownian motion)
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_real_distribution<> distrib(-STEP_SIZE, STEP_SIZE);

      velocity.x += distrib(gen);
      velocity.y += distrib(gen);

      // Apply viscosity (optional)
      velocity *= VISCOSITY;

      shape.move(velocity * deltaTime);

      // Boundary conditions (simple wrapping)
      sf::Vector2f position = shape.getPosition();
      if (position.x < 0) shape.setPosition(WINDOW_WIDTH, position.y);
      if (position.x > WINDOW_WIDTH) shape.setPosition(0, position.y);
      if (position.y < 0) shape.setPosition(position.x, WINDOW_HEIGHT);
      if (position.y > WINDOW_HEIGHT) shape.setPosition(position.x, 0);
  }

  void draw(sf::RenderWindow& window) {
      window.draw(shape);
  }
};


int main() {
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Brownian Motion");
  window.setFramerateLimit(60);

  std::vector<Particle> particles;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> distrib_x(0, WINDOW_WIDTH);
  std::uniform_real_distribution<> distrib_y(0, WINDOW_HEIGHT);
  std::uniform_real_distribution<> distrib_radius(2, 5);
  std::uniform_int_distribution<> distrib_color(0, 255);

  // Initialize particles
  for (int i = 0; i < NUM_PARTICLES; ++i) {
      sf::Vector2f position(distrib_x(gen), distrib_y(gen));
      float radius = distrib_radius(gen);
      sf::Color color(distrib_color(gen), distrib_color(gen), distrib_color(gen));
      particles.emplace_back(position, radius, color);
  }

  sf::Clock clock;

  while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed) {
              window.close();
          }
      }

      float deltaTime = clock.restart().asSeconds();

      window.clear(sf::Color::Black);

      // Update and draw particles
      for (auto& particle : particles) {
          particle.update(deltaTime);
          particle.draw(window);
      }

      window.display();
  }

  return 0;
}