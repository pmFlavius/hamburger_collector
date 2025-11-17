#Hamburger Collector

A 2D physics-based game built in C++ with SFML where you control a bouncing ball to collect "burgers" and score points.

## 🎮 Features

- **Realistic physics** - gravity, bounce, friction
- **Double jump** - ability to jump twice in the air
- **Score system** - collect objects for +10 points each
- **Live timer** - implemented with multi-threading
- **Random spawn** - objects appear randomly on the map
- **Thread-safe** - uses mutex for synchronization
- **Smooth movement** - acceleration and deceleration mechanics
- **Wall bounce** - ball bounces off screen edges

## 🕹️ Controls

- ← / → - Move left/right
- Space - Jump (double jump available)

## 🛠️ Technologies

- **C++** - Core programming language
- **SFML** - Graphics, System, Window modules
- **Multi-threading** - sf::Thread for game timer
- **Mutex** - Thread synchronization for score and time

## 🎯 Gameplay

1. Control the bouncing ball using arrow keys
2. Jump with Space (you can double jump)
3. Collect randomly spawning objects
4. Each object gives you 10 points
5. Reach 100 points to win the game
6. Timer tracks how fast you can complete the challenge

## 🧵 Technical Highlights

### Multi-threading Implementation
- Separate thread for game timer using `Thread`
- Non-blocking timer that runs independently from game loop

### Thread Synchronization
- `Mutex` protects shared variables (`score`, `timp`)
- Prevents race conditions when accessing data from multiple threads

### Physics System
- Gravity and velocity-based movement
- Bounce coefficient for realistic ball physics
- Friction and acceleration for smooth controls
- Ground and wall collision detection

## 🔮 Future Improvements

- [ ] Add sound effects
- [ ] Multiple levels with increasing difficulty
- [ ] Power-ups and special abilities
- [ ] High score leaderboard
- [ ] Different collectible types with varying point values
- [ ] Obstacles and hazards
