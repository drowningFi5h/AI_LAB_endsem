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

def dls(node, goal_state, limit, nodes_explored):
    nodes_explored[0] += 1  # Increment the node exploration count
    
    if node.state == goal_state:
        path = []
        while node:
            path.append(node.state)
            node = node.parent
        return path[::-1]
    
    if limit <= 0:
        return None

    for successor in get_successors(node):
        result = dls(successor, goal_state, limit - 1, nodes_explored)
        if result is not None:
            return result

    return None

def iddfs(start_state, goal_state):
    depth = 0
    nodes_explored = [0]  # Using a list to allow modification within functions

    while True:
        print(f"Exploring with depth limit: {depth}")
        start_node = Node(start_state)
        result = dls(start_node, goal_state, depth, nodes_explored)
        if result is not None:
            print('Total nodes explored:', nodes_explored[0])
            return result
        depth += 1

# Define start and goal states
start_state = 'EEE_WWW'
goal_state = 'WWW_EEE'

# Find solution using IDDFS
solution = iddfs(start_state, goal_state)

if solution:
    print("Solution found:")
    for step in solution:
        print(step)
else:
    print("No solution found.")