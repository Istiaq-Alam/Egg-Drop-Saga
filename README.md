<div align="center">
<a href="https://github.com/Istiaq-Alam/Car-Rental-Website">
<img align="middle" width="224" height="224" alt="appicon" src="https://github.com/user-attachments/assets/1d5d2ac3-e00b-411b-94d9-24a7df0707cb" />
</a>
 


# 🐔🥚 Egg Drop Saga (C++ / OpenGL GLUT)
</div>

> A 2D Orthographic Arcade Game built using C++ and OpenGL (GLUT)  
> Developed as part of a Computer Graphics course project.

---

![Version](https://img.shields.io/badge/version-v0.1-blue)
![Language](https://img.shields.io/badge/language-C++-00599C)
![Graphics](https://img.shields.io/badge/OpenGL-GLUT-red)
![Build](https://img.shields.io/badge/build-passing-brightgreen)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)
![Status](https://img.shields.io/badge/status-Prototype-orange)
![Project](https://img.shields.io/badge/course-Computer%20Graphics-purple)

---

## 🎮 Game Overview

**Egg Drop Saga** is a 2D arcade-style game where chickens sitting on a wire drop eggs one at a time.  
The player controls a bucket and must catch the falling eggs before they hit the ground.

If an egg touches the ground, the player loses a life.  
The game ends when all 3 lives are lost.

This release focuses on **core gameplay logic and architecture**, not final graphics.

---

## 🚀 Features

### 🎯 Core Gameplay
- Sequential egg spawning (one egg at a time)
- Eggs drop from randomly selected chickens
- Player-controlled bucket (Left / Right movement)
- 3 Lives system
- Score tracking
- Gradually increasing difficulty
- Game Over condition when lives reach zero

---

### 🖥 Interface Screens (Not updated)
- 🏠 Home Screen
- 🎮 Playing Screen
- 💀 Game Over Screen
- Restart & Quit options

---

### 🧠 Technical Highlights
- 2D Orthographic Projection (`gluOrtho2D`)
- Event-driven input handling
- Real-time animation using `glutTimerFunc`
- AABB-style collision detection
- Finite State Machine (HOME / PLAYING / GAME_OVER)
- Object-Oriented Design (Modular Classes)

---

## 🏗 Project Structure
```
EggDropSaga/
│
├── main.cpp
├── Game.h / Game.cpp
├── Bucket.h / Bucket.cpp
├── Egg.h / Egg.cpp
├── Chicken.h / Chicken.cpp
```

---

## 🧩 Class Architecture

### `Game`
- Controls game state
- Manages score & lives
- Handles egg spawning logic
- Collision detection
- Screen rendering

### `Bucket`
- Player-controlled object
- Horizontal movement
- Collision detection interface

### `Egg`
- Falling object
- Speed scaling based on score

### `Chicken`
- Static object positioned on wire
- Source of egg spawning

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| `S` | Start Game |
| `A` | Move Bucket Left |
| `D` | Move Bucket Right |
| `R` | Restart (Game Over screen) |
| `Q` | Quit Game |

---

## 🛠 Build Instructions (Linux)

### 1️⃣ Install Dependencies

```bash
sudo apt install freeglut3-dev
```

### 2️⃣ Compile
```bash
g++ main.cpp Game.cpp Bucket.cpp Egg.cpp Chicken.cpp -o EggCatcher -lGL -lGLU -lglut
```

### 3️⃣ Run
```bash
./EggCatcher
```

---

## 📈 Difficulty Scaling

Egg falling speed increases as the score increases.
```
Base Speed = 2.0
Speed += score * 0.2
(Max capped to prevent extreme difficulty)
```
This creates progressive gameplay challenge.

---

## 🎨 Current Visual State

⚪ This version uses placeholder graphics:
- Bucket → Rectangle
- Egg → Circle
- Chicken → Basic primitive shape
- Background → Static sky color

⚪ Future versions will include:
- Animated clouds
- Improved egg shape
- Broken egg animation
- Enhanced UI
- Sound effects

⚪ 🧪 Known Limitations
- No background animation yet
- No textures
- No sound
- Basic collision approximation
- Single-egg system only

---

## 🔮 Roadmap
### v0.2
- Animated cloud background
- Improved egg design
- Better collision precision

### v0.3
- Sound effects
- UI polish
- Difficulty balancing

### v1.0
- Final graphics
- Full animation system
- Optimized performance

--- 

# 📜 License

This project is released for academic and educational purposes.

# ⭐ If You Like This Project

Consider giving it a ⭐ on GitHub!
