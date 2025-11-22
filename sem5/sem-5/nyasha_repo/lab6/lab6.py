import numpy as np

def initialize_network(n):
    neurons = n * n
    
    weights = np.zeros((neurons, neurons))
    
    for i in range(n):
        for j in range(n):
            idx1 = i * n + j
            for k in range(n):
                if k != j:  # Same row, different column
                    idx2 = i * n + k
                    weights[idx1, idx2] = -2
            for k in range(n):
                if k != i:  # Same column, different row
                    idx2 = k * n + j
                    weights[idx1, idx2] = -2
    
    thresholds = np.full(neurons, -1)
    
    return weights, thresholds

def update_neuron_states(states, weights, thresholds):
    new_states = np.copy(states)
    for i in range(len(states)):
        total_input = np.dot(weights[i], new_states) + thresholds[i]
        new_states[i] = 1 if total_input >=-1 else 0
    return new_states

def calculate_energy(states, weights, thresholds):
    return -0.5 * np.dot(states, np.dot(weights, states)) - np.dot(thresholds, states)

def solve_eight_rooks(n=8, max_iterations=10):
    weights, thresholds = initialize_network(n)
    
    states = np.random.choice([0], size=(n * n))
    states[0] =1 
    for i in range(n):
        row_start = i * n
        row_end = (i + 1) * n
        if not states[row_start:row_end].any():   
            states[row_start + np.random.randint(0, n)] = 1 
    
    print(states)
    for iteration in range(max_iterations):
        new_states = update_neuron_states(states, weights, thresholds)
        if np.array_equal(new_states, states):  
            break
        states = new_states


    board = states.reshape((n, n))
    valid = (board.sum(axis=0).max() == 1) and (board.sum(axis=1).max() == 1)

    return board if valid else None

# Solve the 8-rooks problem
solution = solve_eight_rooks()
if solution is not None:
    print("Solution found:")
    print(solution)
else:
    print("No valid solution found.")
