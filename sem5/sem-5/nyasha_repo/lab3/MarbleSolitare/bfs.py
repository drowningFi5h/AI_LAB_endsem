import heapq
import time

class Node:
    def __init__(self, board, g=0, h=0, path=None):
        self.board = board
        self.g = g
        self.path = path if path is not None else []

    def __lt__(self, other):
        return self.g < other.g

directions = [(0, -2), (0, 2), (2, 0), (-2, 0)]  

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
def num_marblres(board):
    count = 0
    for i in range(len(board)):
        for j in range(len(board[i])):
            if(board[i][j]) == 1:
                count+=1
    return count




def get_successor(node):
    successors = []
    for x in range(7):
        for y in range(7):
            if node.board[x][y] == 1:
                for dx, dy in directions:
                    if is_valid(node.board, x, y, dx, dy):
                        new_board = apply_move(node.board, x, y, dx, dy)
                        successor_node = Node(new_board, g=num_marblres(new_board), path=node.path + [(x, y, x + dx, y + dy)])
                        successors.append(successor_node)
    return successors

def search_agent(start_board):
    start_node = Node(start_board, g = 32)
    frontier = []
    heapq.heappush(frontier, (start_node.g,start_node))
    visited_states = set()
    nodes_expanded = 0
    start_time = time.time()

    while frontier:
        _, current_node = heapq.heappop(frontier)
        nodes_expanded += 1

        if is_goal_state(current_node.board):
            print(f"Total number of nodes explored: {nodes_expanded}")
            return current_node.path

        visited_states.add(tuple(map(tuple, current_node.board)))

        for successor in get_successor(current_node):
            if tuple(map(tuple, successor.board)) not in visited_states:
                    heapq.heappush(frontier, (successor.g,successor))

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
