# Advanced Naval Battle Simulator

## 📌 Project Overview
The **Advanced Naval Battle Simulator** is a C-based program developed to simulate progressively challenging naval warfare scenarios. The simulator is designed for training purposes, demonstrating the interaction between battleships and escort ships using projectile motion, impact calculation, and user strategy.






## ✨ Features
- Interactive menu with multiple simulation options
- Three distinct simulation levels with increasing complexity
- Real-time projectile calculations using physics equations
- Dynamic memory allocation for flexible ship and canvas creation
- Input validation and error handling throughout
- Persistent statistics and battlefield state saved to files

---

## 📋 Menu Options
Upon starting the program, users can:
- 🔫 **Start Simulation**  
- 📘 **View Instructions**: Overview of rules and simulation logic  
- 📊 **View Statistics**: Load previous simulation data from saved files  
- ❌ **Exit Program**

The program uses a `switch` statement to manage menu selections.

---

## 🎮 Simulation Levels

### 1. **Basic Training (Part 1-A)**
- Static battlefield setup
- User selects a battleship and coordinates
- Escort ships are randomly positioned
- Basic projectile calculation

### 2. **Path of Brave (Part 1-B)**
- Battleship moves through user-defined coordinates
- Angle limitation is introduced in Simulation 2 (0°–60° firing angle)

### 3. **Advanced Warfare (Part 1-C)**
- Cumulative impact logic introduced
- An escort ship is only sunk when cumulative impact ≥ 1
- Option to simulate with or without movement


## 🧠 Implementation Details

### 🔹 Initial Conditions
- Battleship and escort ship properties stored using `Btype` and `Etype` structs.
- Arrays are dynamically allocated and then assigned to structs.
- **Escort ships:**
  - Randomly generated `minVelocity` and `angle`.
  - Angles are converted to radians:  
    ```c
    E->minAngle = ((rand() / (double)RAND_MAX) * (90 - range)) * PI / 180.0;
    ```
  - **Max angle** is calculated using the range.
  - Projectile motion equation used for calculating attack range:  
    ```c
    range = (velocity² × sin(2 × angle)) / GRAVITY;
    ```
- **Battleships:**
  - Velocity is user input.
  - Angle is randomly generated between 0–90°.

---

### 🧱 Canvas Creation
- User specifies upper limits for the X and Y canvas coordinates.
- Canvas bounds are validated with error handling.
- Escort ships are randomly placed with type and coordinates.
- Battleship is selected and placed by the user.
- Canvas is represented as a 1D array using pointer arithmetic:
  - `'*'` → Battleship  
  - `'E'` → Escort ship  
  - `'~'` → Empty space
- Initial conditions are saved to file for future access.

---

### 🚀 Projectile Calculations & Combat Outcomes
- Battleship’s attack range and position compared against each escort ship.
- Escort ship sinks if it falls within range.
- If any escort ship's attack range includes the battleship, it's declared sunk.
- Results include:
  - Number and details of sunk escort ships
  - Battlefield state
  - Coordinates and properties saved in `.txt` files

---

### 💾 Memory Management
- Dynamic memory for ships and canvas is freed at the end of simulation to avoid memory leaks.

---

## 🔁 Simulation Extensions

### 🔹 Part 1B – Simulation 1
- Battleship moves through a user-defined number of points.
- After each move:
  - Attack range recalculated
  - Combat checks re-evaluated
- Results saved after each move.

### 🔹 Part 1B – Simulation 2
- Firing angle is limited to 0°–60° after a few moves.
- Since angles > 45° maintain similar range, the impact is minimal.

### 🔹 Part 1C – Impact-Based Simulation
- Escort ships now require cumulative impact ≥ 1 to be destroyed.
- A `bool` flag `considerImpact` toggles this feature:
  - Set to `false` in earlier simulations.
  - Set to `true` in Part 1C.
- Previous functions were adapted using parameter flags rather than rewriting them entirely.

---

## ⚠️ Challenges & Solutions

### 🚧 Key Challenges:
- Managing random generation and dynamic memory allocation
- Ensuring input validity and boundary control
- Modifying existing functions to include new parameters like `impact`
- Calculating escort ship attack range using annulus shape logic

### ✅ Solutions:
- Implemented robust error checking for all user inputs
- Used helper functions for memory allocation and validation
- Handled impact mode with clean boolean parameterization for code reuse

---

## 🏁 Conclusion
The **Advanced Naval Battle Simulator** provides:
- A progressive simulation experience
- Realistic physics-based projectile modeling
- Modular code with proper memory and file handling

### 🔮 Future Enhancements
- Limit battleship attacks over time
- Restrict attacks to a specific number of escort ships
- Improve visuals or implement terminal-based graphics
- Add AI-controlled enemy behaviors

---

> **Developed by s-sandali** as part of an advanced programming coursework in simulation and low-level system design.

