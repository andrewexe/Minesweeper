# 🔹 Minesweeper (Project 3)

**University of Florida • COP3503C: Programming Fundamentals II**

---

## 🌐 Overview

This project is a full-featured implementation of the classic **Minesweeper** game using **C++** and the **SFML (Simple and Fast Multimedia Library)**. Players must uncover all tiles that are not mines, using logical deduction based on adjacent mine counts. The game consists of multiple interactive windows: Welcome, Game, and Leaderboard.

---

## 🎮 Game Objective

Uncover all non-mine tiles while avoiding mines. When a tile is clicked:

* If it contains a **mine**, the game ends (loss).
* If not, it reveals a number (0–8) indicating adjacent mines.
* A `0` tile reveals adjacent non-mine tiles recursively.
* Flagging suspected mines and tracking remaining count are key gameplay features.
* Victory is achieved when all non-mine tiles are revealed.

---

## 📊 Project Highlights

### 👤 Welcome Window

* Prompts player to enter a name (letters only, max 10 chars).
* Enforces formatting (e.g., "aLeX" becomes "Alex").
* Shows typing indicator ("|") and supports backspace.
* Closes only when valid name is entered.

### 🎮 Game Window

* Interactive tile-based board generated via `board_config.cfg`.
* Supports left-click reveal and right-click flag placement.
* Includes debug mode to show all mines for testing.
* Pause/play toggle freezes gameplay and timer.
* Tracks timer and mine count in real-time.
* Uses face icons for game state: Happy, Win, Lose.

### 🔍 Leaderboard Window

* Displays top 5 player times from `leaderboard.txt`.
* Updates leaderboard when a player wins with a top score.
* Displays current player's time with an asterisk if it qualifies.

---

## 📂 File Structure

```
project3/
├── files/
│   ├── board_config.cfg
│   ├── font.tff
│   ├── leaderboard.txt
│   └── images/
│       ├── mine.png
│       ├── tile_hidden.png
│       ├── tile_revealed.png
│       ├── number_1.png ... number_8.png
│       ├── face_happy.png / face_win.png / face_lose.png
│       ├── debug.png / pause.png / play.png / leaderboard.png
│       └── digits.png
├── src/
│   ├── main.cpp
│   ├── Tile.cpp / Tile.h
│   ├── Board.cpp / Board.h
│   ├── TextureManager.cpp / TextureManager.h
│   └── WindowManager.cpp / WindowManager.h
├── Makefile
└── README.md
```

---

## ⚙️ Features Checklist

* [x] Custom board dimensions from `board_config.cfg`
* [x] Recursive reveal for 0-adjacent-mine tiles
* [x] Victory & defeat detection
* [x] Counter + timer synced with gameplay
* [x] Dynamic leaderboard update and display
* [x] Pause/play/debug functionality
* [x] Clean resource management using TextureManager

---

## 💻 Technical Concepts

* **Pointers & Recursion** (tile adjacency and flood fill)
* **Event-Driven Programming** (mouse input handling via SFML)
* **File I/O** (leaderboard and config file parsing)
* **Vector & Maps** (tile storage, texture management)
* **SFML Graphics** (window management, sprites, fonts)

---

## 🔧 Compilation

### Via Makefile:

```bash
make
./sfml-app
```

> Ensure SFML is installed and linked correctly.
> Example paths provided in Makefile for macOS and Windows.

---

## 🌐 Configurable Options

Edit `files/board_config.cfg` to change board setup:

```
25   # number of columns
16   # number of rows
50   # number of mines
```

---

## 🏋️ Sample Leaderboard.txt

```
07:01,Alex
09:04,Bruce
10:03,Carl
11:24,Daniel
12:15,Eve
```

---

## 🔖 Notes

* Relative file paths are used (e.g., `files/images/mine.png`).
* Global sf::Texture and sf::Font objects are **avoided**.
* Leaderboard supports persistence and insertion sort logic.

---

## 👤 Student Info (for README submission)

```
Name: Andrew Huang
Section: [Your section here]
UFL email: [Your UFL email here]
System: macOS / Windows / Linux
Compiler: g++ 11.4 / Clang
SFML version: 2.5.1
IDE: CLion / VSCode / Terminal
Other notes: None
```

---

## 📄 License

MIT License © 2023
