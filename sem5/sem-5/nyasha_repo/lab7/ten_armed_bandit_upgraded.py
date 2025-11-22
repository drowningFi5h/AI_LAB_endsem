import numpy as np
import matplotlib.pyplot as plt

# Nonstationary Bandit with random walk mean rewards
class BanditNonStat:
    def _init_(self, n_arms=10, mu=0.0, sigma_walk=0.01):
        self.n_arms = n_arms
        self.mu = np.ones(n_arms) * mu  # Initialize mean rewards equally
        self.sigma_walk = sigma_walk    # Std deviation for random walk

    def step(self, action):
        reward = np.random.randn() + self.mu[action]  # Generate reward for chosen arm
        self.mu += np.random.normal(0, self.sigma_walk, self.n_arms)  # Random walk
        return reward

# Modified Epsilon-Greedy Agent with constant step size (alpha)
class ModifiedEpsilonGreedyAgent:
    def _init_(self, n_actions, epsilon, alpha=0.1):
        self.n_actions = n_actions
        self.epsilon = epsilon
        self.alpha = alpha  # Constant step size
        self.q_values = np.zeros(n_actions)  # Initialize Q-values
        self.action_counts = np.zeros(n_actions)  # Count of actions taken

    def select_action(self):
        # Epsilon-greedy action selection
        if np.random.rand() < self.epsilon:  # Explore
            return np.random.randint(self.n_actions)
        else:  # Exploit
            return np.argmax(self.q_values)

    def update_q_values(self, action, reward):
        # Incremental Q-value update using constant step size
        self.q_values[action] += self.alpha * (reward - self.q_values[action])

def run_experiment(n_steps, bandit, agent):
    rewards = np.zeros(n_steps)
    actions = np.zeros(n_steps)
    for step in range(n_steps):
        action = agent.select_action()  # Choose action
        reward = bandit.step(action)  # Get reward
        agent.update_q_values(action, reward)  # Update Q-values
        rewards[step] = reward  # Track rewards
        actions[step] = action  # Track actions
    return rewards, actions

# Experiment settings
n_arms = 10
n_steps = 10000
epsilon = 0.1
alpha = 0.1  # Constant step-size for nonstationary tracking

# Create nonstationary bandit and modified agent
bandit = BanditNonStat(n_arms=n_arms)
agent = ModifiedEpsilonGreedyAgent(n_actions=n_arms, epsilon=epsilon, alpha=alpha)

# Run experiment for 10,000 steps
rewards, actions = run_experiment(n_steps, bandit, agent)

# Plot the cumulative average reward over time
plt.figure(figsize=(10, 6))
plt.plot(np.cumsum(rewards) / (np.arange(n_steps) + 1))
plt.xlabel('Steps')
plt.ylabel('Average Reward')
plt.title('Average Reward over Time (Modified Epsilon-Greedy, Nonstationary Bandit)')
plt.grid(True)
plt.show()

# Plot action selection frequencies
plt.figure(figsize=(10, 6))
plt.hist(actions, bins=np.arange(n_arms+1) - 0.5, rwidth=0.8, align='mid')
plt.xlabel('Action')
plt.ylabel('Frequency')
plt.title('Action Selection Frequency (Modified Epsilon-Greedy)')
plt.grid(True)
plt.xticks(np.arange(n_arms))
plt.show()