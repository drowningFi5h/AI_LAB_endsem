import math
import random
import requests

def euclidean_distance(city1, city2):
    return math.sqrt((city1[0] - city2[0])**2 + (city1[1] - city2[1])**2)

def total_distance(tour, cities):
    return sum(euclidean_distance(cities[tour[i]], cities[tour[i-1]]) for i in range(len(tour)))

def simulated_annealing(cities, temp, cooling_rate, iterations):
    num_cities = len(cities)
    current_tour = list(range(num_cities))
    random.shuffle(current_tour)
    current_dist = total_distance(current_tour, cities)
    
    best_tour = current_tour[:]
    best_dist = current_dist
    
    for _ in range(iterations):
        if temp <= 0:
            break
        
        i, j = random.sample(range(num_cities), 2)
        new_tour = current_tour[:]
        new_tour[i], new_tour[j] = new_tour[j], new_tour[i]
        
        new_dist = total_distance(new_tour, cities)
        
        if new_dist < current_dist or random.random() < math.exp((current_dist - new_dist) / temp):
            current_tour = new_tour
            current_dist = new_dist
        
        if current_dist < best_dist:
            best_tour = current_tour[:]
            best_dist = current_dist
        
        temp *= cooling_rate
    
    return best_tour, best_dist

def read_tsp_file(url):
    response = requests.get(url)
    lines = response.text.strip().split('\n')
    
    # Skip the first 9 lines and remove the last line (EOF)
    data_lines = lines[9:-1]
    
    cities = []
    for line in data_lines:
        parts = line.split()
        if len(parts) == 3 and parts[0].isdigit():
            _, x, y = map(int, parts)
            cities.append((x, y))
    return cities

def solve_vlsi_tsp(url):
    cities = read_tsp_file(url)
    best_tour, best_dist = simulated_annealing(cities, temp=1000, cooling_rate=0.995, iterations=100000)
    return best_dist

# Solve 5 VLSI TSP instances
vlsi_problems = [
    "http://www.math.uwaterloo.ca/tsp/vlsi/xqf131.tsp",
    "http://www.math.uwaterloo.ca/tsp/vlsi/xqg237.tsp",
    "http://www.math.uwaterloo.ca/tsp/vlsi/pma343.tsp",
    "http://www.math.uwaterloo.ca/tsp/vlsi/pka379.tsp",
    "http://www.math.uwaterloo.ca/tsp/vlsi/bcl380.tsp"
]

results = {}
for problem_url in vlsi_problems:
    problem_name = problem_url.split('/')[-1].split('.')[0]
    tour_distance = solve_vlsi_tsp(problem_url)
    results[problem_name] = tour_distance

# Print results
print("VLSI TSP Results:")
for problem, tour_distance in results.items():
    print(f"{problem}: {tour_distance:.2f}")