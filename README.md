# 2D Physics Simulation with Spatial Partitioning

A C++ implementation of a 2D physics engine using SFML for rendering and spatial grid partitioning for efficient collision detection.

## Features

- Real-time 2D physics simulation
- Spatial grid-based collision detection (O(n) complexity)
- Elastic and inelastic collisions
- Gravity and viscosity effects
- Particle-based rigid body dynamics
- Boundary collision handling
- Energy conservation and dissipation

## Physics Concepts

- Newton's Laws of Motion
- Conservation of Momentum
- Elastic Collisions
- Impulse-Based Physics
- Spatial Partitioning
- Collision Detection and Resolution
- Energy Dissipation

## Dependencies

- SFML 2.5+
- C++17 or higher
- CMake 3.10+ (optional)

## Installation

1. Install SFML:
```bash
sudo apt-get install libsfml-dev
```

2. Clone the repository:
```bash
git clone https://github.com/yourusername/physics-simulation.git
cd physics-simulation
```

3. Compile the code:
```bash
g++ -o spatial spatial.cpp -lsfml-graphics -lsfml-window -lsfml-system
```

## Usage

Run the simulation:
```bash
./spatial
```

## Controls

- Close window: Click the X button
- Physics parameters can be adjusted in the source code:
  - `NUM_OF_BODIES`: Number of particles
  - `GRAVITY`: Gravity constant
  - `VISCOSITY`: Damping factor
  - `CELL_SIZE`: Size of grid cells for collision detection

## Implementation Details

### Spatial Grid System
- Divides the window into a grid of cells
- Each cell tracks particles within its bounds
- Reduces collision checks to nearby particles only

### Collision Detection
- Circle-circle intersection tests
- Grid-based broad phase
- Precise narrow phase checking

### Physics Resolution
- Impulse-based collision response
- Energy conservation with optional damping
- Position correction to prevent sinking

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a new Pull Request