import string
import random
from itertools import product

def get_user_input():
    return {
        'clause_count': int(input("How many clauses? ")),
        'vars_per_clause': int(input("How many variables per clause? ")),
        'total_vars': int(input("Total number of variables? "))
    }

def generate_sat_instance(params):
    symbols = string.ascii_lowercase[:params['total_vars']]
    all_vars = symbols + symbols.upper()
    clause_options = list(product(all_vars, repeat=params['vars_per_clause']))
    
    return symbols, [random.sample(clause_options, params['clause_count']) for _ in range(5)]

def create_random_solution(variables):
    return {v: random.choice([True, False]) for v in variables}

def evaluate_solution(instance, solution):
    return sum(any(solution[v.lower()] ^ v.isupper() for v in clause) for clause in instance)

def local_search(instance, initial_solution, max_iterations=1000):
    current = initial_solution.copy()
    best_score = evaluate_solution(instance, current)
    
    for i in range(max_iterations):
        var_to_flip = random.choice(list(current.keys()))
        neighbor = current.copy()
        neighbor[var_to_flip] = not neighbor[var_to_flip]
        
        new_score = evaluate_solution(instance, neighbor)
        if new_score > best_score:
            current = neighbor
            best_score = new_score
            if best_score == len(instance):
                return current, f"{i+1}/{max_iterations}"
    
    return current, f"{max_iterations}/{max_iterations}"

def beam_search(instance, beam_width, variables, max_iterations=1000):
    initial_solution = create_random_solution(variables)
    beam = [(evaluate_solution(instance, initial_solution), initial_solution)]
    
    for i in range(max_iterations):
        candidates = []
        for score, solution in beam:
            for var in solution:
                neighbor = solution.copy()
                neighbor[var] = not neighbor[var]
                new_score = evaluate_solution(instance, neighbor)
                candidates.append((new_score, neighbor))
        
        # Sort by score (first element of tuple) instead of attempting to compare dictionaries
        beam = sorted(candidates, key=lambda x: x[0], reverse=True)[:beam_width]
        
        if not beam:  # If beam becomes empty, reinitialize with a new random solution
            new_solution = create_random_solution(variables)
            beam = [(evaluate_solution(instance, new_solution), new_solution)]
        
        best_score, best_solution = beam[0]
        if best_score == len(instance):
            return best_solution, f"{i+1}/{max_iterations}"
    
    return beam[0][1], f"{max_iterations}/{max_iterations}"


def adaptive_search(instance, initial_beam_width, variables, max_iterations=1000):
    beam_width = initial_beam_width
    solution = create_random_solution(variables)
    
    for i in range(max_iterations):
        candidates = []
        for _ in range(beam_width):
            neighbor = solution.copy()
            var_to_flip = random.choice(list(neighbor.keys()))
            neighbor[var_to_flip] = not neighbor[var_to_flip]
            score = evaluate_solution(instance, neighbor)
            candidates.append((score, neighbor))
        
        best_candidate = max(candidates, key=lambda x: x[0])
        if best_candidate[0] > evaluate_solution(instance, solution):
            solution = best_candidate[1]
            beam_width = initial_beam_width
        else:
            beam_width += 1
        
        if evaluate_solution(instance, solution) == len(instance):
            return solution, f"{i+1}/{max_iterations}", beam_width
    
    return solution, f"{max_iterations}/{max_iterations}", beam_width

def main():
    params = get_user_input()
    variables, instances = generate_sat_instance(params)
    
    for idx, instance in enumerate(instances, 1):
        print(f"\nInstance {idx}:")
        initial_solution = create_random_solution(variables)
        initial_score = evaluate_solution(instance, initial_solution)
        
        ls_solution, ls_steps = local_search(instance, initial_solution)
        bs_solution, bs_steps = beam_search(instance, 3, variables)
        as_solution, as_steps, final_width = adaptive_search(instance, 2, variables)
        
        print(f"Initial Score: {initial_score}")
        print(f"Local Search: Score = {evaluate_solution(instance, ls_solution)}, Steps = {ls_steps}")
        print(f"Beam Search: Score = {evaluate_solution(instance, bs_solution)}, Steps = {bs_steps}")
        print(f"Adaptive Search: Score = {evaluate_solution(instance, as_solution)}, Steps = {as_steps}, Final Width = {final_width}")

if __name__ == "__main__":
    main()