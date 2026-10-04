# 🟡 Pacman

A Pacman game written in C++ with [SFML](https://www.sfml-dev.org), built as a **Data Structures** course project. The maze is modeled as a graph, and each ghost uses a different graph algorithm to hunt the player.

<!-- Add a screenshot or GIF here, e.g. ![Gameplay](docs/gameplay.png) -->
<img width="800" alt="Gameplay" src="https://github.com/user-attachments/assets/badf378f-5e48-425c-97b8-d3bafbc9c0d9" />---

## 📌 Table of Contents

- [Features](#-features)
- [Data Structures & Algorithms](#-data-structures--algorithms)
- [Tech Stack](#-tech-stack)
- [Repository Structure](#-repository-structure)
- [Getting Started](#-getting-started)

---

## ✨ Features

- **Two maps**: Normal and Hard, each with its own maze layout.
- **Four ghosts** with distinct behaviors (Blinky, Pinky, Inky and Clyde).
- **Power-ups** placed on random nodes. Ghosts can become poisoned (for a limited time), be eaten, and return to their start.
- **Lives, score and high score** tracked per player profile.
- **Player profiles and settings** saved to `Assets/Settings.txt`: sound and music volume, plus rebindable movement keys.
- **Full menu system**: Play Game, Instructions, Change Profile, High Score, Options (graphics, sound, controls), Credits, and in-game pause and game-over screens.
- Animated sprites and sound effects.

---

## 🧠 Data Structures & Algorithms

| Concept | Where it's used |
| --- | --- |
| **Graph (adjacency list)** | Each maze is a graph of numbered nodes (`Map.h`). Edges are stored in `unordered_map<int, vector<int>>`, and each node has an on-screen position. |
| **BFS** | `Ghost::precomputeAllPaths` runs BFS from every node to store the shortest path between every pair of nodes when a map loads. |
| **DFS** | Pinky looks a few nodes ahead of Pacman's direction of travel. |
| **A\*** | On the Hard map, a ghost uses an A\*-based chase, with BFS path lengths as the heuristic (`Ghost::HardMove`). |
| **Hash maps** | Player settings, precomputed paths and adjacency lists. |

---

## 🛠 Tech Stack

- **Language**: C++
- **Library**: [SFML](https://www.sfml-dev.org) 2.x (graphics, audio, input)
- **IDE**: Visual Studio (`pacman.sln`)

---

## 📂 Repository Structure

```text
├── include/SFML/        # SFML headers
├── lib/                 # SFML libraries
├── pacman/
│   ├── main.cpp         # Game loop
│   ├── Map.*            # Maze graph: nodes, edges, positions
│   ├── Player.*         # Pacman movement, input and animation
│   ├── Ghost.*          # Ghost AI and pathfinding
│   ├── menu.*           # Menus, options and profile screens
│   ├── FilesController.*# Settings, profiles and high-score file I/O
│   ├── Sounds.*         # Sound effects and music
│   └── Assets/          # Textures, sounds, fonts and Settings.txt
└── pacman.sln           # Visual Studio solution
```

---

## 🚀 Getting Started

1. Clone the repository:

   ```bash
   git clone https://github.com/BeboNasif/Pacman.git
   ```

2. Open `pacman.sln` in Visual Studio.
3. Build and run (**F5**).

The SFML headers, libraries and DLLs are included, so no extra setup is needed. If the game can't find its assets, make sure the working directory is the `pacman/` folder.
