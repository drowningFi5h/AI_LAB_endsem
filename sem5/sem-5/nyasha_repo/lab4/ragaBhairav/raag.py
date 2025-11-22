import random

# Define notes for Raag Bhairav
NOTES = [1, 2, 3, 4, 5, 6, 7]  # Sa, Re, Ga, Ma, Pa, Dha, Ni
SARGAM = {
    1: "Sa",
    2: "Re",
    3: "Ga",
    4: "Ma",
    5: "Pa",
    6: "Dha",
    7: "Ni"
}

VADI = 6  # Dha
SAMVADI = 2  # Re
NYAAS = [1, 2, 5, 6]  # Sa, Re, Pa, Dha
UPANYAS = [4]  # Ma

# Pakad and Mukhya Swar Sangati phrases
PAKAD = [3, 4, 6, 6, 5, 3, 4, 2, 2, 1]
MUKHYA_SWAR_SANGATI = [
    [3, 4, 2, 2, 1],  # Ga Ma Re Re Sa
    [7, 1, 6, 6, 5],  # Ni Sa Dha Dha Pa
    [3, 4, 6, 6, 5],  # Ga Ma Dha Dha Pa
    [5, 6, 4, 5, 3],  # Pa Dha Ma Pa Ga
    [6, 7, 1, 4],     # Dha Ni Sa Ma
]

# Function to check if the melody is recognizable as Raag Bhairav
def is_raag_bhairav(melody):
    """
    Determines if the given melody is recognizable as Raag Bhairav
    by checking for the presence of at least 3 consecutive notes
    from the Pakad or Mukhya Swar Sangati phrases.
    """
    # Combine Pakad and Mukhya Swar Sangati phrases
    characteristic_phrases = [PAKAD] + MUKHYA_SWAR_SANGATI

    # Check for at least 3 consecutive matching notes
    for phrase in characteristic_phrases:
        for i in range(len(phrase) - 2):  # Start positions for subsequences of length 3
            sub_phrase = phrase[i:i+3]
            if contains_subsequence(melody, sub_phrase):
                return True

    # If no matching subsequence of length >= 3 is found
    return False

def contains_subsequence(melody, sub_phrase):
    """
    Checks if the melody contains the given sub_phrase as a consecutive subsequence.
    """
    for i in range(len(melody) - len(sub_phrase) + 1):
        if melody[i:i+len(sub_phrase)] == sub_phrase:
            return True
    return False

# Fitness function to evaluate the melody
def fitness(melody):
    score = 0

    # Reward for using Vadi and Samvadi notes
    score += melody.count(VADI) * 2  # Vadi (Dha)
    score += melody.count(SAMVADI) * 2  # Samvadi (Re)

    # Reward for ending on Nyaas notes (higher priority)
    if melody[-1] in NYAAS:
        score += 15
    # Reward for ending on Upanyas note (lower priority)
    elif melody[-1] in UPANYAS:
        score += 10

    # Reward for including Mukhya Swar Sangati phrases
    for phrase in MUKHYA_SWAR_SANGATI:
        if contains_subsequence(melody, phrase):
            score += 10

    # Reward for including Pakad
    if contains_subsequence(melody, PAKAD):
        score += 20  # Increased reward for Pakad

    return score

# Generate random melody with variable length
def generate_melody():
    length = random.randint(2, 10)  # Melody length between 2 and 10
    return [random.choice(NOTES) for _ in range(length)]

# Selection: Tournament selection
def selection(population):
    selected = random.sample(population, k=min(5, len(population)))
    selected.sort(key=lambda x: x[1], reverse=True)
    return selected[0][0]  # Return the melody

# Crossover: Single point crossover with variable-length handling
def crossover(parent1, parent2):
    min_length = min(len(parent1), len(parent2))
    if min_length > 1:
        point = random.randint(1, min_length - 1)
        offspring = parent1[:point] + parent2[point:]
    else:
        offspring = parent1 if random.random() < 0.5 else parent2

    # Ensure offspring length is within allowed limits
    if len(offspring) < 2:
        offspring.append(random.choice(NOTES))
    elif len(offspring) > 10:
        offspring = offspring[:10]
    return offspring

# Mutation: Change, add, or remove a note
def mutate(melody):
    mutation_type = random.choice(['change', 'insert', 'delete'])
    if mutation_type == 'change':
        index = random.randint(0, len(melody) - 1)
        melody[index] = random.choice(NOTES)
    elif mutation_type == 'insert' and len(melody) < 10:
        index = random.randint(0, len(melody))
        melody.insert(index, random.choice(NOTES))
    elif mutation_type == 'delete' and len(melody) > 2:
        index = random.randint(0, len(melody) - 1)
        del melody[index]
    return melody

# Genetic Algorithm
def genetic_algorithm(population_size=20):
    generation = 0
    population = []
    for _ in range(population_size):
        melody = generate_melody()
        population.append((melody, 0))  # (melody, fitness)

    while True:
        generation += 1

        # Evaluate fitness of the population
        evaluated_population = []
        for melody, _ in population:
            score = fitness(melody)
            evaluated_population.append((melody, score))

        # Sort population based on fitness
        evaluated_population.sort(key=lambda x: x[1], reverse=True)

        # Check if any melody is recognizable as Raag Bhairav
        for melody, score in evaluated_population:
            if is_raag_bhairav(melody):
                print(f"Generation {generation}: Found a melody recognizable as Raag Bhairav!")
                return melody, score, generation

        # Print best melody every 50 generations
        if generation % 50 == 0:
            best_melody = evaluated_population[0]
            print(f"Generation {generation}: Best Melody so far: {best_melody[0]}, Fitness: {best_melody[1]}")

        # Create new population
        new_population = []
        while len(new_population) < population_size:
            parent1 = selection(evaluated_population)
            parent2 = selection(evaluated_population)
            # Crossover
            offspring1 = crossover(parent1, parent2)
            offspring2 = crossover(parent2, parent1)
            # Mutation
            if random.random() < 0.2:
                offspring1 = mutate(offspring1)
            if random.random() < 0.2:
                offspring2 = mutate(offspring2)
            # Add offspring to new population
            new_population.append((offspring1, 0))
            new_population.append((offspring2, 0))
        population = new_population[:population_size]

# Function to convert the melody to Sargam
def print_melody_as_sargam(melody):
    sargam_melody = [SARGAM.get(note, "?") for note in melody]
    print("Melody in Sargam:", ' '.join(sargam_melody))

# Run the genetic algorithm
best_melody, best_score, total_generations = genetic_algorithm(population_size=20)

# Print the best melody
print("\nBest Melody Found:")
print_melody_as_sargam(best_melody)
print(f"Fitness Score: {best_score}")
print(f"Total Generations: {total_generations}")
print("The melody is recognizable as Raag Bhairav.")
