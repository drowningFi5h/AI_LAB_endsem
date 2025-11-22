import numpy as np
import matplotlib.pyplot as plt

class BanditNonStat:
    def _init_(self, n_arms=10, mu=0.0, sigma_walk=0.01):
        self.n_arms = n_arms
        self.mu = np.ones(n_arms) * mu  # initialize mean rewards equally
        self.sigma_walk = sigma_walk  # standard deviation for random walk

    def step(self, action):
        reward = np.random.randn() + self.mu[action]  # generate reward for chosen arm
        self.mu += np.random.normal(0, self.sigma_walk, self.n_arms)  # random walk
        return reward

class EpsilonGreedyAgent:
    def _init_(self, n_actions, epsilon, alpha=None):
        self.n_actions = n_actions
        self.epsilon = epsilon
        self.alpha = alpha
        self.q_values = np.zeros(n_actions)  # initialize Q-values
        self.action_counts = np.zeros(n_actions)  # counts for each action

    def select_action(self):
        if np.random.rand() < self.epsilon:  # explore
            return np.random.randint(self.n_actions)
        else:  # exploit
            return np.argmax(self.q_values)

    def update_q_values(self, action, reward):
        self.action_counts[action] += 1
        if self.alpha:  # constant step size
            self.q_values[action] += self.alpha * (reward - self.q_values[action])
        else:  # sample average method
            self.q_values[action] += (1.0 / self.action_counts[action]) * (reward - self.q_values[action])

def run_experiment(n_steps, bandit, agent):
    rewards = np.zeros(n_steps)
    for step in range(n_steps):
        action = agent.select_action()
        reward = bandit.step(action)
        agent.update_q_values(action, reward)
        rewards[step] = reward
    return rewards

# Settings
n_arms = 10
n_steps = 1000
epsilon = 0.1
alpha = 0.1  # optional: for constant step size, if None uses sample average

bandit = BanditNonStat(n_arms=n_arms)
agent = EpsilonGreedyAgent(n_actions=n_arms, epsilon=epsilon, alpha=alpha)
rewards = run_experiment(n_steps, bandit, agent)

# Plot results
plt.plot(np.cumsum(rewards) / (np.arange(n_steps) + 1))
plt.xlabel('Steps')
plt.ylabel('Average Reward')
plt.title('10-Armed Nonstationary Bandit')
plt.grid()
plt.show()