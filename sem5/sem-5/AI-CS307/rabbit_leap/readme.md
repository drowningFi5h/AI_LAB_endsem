# Rabbit Leap Problem Solver

This project provides solutions to the Rabbit Leap problem using three different search algorithms: Breadth-First Search (BFS), Depth-First Search (DFS), and Iterative Deepening Depth-First Search (IDDFS).

## Problem Description

The Rabbit Leap problem involves a line of rabbits, where there are three white rabbits on the left, three black rabbits on the right, and an empty space in the middle. The goal is to swap the positions of the white and black rabbits. Rabbits can only move forward, either by stepping into an empty adjacent space or by jumping over one rabbit of the opposite color into an empty space.

## Implementation

The problem is solved using three different search algorithms, each implemented in a separate Python file:

1. `bfs.py`: Breadth-First Search implementation
2. `dfs.py`: Depth-First Search implementation
3. `iddfs.py`: Iterative Deepening Depth-First Search implementation

### Common Components

All implementations share these common elements:

- State representation: A string where 'E' represents a white rabbit, 'W' represents a black rabbit, and '_' represents the empty space.
- `Node` class: Represents a state in the search tree, with a parent reference for path reconstruction.
- `get_successors` function: Generates all possible valid next states from a given state.

### Algorithm-Specific Details

1. BFS (bfs.py):
   - Uses a queue to explore nodes in a breadth-first manner.
   - Explores 71 nodes to find the solution.

2. DFS (dfs.py):
   - Uses a stack to explore nodes in a depth-first manner.
   - Explores 37 nodes to find the solution.

3. IDDFS (iddfs.py):
   - Combines depth-limited search with iterative deepening.
   - Explores 844 nodes to find the solution.

## Usage

To run any of the implementations, use Python 3.x:

```
python bfs.py
python dfs.py
python iddfs.py
```

Each script will output:
- The number of nodes explored during the search
- The sequence of states from the initial state to the goal state

## Solution Interpretation

Each state in the solution is represented as a string:
- 'E': White rabbit
- 'W': Black rabbit
- '_': Empty space

The initial state is 'EEE_WWW' and the goal state is 'WWW_EEE'.

## Performance Comparison

- BFS: Explores 71 nodes
- DFS: Explores 37 nodes
- IDDFS: Explores 844 nodes

While DFS explores the fewest nodes in this case, it's important to note that this doesn't guarantee it will always find the optimal (shortest) solution. BFS is guaranteed to find the shortest path, while IDDFS combines the space efficiency of DFS with the completeness of BFS.

## Dependencies

These scripts use Python's standard library and have no external dependencies. They require Python 3.x to run.

## License

This project is open source and available under the [MIT License](https://opensource.org/licenses/MIT).