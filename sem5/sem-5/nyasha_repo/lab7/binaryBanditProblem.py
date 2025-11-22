import random
import numpy as np

# Simulating the bandit actions as per given definitions in MATLAB code
def binaryBanditA(action):
    p = [0.1, 0.2]
    return 1 if random.random() < p[action - 1] else 0  # action-1 because MATLAB is 1-indexed

def binaryBanditB(action):
    p = [0.8, 0.9]
    return 1 if random.random() < p[action - 1] else 0

# Epsilon-greedy algorithm
def epsilon_greedy(epsilon, n_iterations):
    # Initializing reward estimates for both actions of both bandits
    Q_A = [0, 0]  # Estimated rewards for actions 1 and 2 of Bandit A
    Q_B = [0, 0]  # Estimated rewards for actions 1 and 2 of Bandit B
    N_A = [0, 0]  # Number of times actions 1 and 2 of Bandit A have been selected
    N_B = [0, 0]  # Number of times actions 1 and 2 of Bandit B have been selected
    
    rewards = []  # Keep track of all rewards to calculate cumulative reward

    for _ in range(n_iterations):
        # Decide which bandit to choose: A or B
        if random.random() < epsilon:
            bandit = random.choice(['A', 'B'])  # Explore: Randomly pick bandit
        else:
            # Exploit: Choose bandit and action based on highest expected reward
            avg_A = max(Q_A)
            avg_B = max(Q_B)
            bandit = 'A' if avg_A > avg_B else 'B'
        
        # Choose an action for the selected bandit
        if bandit == 'A':
            if random.random() < epsilon:
                action = random.choice([1, 2])  # Explore
            else:
                action = 1 if Q_A[0] > Q_A[1] else 2  # Exploit
            
            reward = binaryBanditA(action)
            N_A[action - 1] += 1  # Update count for the selected action
            Q_A[action - 1] += (reward - Q_A[action - 1]) / N_A[action - 1]  # Update reward estimate
        
        else:  # bandit == 'B'
            if random.random() < epsilon:
                action = random.choice([1, 2])  # Explore
            else:
                action = 1 if Q_B[0] > Q_B[1] else 2  # Exploit
            
            reward = binaryBanditB(action)
            N_B[action - 1] += 1  # Update count for the selected action
            Q_B[action - 1] += (reward - Q_B[action - 1]) / N_B[action - 1]  # Update reward estimate
        
        rewards.append(reward)  # Store the reward for this iteration
    
    return rewards, Q_A, Q_B

# Run the epsilon-greedy algorithm
epsilon = 0.1  # Exploration probability
n_iterations = 1000  # Number of iterations

rewards, Q_A, Q_B = epsilon_greedy(epsilon, n_iterations)

# Results
print(f"Estimated rewards for Bandit A (Action 1, 2): {Q_A}")
print(f"Estimated rewards for Bandit B (Action 1, 2): {Q_B}")
print(f"Total reward accumulated: {sum(rewards)}")

# Plotting cumulative rewards over time
import matplotlib.pyplot as plt
plt.plot(np.cumsum(rewards))
plt.title("Cumulative Rewards Over Time")
plt.xlabel("Iterations")
plt.ylabel("Cumulative Reward")
plt.show()
