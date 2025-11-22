import heapq

# Representing the Marble Solitaire board (7x7 grid)
# Using -1 for invalid positions, 1 for marbles, and 0 for empty slots
initial_board = [
    [-1, -1, 1, 1, 1, -1, -1],
    [-1, -1, 1, 1, 1, -1, -1],
    [ 1,  1, 1, 1, 1,  1,  1],
    [ 1,  1, 1, 0, 1,  1,  1],
    [ 1,  1, 1, 1, 1,  1,  1],
    [-1, -1, 1, 1, 1, -1, -1],
    [-1, -1, 1, 1, 1, -1, -1]
]

# Directions for valid moves (jumping over a marble)
directions = [(-2, 0), (2, 0), (0, -2), (0, 2)]  # Up, Down, Left, Right

# Check if a move is valid
def is_valid_move(board, x, y, dx, dy):
    if 0 <= x + dx < 7 and 0 <= y + dy < 7 and board[x][y] == 1:
        # Check if there's a marble to jump over and an empty spot to land
        if board[x + dx // 2][y + dy // 2] == 1 and board[x + dx][y + dy] == 0:
            return True
    return False

# Apply a move (modify the board)
def apply_move(board, x, y, dx, dy):
    new_board = [row[:] for row in board]  # Create a deep copy of the board
    new_board[x][y] = 0  # Remove the marble
    new_board[x + dx // 2][y + dy // 2] = 0  # Remove the jumped-over marble
    new_board[x + dx][y + dy] = 1  # Place the marble in the new position
    return new_board

# Heuristic 1: Manhattan distance of all marbles from the center
def heuristic_manhattan(board):
    center = (3, 3)
    distance = 0
    for i in range(7):
        for j in range(7):
            if board[i][j] == 1:
                distance += abs(i - center[0]) + abs(j - center[1])
    return distance

# Heuristic 2: Number of valid moves available
def heuristic_moves_remaining(board):
    valid_moves = 0
    for x in range(7):
        for y in range(7):
            if board[x][y] == 1:
                for dx, dy in directions:
                    if is_valid_move(board, x, y, dx, dy):
                        valid_moves += 1
    return -valid_moves  # We want fewer valid moves to have higher priority

# Priority Queue Node
class Node:
    def __init__(self, board, g_cost, h_cost, path):
        self.board = board  # Current board state
        self.g_cost = g_cost  # Path cost (number of moves)
        self.h_cost = h_cost  # Heuristic cost
        self.f_cost = g_cost + h_cost  # Total cost for A* (g + h)
        self.path = path  # The sequence of moves leading to this state

    def __lt__(self, other):
        # This defines the priority for the priority queue (using f_cost for A*)
        return self.f_cost < other.f_cost

# Check if the board is in a goal state (only one marble remains at the center)
def is_goal_state(board):
    marble_count = 0
    for i in range(7):
        for j in range(7):
            if board[i][j] == 1:
                marble_count += 1
                if marble_count > 1:  # More than 1 marble, not the goal
                    return False
    # Ensure that the last marble is in the center
    return marble_count == 1 and board[3][3] == 1

# A* or Best-First Search
def best_first_search(initial_board, heuristic_fn, use_astar=False):
    priority_queue = []
    visited_states = set()
    
    # Initial node
    initial_node = Node(initial_board, g_cost=0, h_cost=heuristic_fn(initial_board), path=[])
    heapq.heappush(priority_queue, initial_node)
    
    while priority_queue:
        current_node = heapq.heappop(priority_queue)
        
        # If goal state reached
        if is_goal_state(current_node.board):
            return current_node.path  # Return the sequence of moves
        
        # Mark current state as visited
        visited_states.add(tuple(map(tuple, current_node.board)))
        
        # Try all valid moves
        for x in range(7):
            for y in range(7):
                if current_node.board[x][y] == 1:  # If it's a marble
                    for dx, dy in directions:
                        if is_valid_move(current_node.board, x, y, dx, dy):
                            new_board = apply_move(current_node.board, x, y, dx, dy)
                            if tuple(map(tuple, new_board)) not in visited_states:
                                new_g_cost = current_node.g_cost + 1  # Increase path cost
                                new_h_cost = heuristic_fn(new_board)
                                if use_astar:
                                    new_node = Node(new_board, new_g_cost, new_h_cost, current_node.path + [(x, y, dx, dy)])
                                else:
                                    new_node = Node(new_board, 0, new_h_cost, current_node.path + [(x, y, dx, dy)])
                                heapq.heappush(priority_queue, new_node)

    return None  # If no solution is found


# Run Best-First Search (Greedy Search) using moves remaining heuristic
best_first_solution = best_first_search(initial_board, heuristic_fn=heuristic_moves_remaining, use_astar=False)
print("Best-First Search Solution:", best_first_solution)

# Run A* Search using Manhattan distance heuristic
astar_solution = best_first_search(initial_board, heuristic_fn=heuristic_manhattan, use_astar=True)
print("A* Solution:", astar_solution)