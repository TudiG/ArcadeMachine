# Chicken Invaders – Grid-Based Shooter

## Description
Chicken Invaders is a 2D arcade-style shooter with a custom ship-building mechanic.  
Players design their ship in a garage using blocks (engine, cannon, bumper, solid), then face waves of enemy chickens.  

## Features:
- Drag-and-drop ship construction in a garage.
- Grid-based ship validation ensuring playable designs.
- Enemy waves with sinusoidal movement and randomized shooting.
- Upgradeable ship speed and projectile speed.
- Particle effects for destroyed enemies.
- Game-over screen with score tracking.

## Requirements
- C++17 or higher
- Visual Studio 2022 (or later)
- CMake 3.16+
- OpenGL 3.3+ compatible GPU

## Build Instructions

1. **Clone the repository:**

git clone <your-repo-url>
cd ChickenInvaders

2. Create a build folder:

mkdir build

cd build

3. Generate Visual Studio project files with CMake:

cmake ..

4. Build the project:

Open the generated .sln file in Visual Studio.

## Running the Game

After building, run the game using the framework executable:

GFXFramework.exe

## Gameplay
1. Garage Mode

Drag blocks from the left panel onto the grid to build your ship.

Types of blocks:

Engine: Required for propulsion.

Cannon: Fires projectiles at enemies.

Bumper: Defensive component.

Solid: Structural block.

### Ship validation rules:

Must have at least 1 and at most 10 blocks.

All blocks must be connected.

Cannon cannot have blocks above it; Engine cannot have blocks below it.

Bumpers cannot overlap improperly.

### Click Start when the ship is valid to enter the game.

2. Game Mode

Control the ship with WASD keys.

Press SPACE to fire projectiles.

Enemy chickens move sinusoidally and fire projectiles.

Survive waves and earn points for destroying enemies.

Upgrade ship speed (key 1) and projectile speed (key 2) using earned points.

Game ends if enemy projectiles hit your ship or enemies reach the bottom.

## HUD

Displays Score, Enemies Remaining, Ship Speed Level, and Projectile Speed Level.

## Controls

W/A/S/D: Move ship

SPACE: Shoot projectiles / restart round

Mouse Left Click: Drag and drop blocks in garage

Mouse Right Click: Remove blocks from grid in garage

Mouse: Aim/drag blocks

Number Keys (1/2): Upgrade ship/projectile speed

## Notes

The game uses a 2D orthographic projection with a consistent logic-to-screen mapping.

The project separates garage setup from gameplay to simplify ship validation and wave management.

## License

This project is licensed under the MIT License. See LICENSE.md for details.
