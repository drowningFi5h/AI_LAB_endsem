import random

def game():
    n = 0
    R = []
    while n < 1000:
        terminate = False
        reward = 0
        while not terminate:
            num = random.random()
            if num < 1/3:
                reward += 10
                break
            else:
                reward += 4
        R.append(reward)
        n += 1
    sum = 0
    for r in R:
        sum += r
    # print(sum/len(R))
    return sum/len(R)
for i in range(1000):
    sum = 0
    # print("Game", i+1)
    sum += game()
    # print(game())
print(sum)