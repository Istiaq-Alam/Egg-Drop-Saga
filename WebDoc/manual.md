# Egg-Drop-Saga User Manual

## Overview

**Egg-Drop-Saga** is a 2D arcade-style computer graphics game developed using OpenGL and GLUT. The objective is simple: catch falling eggs using a bucket while avoiding misses. As the game progresses, the speed and difficulty increase, challenging the player's reflexes and coordination.

---

# System Requirements

## Windows

* Windows 10 or later
* OpenGL-compatible graphics driver
* GCC Compiler (MinGW)
* GLUT Library

## Recommended

* 4 GB RAM or higher
* 1366×768 screen resolution or above

---

# Installation

## Option 1: Run Prebuilt Executable

1. Download the latest release.
2. Extract the ZIP archive.
3. Run:

```text
Egg-Drop-Saga.exe
```

No additional installation is required if all dependencies are included.

---

## Option 2: Build from Source

### Required Libraries

* OpenGL
* GLUT / FreeGLUT

### Build Steps

1. Open the project in Code::Blocks.
2. Ensure GLUT libraries are correctly linked.
3. Build and Run:

```text
Build → Build and Run
```

---

# Game Objective

The player's goal is to:

* Catch falling eggs using the bucket.
* Earn points for each successfully caught egg.
* Avoid letting eggs hit the ground.
* Achieve the highest score possible.

---

# Controls
-------------------------------------
| Key           | Action            |
| ------------- | ----------------- |
| ← Left Arrow  | Move bucket left  |
| → Right Arrow | Move bucket right |
| P             | Pause game        |
| R             | Restart game      |
| ESC           | Exit game         |
| ENTER         | Start game        |
-------------------------------------


# Main Menu

When the game starts, the Main Menu appears.

Available options:

### Start Game

Begins a new game session.

### Instructions

Displays game controls and gameplay information.

### Credits

Shows developer and project information.

### Exit

Closes the game.

---

# Gameplay

## Catching Eggs

* Eggs fall from the chicken above.
* Move the bucket underneath the falling egg.
* Catching an egg increases your score.

## Missing Eggs

* If an egg reaches the ground without being caught:

  * A life may be lost (depending on game mode).
  * The game becomes more difficult.

## Increasing Difficulty

As your score increases:

* Egg falling speed increases.
* Reaction time decreases.
* Gameplay becomes more challenging.

---

# Pause Function

Press:

```text
P
```

to pause the game.

Press:

```text
P
```

again to resume.

---

# Restarting

To start over at any time:

```text
R
```

The score and game state will reset.

---

# Game Over

The game ends when:

* The player loses all available chances, or
* The game's failure condition is reached.

After Game Over:

* Final score is displayed.
* Player may restart the game.

---

# Scoring System

| Action    | Points   |
| --------- | -------- |
| Catch Egg | +1 Score |
| Miss Egg  | No Score |

The objective is to achieve the highest score possible.

---

# Graphics Features

The game demonstrates several computer graphics concepts:

* 2D Transformations
* Primitive Drawing
* Animation
* Object Rendering
* Collision Detection
* Interactive User Input
* Real-Time Rendering

---

# Troubleshooting

## Black Screen

Ensure OpenGL drivers are properly installed.

## Game Does Not Launch

Verify:

* GLUT libraries are available.
* Required DLL files are present.

## Low Performance

Close background applications and update graphics drivers.

---

# Credits



## Developer



Istiak Alam - ID:05

Tanveer Ratul - ID:04



## Project-Link



[Egg-Drop-Saga](https://github.com/Istiaq-Alam/Egg-Drop-Saga.git)



[Website-Link](https://egg-drop-saga.vercel.app/)



---



# Version Information



Current Version: 0.5



Release Type: Public Release


Enjoy playing Egg-Drop-Saga and try to achieve the highest score possible!
