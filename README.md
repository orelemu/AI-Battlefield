# Tactical Battle Simulation (C++)

## Project Overview

A tactical battle simulation in a closed arena, developed in C++. The simulation features two rival teams competing for control using dynamic movement, strategic combat, and real-time resource management.

## Team Structure

Each team consists of:

- **2 Soldiers**: Their goal is to neutralize the opposing team. They can:
  - Advance towards enemies
  - Hide behind cover
  - Shoot with firearms
  - Throw grenades
  - Call for support when low on health or ammo

- **1 Support Unit (Logistics Carrier)**: Monitors the soldiers in real-time. When a team member is in trouble (low health or ammunition), the support unit navigates to retrieve and deliver the appropriate supply crate from elsewhere in the arena.

## Algorithms and AI

Several algorithms were implemented to support navigation, decision-making, and dynamic scenario handling, including:

- A* (A-Star)
- BFS (Breadth-First Search)
- DFS (Depth-First Search)
- Custom heuristics tailored to combat strategy and arena awareness

## Arena Elements

- **Purple nodes**: Ammo crates (contain 1 grenade + 5 bullets)
- **Green nodes**: Medical kits (restore HP)
- **Small red nodes**: Bullets
- **Large red nodes**: Grenades (explode and scatter bullets in all directions)

## Visuals

- The **light blue team** faces off against the **yellow team**
- The **darker-shaded soldiers** on each team represent the **support units**

## Technologies

- C++ for simulation logic and architecture
- Object-Oriented Design
- Pathfinding and decision-making algorithms
- Custom state management for combat units and arena events
