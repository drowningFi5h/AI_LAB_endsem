import heapq
import time

class Node:
    def __init__(self, board, g=0, h=0, path=None):
        self.board = board
        self.g = g
        self.h = h
        self.f = g + h
        self.path = path if path is not None else []

    def __lt__(self, other):
        return self.f < other.f

directions = [(0, -2), (0, 2), (-2, 0), (2, 0)]  # up, down, left, right

def is_valid(board, x, y, dx, dy):
    if 0 <= x + dx < 7 and 0 <= y + dy < 7 and board[x][y] == 1:
        if board[x + dx // 2][y + dy // 2] == 1 and board[x + dx][y + dy] == 0:
            return True
    return False

def apply_move(board, x, y, dx, dy):
    new_board = [row[:] for row in board]
    new_board[x][y] = 0
    new_board[x + dx // 2][y + dy // 2] = 0
    new_board[x + dx][y + dy] = 1
    return new_board

def manhattan_heuristic(board):
    center = (3, 3)  # Assuming the center is at (3, 3) for a 7x7 board
    distance = 0
    for i in range(len(board)):
        for j in range(len(board[i])):
            if board[i][j] == 1:
                distance += abs(i - center[0]) + abs(j - center[1])
    return distance

def get_successor(node):
    successors = []
    for x in range(7):
        for y in range(7):
            if node.board[x][y] == 1:
                for dx, dy in directions:
                    if is_valid(node.board, x, y, dx, dy):
                        new_board = apply_move(node.board, x, y, dx, dy)
                        successor_node = Node(new_board, g=node.g + 1, h=manhattan_heuristic(new_board), path=node.path + [(x, y, x + dx, y + dy)])
                        successors.append(successor_node)
    return successors

def search_agent(start_board):
    start_node = Node(start_board)
    frontier = []
    heapq.heappush(frontier, (start_node.f,start_node))
    visited_states = set()
    nodes_expanded = 0
    start_time = time.time()

    while frontier:
        _, current_node = heapq.heappop(frontier)
        nodes_expanded += 1

        if is_goal_state(current_node.board):
            return current_node.path

        visited_states.add(tuple(map(tuple, current_node.board)))

        for successor in get_successor(current_node):
            if tuple(map(tuple, successor.board)) not in visited_states:
                    heapq.heappush(frontier, (successor.f,successor))

        if time.time() - start_time >= 20:
            print(f"Nodes expanded: {nodes_expanded}")
            start_time = time.time()
    return None

def is_goal_state(board):
    marble_count = sum(row.count(1) for row in board)
    return marble_count == 1 and board[3][3] == 1

initial_board = [
    [-1, -1, 1, 1, 1, -1, -1],
    [-1, -1, 1, 1, 1, -1, -1],
    [1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 0, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1],
    [-1, -1, 1, 1, 1, -1, -1],
    [-1, -1, 1, 1, 1, -1, -1]
]

solution_path = search_agent(initial_board)
if solution_path:
    print("Solution Found")
    for s in solution_path:
        print(s)
else:
    print("No solution")
