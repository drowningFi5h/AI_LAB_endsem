import math
import random

def distance(city1, city2):
    return math.sqrt((city1[0] - city2[0])**2 + (city1[1] - city2[1])**2)

def total_distance(tour, cities):
    return sum(distance(cities[tour[i]], cities[tour[i-1]]) for i in range(len(tour)))

def simulated_annealing(cities, temp, cooling_rate, iterations):
    num_cities = len(cities)
    current_tour = list(range(num_cities))
    random.shuffle(current_tour)
    current_distance = total_distance(current_tour, cities)
    
    best_tour = current_tour[:]
    best_distance = current_distance
    
    for i in range(iterations):
        if temp <= 0:
            break
        
        # Choose two random cities to swap
        i, j = random.sample(range(num_cities), 2)
        new_tour = current_tour[:]
        new_tour[i], new_tour[j] = new_tour[j], new_tour[i]
        
        new_distance = total_distance(new_tour, cities)
        
        # Decide whether to accept the new tour
        if new_distance < current_distance or random.random() < math.exp((current_distance - new_distance) / temp):
            current_tour = new_tour
            current_distance = new_distance
        
        # Update the best tour if necessary
        if current_distance < best_distance:
            best_tour = current_tour[:]
            best_distance = current_distance
        
        # Cool down the temperature
        temp *= cooling_rate
    
    return best_tour, best_distance

rajasthan_cities = {
    "Jaipur": (26.9, 75.8),
    "Udaipur": (24.6, 73.7),
    "Jodhpur": (26.3, 73.0),
    "Jaisalmer": (26.9, 70.9),
    "Pushkar": (26.5, 74.6),
    "Ranthambore": (26.0, 76.5),
    "Mount Abu": (24.6, 72.7),
    "Ajmer": (26.5, 74.6),
    "Bikaner": (28.0, 73.3),
    "Chittorgarh": (24.9, 74.6)
}

# Convert dictionary to list of tuples
cities = list(rajasthan_cities.values())
city_names = list(rajasthan_cities.keys())

# Run simulated annealing
best_tour, best_distance = simulated_annealing(cities, temp=1000, cooling_rate=0.995, iterations=10000)

# Print the results
print("Best tour:")
for i in best_tour:
    print(city_names[i])
print(f"\nTotal distance: {best_distance:.2f}")