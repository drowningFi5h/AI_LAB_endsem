from collections import deque

class Node:
    def __init__(self, state, parent=None):
        self.state = state
        self.parent = parent

def get_successors(node):
    successors = []
    state = node.state
    empty_index = state.index('_')
    
    # Possible moves: step forward or jump over one rabbit
    for i, rabbit in enumerate(state):
        if rabbit == 'E' and i < empty_index:
            if i + 1 == empty_index or (i + 2 == empty_index and state[i+1] == 'W'):
                new_state = state[:i] + '_' + state[i+1:empty_index] + 'E' + state[empty_index+1:]
                successors.append(Node(new_state, node))
        elif rabbit == 'W' and i > empty_index:
            if i - 1 == empty_index or (i - 2 == empty_index and state[i-1] == 'E'):
                new_state = state[:empty_index] + 'W' + state[empty_index+1:i] + '_' + state[i+1:]
                successors.append(Node(new_state, node))
    
    return successors

def bfs(start_state, goal_state):
    start_node = Node(start_state)
    queue = deque([start_node])
    visited = set()
    nodes_explored = 0

    while queue:
        node = queue.popleft()
        if node.state == goal_state:
            path = []
            while node:
                path.append(node.state)
                node = node.parent
            print('Total nodes explored:', nodes_explored)
            return path[::-1]
        
        if node.state not in visited:
            visited.add(node.state)
            nodes_explored += 1
            print(node.state)
            for successor in get_successors(node):
                queue.append(successor)
    
    print('Total nodes explored:', nodes_explored)
    return None

# Define start and goal states
start_state = 'EEE_WWW'
goal_state = 'WWW_EEE'

# Find solution
solution = bfs(start_state, goal_state)

if solution:
    print("Solution found:")
    for step in solution:
        print(step)
else:
    print("No solution found.")