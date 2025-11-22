from string import ascii_lowercase
import random
from itertools import combinations
import numpy as np

# Input Section
print("Enter the number of clauses ")
m = int(input())
print("Enter the number of variables in a clause ")
k = int(input())
print("Enter number of variables ")
n = int(input())

def createProblem(m, k, n, threshold=10):
    """
    Generates a list of unique 3-SAT problems.
    Each problem consists of `m` clauses, each with `k` variables.
    """
    # Lowercase for positive literals, uppercase for negative literals
    positive_var = (list(ascii_lowercase))[:n]
    negative_var = [c.upper() for c in positive_var]
    variables = positive_var + negative_var
    problems = []
    allCombs = list(combinations(variables, k))
    i = 0

    while i < threshold:
        c = random.sample(allCombs, m)
        if c not in problems:
            i += 1
            problems.append(list(c))
    return variables, problems

variables, problems = createProblem(m, k, n)

def assignment(variables, n):
    """
    Generates a random assignment of 0 or 1 to each variable.
    """
    forPositive = list(np.random.choice(2, n))
    forNegative = [abs(1 - i) for i in forPositive]
    assign = dict(zip(variables, forPositive + forNegative))
    return assign

def solve(problem, assign):
    """
    Evaluates how many clauses are satisfied by the assignment.
    A clause is satisfied if at least one of its literals is True (1).
    """
    count = 0
    for sub in problem:
        # A clause is satisfied if any of its literals is True
        if any(assign[val] for val in sub):
            count += 1
    return count

# Hill Climbing Implementation
def hillClimbing(problem, assign, parentNum, received, step):
    bestAssign = assign.copy()
    assignValues = list(assign.values())
    assignKeys = list(assign.keys())

    maxNum = parentNum
    maxAssign = assign.copy()
    editAssign = assign.copy()

    for i in range(len(assignValues)):
        step += 1
        # Flip the value of the variable
        editAssign[assignKeys[i]] = abs(assignValues[i] - 1)
        c = solve(problem, editAssign)
        if maxNum < c:
            received = step
            maxNum = c
            maxAssign = editAssign.copy()

    if maxNum == parentNum:
        s = f"{received}/{step - len(assignValues)}"
        return bestAssign, maxNum, s
    else:
        parentNum = maxNum
        bestAssign = maxAssign.copy()
        return hillClimbing(problem, bestAssign, parentNum, received, step)

# Beam Search Implementation
def beamSearch(problem, initial_assign, beam_width, max_steps=1000):
    """
    Implements Beam Search with a specified beam width.
    """
    beam = [(initial_assign.copy(), solve(problem, initial_assign))]
    steps = 0

    for step in range(1, max_steps + 1):
        candidates = []
        for assign, score in beam:
            for var in assign:
                # Generate neighbor by flipping variable
                neighbor = assign.copy()
                neighbor[var] = abs(neighbor[var] - 1)
                neighbor_score = solve(problem, neighbor)
                candidates.append((neighbor, neighbor_score))
                steps += 1
                # Early exit if all clauses are satisfied
                if neighbor_score == len(problem):
                    penetrance = f"{neighbor_score}/{len(problem)}"
                    return neighbor, f"{steps}/{max_steps}"

        # Select top `beam_width` candidates based on score
        candidates.sort(key=lambda x: x[1], reverse=True)
        beam = candidates[:beam_width]

        # Check if any assignment in the beam satisfies all clauses
        for assign, score in beam:
            if score == len(problem):
                penetrance = f"{score}/{len(problem)}"
                return assign, f"{steps}/{max_steps}"

    # If no perfect assignment found
    best_assign, best_score = beam[0]
    penetrance = f"{best_score}/{len(problem)}"
    return best_assign, f"{steps}/{max_steps}"

# Variable Neighborhood Descent Implementation
def variableNeighbor(problem, initial_assign, beam_width, max_steps=1000):
    """
    Implements Variable Neighborhood Descent with increasing beam widths.
    """
    current_assign = initial_assign.copy()
    current_score = solve(problem, current_assign)
    steps = 0
    b = beam_width

    for step in range(1, max_steps + 1):
        candidates = []
        for var in current_assign:
            neighbor = current_assign.copy()
            neighbor[var] = abs(neighbor[var] - 1)
            neighbor_score = solve(problem, neighbor)
            candidates.append((neighbor, neighbor_score))
            steps += 1
            if neighbor_score == len(problem):
                penetrance = f"{neighbor_score}/{len(problem)}"
                return neighbor, f"{steps}/{max_steps}", b

        # Select top `b` candidates
        candidates.sort(key=lambda x: x[1], reverse=True)
        top_candidates = candidates[:b]

        # Find the best candidate
        best_candidate, best_score = top_candidates[0]

        if best_score > current_score:
            current_assign = best_candidate.copy()
            current_score = best_score
            b = beam_width  # Reset beam width
            if best_score == len(problem):
                penetrance = f"{best_score}/{len(problem)}"
                return current_assign, f"{steps}/{max_steps}", b
        else:
            b += 1  # Increase beam width if no improvement

    # If no perfect assignment found
    penetrance = f"{current_score}/{len(problem)}"
    return current_assign, f"{steps}/{max_steps}", b

# Main Execution
hAssigns = []
assigns = []
h_n = []
initials = []
hill_penetration = []
beam_penetration_3 = []
beam_penetration_4 = []
var_penetration = []
vAssigns = []
bAssigns_3 = []
bAssigns_4 = []

i = 0

for problem in problems:
    i += 1
    assign = assignment(variables, n)
    initial = solve(problem, assign)

    # Hill Climbing
    bestAssign, score, hp = hillClimbing(problem, assign, initial, 1, 1)
    hAssigns.append(bestAssign)
    h_n.append(score)
    hill_penetration.append(hp)

    # Beam Search with beam width 3
    bs_assign_3, bs_p3 = beamSearch(problem, assign, beam_width=3)
    bAssigns_3.append(bs_assign_3)
    beam_penetration_3.append(bs_p3)

    # Beam Search with beam width 4
    bs_assign_4, bs_p4 = beamSearch(problem, assign, beam_width=4)
    bAssigns_4.append(bs_assign_4)
    beam_penetration_4.append(bs_p4)

    # Variable Neighborhood Descent
    v_assign, v_p, final_width = variableNeighbor(problem, assign, beam_width=3)
    var_penetration.append(v_p)
    vAssigns.append(v_assign)

    print(f'Problem {i}: {problem}')
    print(f'HillClimbing: {bestAssign}, Penetrance: {hp}')
    print(f'Beam Search (3): {bs_assign_3}, Penetrance: {bs_p3}')
    print(f'Beam Search (4): {bs_assign_4}, Penetrance: {bs_p4}')
    print(f'Variable Neighbourhood: {v_assign}, Penetrance: {v_p}')
    print()

# Optional: Summary of Penetrance Values
print("\nSummary of Penetrance Values:")
print("------------------------------------------------------------")
print("| Problem | Hill Climbing | Beam Search (3) | Beam Search (4) | VND |")
print("------------------------------------------------------------")
for idx in range(len(problems)):
    print(f"| {idx+1:7} | {hill_penetration[idx]:14} | {beam_penetration_3[idx]:15} | {beam_penetration_4[idx]:15} | {var_penetration[idx]:3} |")
print("------------------------------------------------------------")
