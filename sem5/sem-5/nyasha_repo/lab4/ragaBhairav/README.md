# Raag Bhairav Melody Generation Using Genetic Algorithm

## Overview
This project implements a **genetic algorithm** to generate melodies that adhere to the structure and grammar of **Raag Bhairav**, a classical raga from Hindustani music. The goal is to evolve a melody that incorporates specific musical rules and characteristic phrases, such as the **Pakad** and **Mukhya Swar Sangati**. The melody evolves over generations until it is recognized as Raag Bhairav.

## Objective
The primary objective is to use a genetic algorithm to generate melodies that follow the grammar of Raag Bhairav. The algorithm will:
1. Initialize a population of random melodies.
2. Evaluate their fitness based on how well they conform to the structure of Raag Bhairav.
3. Apply genetic operators (selection, crossover, mutation) to evolve the population.
4. Continue this process until a melody recognizable as Raag Bhairav is found.

## Features
- **Genetic Algorithm**: Uses selection, crossover, and mutation to evolve melodies.
- **Fitness Function**: Rewards melodies that include characteristic musical phrases from Raag Bhairav.
- **Flexible Melody Length**: Melodies can vary in length between 2 and 10 notes.
- **Raag Recognition**: Melodies are checked for key Raag Bhairav phrases to ensure they follow the raga’s structure.

## Project Structure
The project is structured as follows:

- `genetic_algorithm.py`: Contains the main logic for the genetic algorithm, including fitness evaluation, crossover, mutation, and selection.
- `fitness.py`: Defines the fitness function used to score melodies.
- `raag_recognition.py`: Implements the logic to recognize whether a melody belongs to Raag Bhairav.
- `utils.py`: Helper functions for generating random melodies and printing them in Sargam notation.
- `README.md`: This documentation file.

## How It Works

### Genetic Algorithm Process:
1. **Initialization**: A population of random melodies is generated with lengths varying from 2 to 10 notes.
2. **Fitness Evaluation**: Each melody is scored based on its inclusion of Raag Bhairav's key phrases, usage of Vadi (Dha) and Samvadi (Re), and whether it ends on certain important notes (Nyaas/Upanyas).
3. **Selection**: Melodies are selected for reproduction using tournament selection, favoring those with higher fitness.
4. **Crossover**: Two parent melodies are combined at a random crossover point to create offspring.
5. **Mutation**: Offspring are mutated by changing, inserting, or deleting notes with a small probability.
6. **Termination**: The algorithm continues for a minimum of 20 generations and runs until a melody is found that adheres to Raag Bhairav.

### Raag Bhairav Recognition:
- The algorithm checks whether the melody contains at least three consecutive notes from Raag Bhairav's **Pakad** or **Mukhya Swar Sangati**. If the melody contains these sequences, it is considered recognizable as Raag Bhairav.

## Example Output
The program outputs the best melody found in each generation, along with its fitness score. When a valid Raag Bhairav melody is found, it is printed in Sargam notation.

