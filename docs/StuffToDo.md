### How to get current repo and push changes from clion

git pull origin main --rebase
git add .                           
git commit -m "What you changed"    
git push origin main

# Chess Puzzle Solver and Algorithm Comparison Project

## Overview
Our project aims to compare two non-trivial algorithms—**Backtracking (recursive depth-first search)** and **A\* Search (heuristic-based search)**—for solving chess puzzles, specifically focusing on mate-in-N puzzles. We will integrate Stockfish to generate legal moves and evaluate positions. For visualization, instead of building an interactive real-time GUI, we will automatically capture board snapshots during the solving process and compile these into GIF outputs that show each puzzle’s solution sequence.

## Project Goals
- **Compare Performance:** Speed, memory usage, and accuracy of Backtracking vs. A\* Search.
- **Integrate Stockfish:** For legal move generation and position evaluation.
- **Automate GIF Generation:** To display move sequences for each solved puzzle.

## Directory Structure
Project3/
├── src/
│   ├── main.cpp                 // Entry point; loads puzzles, runs solvers, and triggers GIF creation.
│   ├── puzzleLoader.cpp         // Functions for loading puzzle datasets.
│   ├── puzzleLoader.h
│   ├── chessEngineInterface.cpp // Interfaces with Stockfish (getLegalMoves(), isMate(), evaluatePosition()).
│   ├── chessEngineInterface.h
│   ├── solverBacktracking.cpp   // Implements recursive DFS (backtracking) for mate-in-N puzzles.
│   ├── solverBacktracking.h
│   ├── solverAStar.cpp          // Implements the A* solver (priority queue, heuristic functions, BoardState structure).
│   ├── solverAStar.h
│   ├── visualization.cpp        // Functions for generating GIFs from saved board snapshots.
│   ├── visualization.h
│   ├── utils.cpp                // Utility functions (FEN parsing, move conversion, logging).
│   └── utils.h
├── data/
│   └── puzzles.txt              // Dataset of mate-in-N puzzles (FEN positions, and optionally move sequences).
├── docs/
│   ├── README.md                // This file
│   └── projectPlan.docx         // Additional planning and meeting notes (if desired)
├── .gitignore                   // Exclude build files, CLion config, etc.
└── CMakeLists.txt               // Build configuration for CLion/CMake


## Detailed Module and Function Breakdown

### 1. **main.cpp**
- **Purpose:** Entry point of the application.
- **Responsibilities:**
    - Load puzzles from the dataset.
    - Choose and invoke the solvers (Backtracking and/or A\* Search).
    - Trigger the GIF output process after a solution is found.
- **Key Tasks:**
    - Initialize modules.
    - Loop over a set of puzzles (e.g., 100 puzzles) and display the solving process.
    - Optionally allow user input for pacing (e.g., “Next Puzzle”).

### 2. **Puzzle Loader (puzzleLoader.cpp / puzzleLoader.h)**
- **Functions:**
    - `vector<Puzzle> loadPuzzlesFromFile(const string &filename);`  
      Loads puzzles (FEN strings and metadata) into a vector of `Puzzle` objects.
    - `bool filterMateInNPuzzles(const Puzzle &puzzle, int N);`  
      Filters puzzles to include only mate-in-N puzzles.
- **Responsibilities:**
    - Parse different file formats (TXT, JSON, or PGN).
    - Provide a filtered dataset for testing and benchmarking.

### 3. **Chess Engine Interface (chessEngineInterface.cpp / chessEngineInterface.h)**
- **Functions:**
    - `vector<string> getLegalMoves(const string &fen);`  
      Interfaces with Stockfish to return all legal moves from a position.
    - `bool isMate(const string &fen);`  
      Checks if a given FEN represents a checkmate using Stockfish’s UCI commands.
    - `int evaluatePosition(const string &fen);`  
      Provides an evaluation score for heuristic calculations.
- **Responsibilities:**
    - Act as the intermediary to Stockfish.
    - Supply move generation and position evaluation services for solvers.

### 4. **Backtracking Solver (solverBacktracking.cpp / solverBacktracking.h)**
- **Function:**
    - `bool solvePuzzleBacktracking(const string &fen, int depth);`  
      Recursively explores each move sequence (depth-first search) until mate is found (or other defined goal).
- **Responsibilities:**
    - Traverse the move tree via recursion.
    - Backtrack when a move sequence does not lead to a solution.
- **Optimizations:**
    - Implement move ordering (e.g., checks first) and early exits upon finding a solution.

### 5. **A* Solver (solverAStar.cpp / solverAStar.h)**
- **Functions:**
    - `vector<string> solvePuzzleAStar(const string &fen);`  
      Uses A* Search to find the optimal sequence of moves leading to mate.
    - `int heuristic(const string &fen);`  
      Computes a heuristic estimate (e.g., based on Stockfish’s evaluation, material count).
- **Responsibilities:**
    - Use a priority queue to prioritize promising move sequences.
    - Reduce node expansion compared to exhaustive backtracking.
- **Optimizations:**
    - Fine-tune the heuristic for performance improvements.

### 6. **Visualization Module (visualization.cpp / visualization.h) — GIF Generation Only**
- **Functions:**
    - `void drawBoard(sf::RenderWindow &window);`  
      Renders a static chessboard using SFML.
    - `void drawPieces(sf::RenderWindow &window, const BoardState &state);`  
      Displays chess pieces according to the current position.
    - `void saveFrame(sf::RenderWindow &window, int frameNumber);`  
      Captures the current window as a PNG image (e.g., "frame_0.png").
    - `void generateSolutionGIF(const string &outputFile);`  
      Executes a system call (using ImageMagick or FFmpeg) to merge saved PNGs into an animated GIF.
- **Workflow:**
    1. After each move in the solution, update the board and call `saveFrame()`.
    2. Once the puzzle is solved, automatically generate a GIF:
        - **ImageMagick Example:**  
          `system("convert -delay 80 -loop 0 frame_*.png solution.gif");`
    3. Optionally, clean up by deleting the PNG frames.
- **Benefits:**
    - Provides a shareable, visual record of the solving process without needing an interactive GUI.

### 7. **Utility Functions (utils.cpp / utils.h)**
- **Functions:**
    - FEN parsing and conversion utilities.
    - Move conversion functions (UCI notation to board coordinates).
    - Logging and helper functions for error handling.
- **Responsibilities:**
    - Support the other modules, ensuring clean and modular code.

## Task Tracking and Work Distribution

Assign tasks as follows:

| **Module**                    | **Responsibilities**                                     | **Assigned To** |
|-------------------------------|----------------------------------------------------------|--|
| **Puzzle Loader**             | Parse datasets and filter puzzles                        |  |
| **Chess Engine Interface**    | Integrate Stockfish for move generation & evaluation       |  |
| **Backtracking Solver**       | Implement DFS/backtracking logic and optimizations         |  |
| **A* Solver**                 | Implement A* search and tune heuristics                   |  |
| **Visualization (GIF Generation)** | Render board snapshots, capture frames, and merge into GIF |  |
| **Utility Functions**         | FEN parsing, move conversion, logging                    |  |
| **Documentation & Integration** | Maintain README, merge modules, final testing             |  |

## Milestones and Timeline

- **Milestone 1: Dataset & Puzzle Loader (Due: MM/DD/YYYY)**
    - Download dataset and implement puzzle loader.

- **Milestone 2: Stockfish Integration (Due: MM/DD/YYYY)**
    - Develop `getLegalMoves()`, `isMate()`, and `evaluatePosition()` functions.

- **Milestone 3: Backtracking Solver (Due: MM/DD/YYYY)**
    - Create and test the recursive DFS solver on simple mate-in-N puzzles.

- **Milestone 4: A* Solver (Due: MM/DD/YYYY)**
    - Implement A* search with custom heuristic and benchmark against Backtracking.

- **Milestone 5: GIF Visualization Module (Due: MM/DD/YYYY)**
    - Implement board rendering and frame capture using SFML.
    - Automate GIF generation with ImageMagick/FFmpeg.
    - Test GIF output on sample puzzles.

- **Milestone 6: Integration & Testing (Due: MM/DD/YYYY)**
    - Integrate all modules and test on 100+ puzzles.
    - Document performance metrics and solver comparisons.

- **Milestone 7: Final Documentation & Presentation (Due: MM/DD/YYYY)**
    - Finalize README and documentation.
    - Prepare presentation slides.
