#state representation (w-wall, t-terminal state) -> each state-tuple(row, col)
grid = [
    ['w', 's', 's', 's', 't'],
    ['w', 's', 'w', 's', 't'],
    ['w', 's', 's', 's', 's'],
    ['w', 'w', 'w', 'w', 'w'],
]

#up down left right
action = [0, 1, 2, 3]
probability = [0.8, 0.1, 0.1]


#discount factor 
y = 0.9

def is_valid_state(row, col):
    return 0 <= row < len(grid) and 0 <= col < len(grid[0]) and grid[row][col] != 'w'

def up(curr_state):
    row, col = curr_state
    next_states = [(row-1, col), (row, col+1), (row, col-1)]
    return calculate_value(next_states, row, col)

def down(curr_state):
    row, col = curr_state
    next_states = [(row+1, col), (row, col-1), (row, col+1)]
    return calculate_value(next_states, row, col)

def left(curr_state):
    row, col = curr_state
    next_states = [(row, col-1), (row+1, col), (row-1, col)]
    return calculate_value(next_states, row, col)

def right(curr_state):
    row, col = curr_state
    next_states = [(row, col+1), (row+1, col), (row-1, col)]
    return calculate_value(next_states, row, col)

def calculate_value(next_states, c_row, c_col):
    value = 0
    for i in range(len(next_states)):
        row, col = next_states[i]
        if (is_valid_state(row, col)):
            value += ( (v_state[row][col])*y + reward[row][col] ) * probability[i]
        else:
            value += ( (v_state[c_row][c_col])*y + reward[c_row][c_col] ) * probability[i]
    return value

def print_v(v_state, iteration):
    print(f"Iteration: {iteration}")
    for i in range(len(v_state)-1):  
        print(" ".join([f"{v_state[i][j]:.3f}" for j in range(1, len(v_state[i]))])) 
    print()

    

def value_iteration():
    theta = 1e-3
    iteration = 0
    # print_v(v_state, iteration)
    while True:
        delta = 0
        iteration += 1
        for i in range(len(v_state)):
            for j in range(len(v_state[0])):
                # if (i==0 or j==0 or ((i,j)==(1,2)) or ((i,j)==(0,4)) or ((i,j)==(1,4))):
                if (grid[i][j] == 'w' or grid[i][j] == 't'):
                    continue
                else:
                    curr_val_fun = v_state[i][j]
                    #calculate max value function for the given actions
                    value_fun = max(down((i,j)), up((i,j)))
                    value_fun = max(value_fun, left((i,j)))
                    value_fun = max(value_fun, right((i,j)))
                    #update the value function for the state
                    v_state[i][j] = value_fun
                    #update delta if possible
                    delta = max(delta, abs(value_fun - curr_val_fun))
        # print_v(v_state, iteration)
        if delta < theta:
            print_v(v_state, iteration)
            print(f"Converger after {iteration} iterations")
            break



r = [0.04, -2, 0.1, 0.02, 1]
for r in r:
    reward = [
        [0, r, r, r, 1],
        [0, r, 0, r, -1],
        [0, r, r, r, r],
        [0, 0, 0, 0, 0]
    ]
    v_state = [
    [0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0]
    ]
    print("############################################################################")
    print()
    print(f"for r(s) = {r}:")
    value_iteration()