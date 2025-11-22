import matplotlib.pyplot as plt
import numpy as np

X = np.array([
    [1, 1, 1, 1, -1, -1, 1, -1, -1, 1, -1, 1, -1, -1, 1, -1, 1, -1, -1, 1, -1, 1, 1, 1, -1],
    [1, 1, 1, 1, 1, -1, -1, -1, 1, -1, -1, -1, -1, 1, -1, 1, -1, -1, 1, -1, 1, 1, 1, -1, -1],
    [-1, 1, 1, 1, 1, 1, -1, -1, -1, -1, 1, -1, -1, -1, -1, 1, -1, -1, -1, -1, -1, 1, 1, 1, 1],
    [1, -1, -1, -1, 1, 1, 1, -1, 1, 1, 1, -1, 1, -1, 1, 1, -1, -1, -1, 1, 1, -1, -1, -1, 1]
]).T  

def plot_char(c):
    plt.imshow(c.reshape(5, 5), cmap='binary')
    plt.axis('off')
    plt.show()

def weights(char):
    m, n = char.shape

    W = np.zeros((m, m))
    for i in range(n):
        W += np.outer(char[:, i], char[:, i])
    np.fill_diagonal(W, 0)
    W /=n
    return W

def hopfield(init_state, w_matrix, max_iter=100):
    x = init_state.copy()

    index = np.random.choice(len(x), 3, replace=False)
    x[index] *= -1

    plt.figure(figsize=(10,5))
    plt.subplot(1, 2, 1)
    plot_char(-X[:, 0])
    plt.title('Initial state')

    plt.subplot(1, 2, 2)
    plot_char(-x)
    plt.title('Noisy state')
    plt.tight_layout()
    plt.show()

    y = x.copy()
    for _ in range(max_iter):
        yp = np.sign(w_matrix @ y)

        if np.allclose(y, y, atol=1):
            break

        y = yp

        plt.figure()
        plot_char(-y)
        plt.title(f"iteration {_+1}")
        plt.pause(0.5)
    return y

ans = hopfield(X[:, 0], weights(X), max_iter=100)
plt.show()