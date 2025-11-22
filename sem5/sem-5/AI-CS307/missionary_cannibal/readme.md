# Missionary-Cannibal Problem Solver

This Python script provides a solution to the classic Missionaries and Cannibals problem using a Depth-First Search (DFS) algorithm.

## Problem Description

The Missionaries and Cannibals problem is a river-crossing puzzle. The goal is to move three missionaries and three cannibals from one side of a river to the other using a boat that can carry at most two people. The constraint is that cannibals must never outnumber missionaries on either side of the river, as the cannibals would then eat the missionaries.

## Implementation

The problem is solved using the following key components:

1. State representation: A tuple `(missionaries, cannibals, boat)` where:
   - `missionaries` and `cannibals` represent the number on the starting side
   - `boat` is 1 if the boat is on the starting side, 0 otherwise

2. Functions:
   - `is_valid(state)`: Checks if a given state is valid
   - `get_successors(state)`: Generates all possible valid next states
   - `dfs(start_state, goal_state)`: Implements Depth-First Search to find a solution

## Usage

To run the script, simply execute it with Python:

```
python missionary_cannibal_solver.py
```

The script will output:
- The number of states visited during the search
- The length of the solution path
- The sequence of states from the initial state to the goal state

## Solution Interpretation

Each state in the solution is represented as a tuple `(m, c, b)` where:
- `m`: Number of missionaries on the starting side
- `c`: Number of cannibals on the starting side
- `b`: Boat location (1 for starting side, 0 for other side)

