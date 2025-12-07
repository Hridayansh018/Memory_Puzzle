---

# Memory Puzzle Game in C++

A clean, object-oriented implementation of the classic **Memory Matching (Concentration)** puzzle game in C++.
Players flip two cards at a time, trying to find matching pairs until the full board is cleared.

---

## Table of Contents

- [Table of Contents](#table-of-contents)
- [Overview](#overview)
- [Folder Structure](#folder-structure)
- [Key Components](#key-components)
  - [Card Class](#card-class)
    - [**Class: `Card`**](#class-card)
  - [Board Class](#board-class)
    - [**Class: `Board`**](#class-board)
  - [Game Class](#game-class)
    - [**Class: `Game`**](#class-game)
- [Game Logic Flow](#game-logic-flow)
- [Controls](#controls)
- [How to Build \& Run](#how-to-build--run)
  - [Using g++ (MinGW, MSYS2, Linux, macOS):](#using-g-mingw-msys2-linux-macos)
  - [Using MSVC (Developer Command Prompt):](#using-msvc-developer-command-prompt)
- [Extending the Game](#extending-the-game)
- [Code Quality Notes](#code-quality-notes)

---

## Overview

The Memory Puzzle Game is written fully in standard C++ using a simple and robust **Object-Oriented design**.
The program runs entirely in the console and is compatible with any C++17 compiler.

Features include:

* Dynamic board size (4×4 by default)
* Fully shuffled random card layout
* Card revealing logic
* Match detection
* Move tracking
* Clean, readable console layout
* Strong input validation
* Zero external dependencies
* Avoids `chrono`, `thread`, and other complex headers for maximum compatibility

This makes it ideal for beginners learning OOP, or for classroom demonstration.

---

## Folder Structure

```
memory-puzzle/
├── main.cpp
├── README.md
```

Since the implementation is self-contained, all logic stays within a single file.

---

## Key Components

### Card Class

Represents a single card on the board.

#### **Class: `Card`**

**Members:**

* `char value` — The letter/symbol on the card
* `bool revealed` — True if temporarily shown
* `bool matched` — True if permanently matched

**Methods:**

* `Card(char v = ' ')` — Constructor
* `char getValue() const` — Returns card symbol
* `bool isRevealed() const`
* `bool isMatched() const`
* `void reveal()` — Shows the card temporarily
* `void hide()` — Hides if not matched
* `void setMatched()` — Locks the card as found

**Purpose:**
Encapsulates all card state and guarantees correct reveal/match behavior.

---

### Board Class

Controls the grid of cards — creation, shuffling, drawing, and position access.

#### **Class: `Board`**

**Members:**

* `int rows, cols` — Board dimensions
* `vector<Card> cards` — All cards in row-major order

**Key Methods:**

* `Board(int r, int c)` — Initializes the board with pairs
* `Card& at(int r, int c)` — Access card by coordinate
* `void revealAt(int r, int c)`
* `void hideAt(int r, int c)`
* `void matchAt(int r, int c)`
* `bool allMatched() const`
* `void display(bool showCoords = true) const` — Renders the board to console

**Logic Highlights:**

* Generates pairs using a character pool (A–Z, a–z, digits, symbols)
* Randomly shuffles the deck using `mt19937`
* Displays hidden cards as `*`, revealed cards as their symbol

---

### Game Class

The manager of the entire gameplay flow.

#### **Class: `Game`**

**Members:**

* `Board board` — Active board
* `int moves` — Counts number of attempts

**Key Methods:**

* `bool valid(int r, int c) const` — Bounds checking
* `pair<int,int> readPos(const string& prompt)` — Safe input parsing
* `void play()`
  Main game loop:

  * Draw board
  * Read two positions
  * Reveal cards
  * Check match
  * Hide again if needed
  * Track move count

**Purpose:**
Coordinates user interaction, game rules, move counting, and win condition.

---

## Game Logic Flow

1. **Initialization**

   * User chooses board size (must be even number of cells)
   * Deck is generated and shuffled
2. **Main Loop**

   * Display board
   * Player selects two positions
   * If same or invalid → prompt again
   * Reveal first card, then second
   * If values match → mark both as matched
   * Else → prompt ENTER and hide both
   * Increment move counter
3. **End Condition**

   * When all cards are matched → show final board and stats

---

## Controls

* **Input Style:**
  `row column` (e.g., `2 3`)

* **Reveal card:** type coordinates

* **Confirm hide on mismatch:** press **Enter**

No arrow keys or special input required.

---

## How to Build & Run

### Using g++ (MinGW, MSYS2, Linux, macOS):

```sh
g++ -std=c++17 -O2 main.cpp -o memory_game
./memory_game
```

### Using MSVC (Developer Command Prompt):

```sh
cl /std:c++17 /EHsc main.cpp
main.exe
```

No external libraries are required.

---

## Extending the Game

This project is ideal for learning. You can easily add:

* Timer system
* Scoreboard
* Difficulty modes
* Card themes (emoji sets)
* Hint system
* Better UI with ncurses / WinAPI
* Multiplayer (alternating turns)
* Save/load system

Because OOP encapsulation is clean, each enhancement fits naturally.

---

## Code Quality Notes

* **Encapsulated classes:**
  Card, Board, and Game each handle their own responsibilities cleanly.

* **Strong input validation:**
  Prevents invalid card choices and out-of-bounds errors.

* **No advanced headers:**
  Ensures maximum portability (no chrono, no thread).

* **Clear control flow:**
  Easy to read and modify — suitable for assignments and teaching.

---
