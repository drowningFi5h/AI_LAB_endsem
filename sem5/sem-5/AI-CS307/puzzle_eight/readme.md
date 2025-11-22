# 8-Puzzle Problem Solver

  

This Python script provides a solution to the classic 8-puzzle problem using **Breadth-First Search (BFS)** and an improved version utilizing **heuristic-based A* search**. The optimization introduces a heuristic to estimate the distance from the current state to the goal, improving efficiency in finding a solution.

  

## Problem Description

  

The 8-puzzle is a sliding puzzle that consists of a frame of numbered square tiles in random order, with one tile missing. The objective is to rearrange the tiles to reach a goal configuration.

  

## Implementation

  

The problem is solved using the following key components:

  

1. **State representation**: A list of 9 integers, where 0 represents the empty space.

2. **Node class**: Represents a state in the search tree, with a parent reference for path reconstruction.

3. **Functions**:

   - `valid_moves(index)`: Returns valid moves for each position.

   - `get_successors(node)`: Generates all possible valid next states.

   - `bfs(start_state, goal_state)`: Implements Breadth-First Search to find a solution.

   - `heuristic(node, goal_state)`: Estimates the cost of reaching the goal state from the current state (used in A* search).

   - `search_agent(start_state, goal_state)`: Implements A* search algorithm for optimized puzzle solving.

  

## Updates to the Solver

  

### Heuristic-Based A* Search

  

In this updated version, a heuristic function has been added to improve the search. The heuristic estimates the number of moves required to reach the goal state by calculating tile swaps, including special handling of the zero tile (empty space).

  

```python

def heuristic(node, goal_state):

    # Heuristic calculates the tile swaps needed to reach the goal state

    h = 0

    current_state = node.state

    for i, tile in enumerate(current_state):

        zero_tile = current_state.index(0)

        if current_state[i] == goal_state[i]:

            continue

        else:

            intended_tile = goal_state[i]

            intended_tile_index = current_state.index(intended_tile)

            current_tile = current_state[i]

            tile_to_be_swap = current_state[intended_tile_index]

            # Additional logic to handle zero tile swapping

            if intended_tile == zero_tile:

                h += 1

            else:

                h += 2

    return h

```

  

The A* algorithm, implemented in the `search_agent` function, prioritizes states with the lowest estimated cost (`f = g + h`), where `g` is the cost to reach the current state and `h` is the heuristic estimate to the goal.

  

### Improvements in Move Generation

  

The `get_successors` function is refined to generate valid moves based on the index of the empty space. It considers both row and column constraints for valid moves:

  

```python

def get_successors(node):

    # Generate valid successors based on empty tile position

    successors = []

    index = node.state.index(0)

    quotient = index // 3

    remainder = index % 3

    moves = []

    # Row and column constrained moves

    if quotient == 0: moves = [3]

    if quotient == 1: moves = [-3, 3]

    if quotient == 2: moves = [-3]

    if remainder == 0: moves += [1]

    if remainder == 1: moves += [-1, 1]

    if remainder == 2: moves += [-1]

  

    for move in moves:

        im = index + move

        if 0 <= im < 9:

            new_state = list(node.state)

            new_state[index], new_state[im] = new_state[im], new_state[index]

            successor = Node(new_state, node, node.g + 1)

            successors.append(successor)

    return successors

```

  

### Performance and Solution Output

  

The script outputs:

- The randomly generated start and goal states.

- The number of nodes explored during the search.

- The sequence of states from the initial state to the goal state.

  

For the A* algorithm, a priority queue is used (via `heapq`) to explore states efficiently based on the evaluation function `f = g + h`.

  

## Usage

  

To run the script, execute it with Python:

  

```bash

python A_star.py

```

  

The script will output:

- The start and goal states.

- The total number of nodes explored.

- The sequence of moves from the initial state to the goal state.

  

## Example Output

  

```bash

Start state: [1, 2, 3, 4, 5, 6, 7, 8, 0]

Goal state: [1, 0, 3, 4, 2, 5, 7, 8, 6]

Total nodes explored: 45

Solution found:

[1, 2, 3, 4, 5, 6, 7, 8, 0]

[1, 2, 3, 4, 5, 0, 7, 8, 6]
```


  

## Solution Interpretation

  

Each state in the solution is represented as a list of 9 integers, where `0` represents the empty space. The tiles are numbered 1-8. The sequence shows the movement of the empty space as the puzzle progresses from the start state to the goal state.

  

## Random Goal State Generation

  

The script generates a random goal state by applying 20 random moves to the solved state. This ensures that the puzzle is solvable and within a reasonable distance from the starting state.

---
