# Pokémon Battle Simulation

This project is a simulation of a Pokémon battle between Pikachu and Blastoise, developed for the **BLG 223E - Data Structures** course at ITU. The simulation models a turn-based Pokémon game using a custom doubly linked list and graph data structures to manage game states.

## Overview

The project simulates a simplified battle where each Pokémon has:
- **HP (Health Points)**: The Pokémon's health, reduced when attacked.
- **PP (Power Points)**: The energy used to perform attacks. If PP is insufficient, the attack cannot be executed.

### Game Mechanics
- Pikachu and Blastoise start with 200 HP and 100 PP each.
- Attacks have attributes such as damage, accuracy, PP cost, and the earliest level they can be used.
- The battle progresses turn-by-turn, and the game's state is managed using a graph structure, where each node represents a unique game state.

## Features
1. **Attack Classes**: Defines each Pokémon's attack with attributes like name, PP cost, accuracy, damage, and first usage level.
2. **Pokémon Classes**: Models Pikachu and Blastoise with methods to manage their HP and PP.
3. **Graph Structure**: Implements the game using a graph where nodes represent possible game states. The graph is expanded level-wise using Breadth-First Search (BFS).
4. **Winning Path Calculation**: Uses BFS to find the shortest path to a winning state, displaying all actions and their effects on HP and PP.

## Implementation Details
- The project uses a custom **DoublyList** data structure to handle nodes efficiently in a BFS traversal.
- **expandGraph Function**: Generates all possible game states from a current state based on attack probabilities and adds them to the graph.
- **findShortestWinningPath Function**: Identifies the shortest sequence of moves leading to a win, ensuring the most efficient gameplay.

## How to Run
Compile the code using `g++`:
```bash
g++ main.cpp -o pokemon_battle
```
Run the simulation:
- **Part 1**: To expand the graph to a specific depth and print nodes:
  ```bash
  ./pokemon_battle part1 [Depth]
  ```
- **Part 2**: To find the shortest winning path for a participant (Pikachu or Blastoise):
  ```bash
  ./pokemon_battle part2 [participant]
  ```

## Output Example
- Detailed output showing the Pokémon's HP and PP changes after each attack.
- Winning paths with probabilities for both Pikachu and Blastoise.

## Key Concepts
- **Data Structures**: Doubly linked list and graph traversal.
- **Algorithm**: Breadth-First Search for level-wise graph exploration.
- **Probability Calculation**: Simulates attack outcomes based on attack accuracy.

## Additional Documentation
For a more detailed explanation of the implementation and analysis, please refer to my report included in the repository.

---
