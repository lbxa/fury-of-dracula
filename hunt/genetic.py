import json
import random
import numpy as np
import os
import subprocess


# Get Game Evaluation
def extract_evaluation_scores():
    num_games = 0
    scores_total = 0

    for f in os.listdir("logs"):
        log = open(os.path.join("logs", f), "r")
        content = log.read()
        lines = content.split("\n")
        try:
            score = int(lines[-3].split(":")[-1].strip())
            scores_total += score
            num_games += 1
        except Exception as e:
            print("Error")
            print(e)
            continue
    return scores_total / num_games


def evaluate_weight_set(weight_set):
    life_string = "#define LIFE_FACTOR {}"
    score_string = "#define SCORE_FACTOR {}"
    distance_string = "#define DISTANCE_FACTOR {}"

    # Run evaluation using provided engine
    os.system("cp minimax.c minimax-save.c")

    os.system("sed 's/{}/{}/g' minimax_template.c > minimax.c"
              .format(life_string, life_string.format(weight_set[0])))

    os.system("sed -i 's/{}/{}/g' minimax.c"
              .format(score_string, score_string.format(weight_set[1])))

    os.system("sed -i 's/{}/{}/g' minimax.c"
              .format(distance_string, distance_string.format(weight_set[2])))

    os.system("/bin/bash ./run_simulations.sh")

    os.system("mv minimax-save.c minimax.c")
    score = extract_evaluation_scores()
    print("Weight set: {} -> Score: {}".format(weight_set, score))
    return score

###


def create_starting_population(individuals, chromosome_length):
    # Set up an initial array of all zeros
    population = []
    # Loop through each row (individual)
    for i in range(individuals):
        p = []
        for i in range(chromosome_length):
            p.append(random.randint(0, 100))
        population.append(p)
    return np.array(population)


def calculate_fitness(population):
    # Create an array of True/False compared to reference
    scores = []
    for i, p in enumerate(population):
        print("Calculating population member:", i)
        score = 366 - evaluate_weight_set(p)
        scores.append(score)
    return scores


def select_individual_by_tournament(population, scores):
    # Get population size
    population_size = len(scores)

    # Pick individuals for tournament
    fighter_1 = random.randint(0, population_size - 1)
    fighter_2 = random.randint(0, population_size - 1)

    # Get fitness score for each
    fighter_1_fitness = scores[fighter_1]
    fighter_2_fitness = scores[fighter_2]

    # Identify undividual with highest fitness
    # Fighter 1 will win if score are equal
    if fighter_1_fitness >= fighter_2_fitness:
        winner = fighter_1
    else:
        winner = fighter_2

    # Return the chromsome of the winner
    return population[winner, :]


def breed_by_crossover(parent_1, parent_2):
    # Get length of chromosome
    chromosome_length = len(parent_1)

    # Pick crossover point, avoding ends of chromsome
    crossover_point = random.randint(1, chromosome_length - 1)

    # Create children. np.hstack joins two arrays
    child_1 = np.hstack((parent_1[0:crossover_point],
                         parent_2[crossover_point:]))

    child_2 = np.hstack((parent_2[0:crossover_point],
                         parent_1[crossover_point:]))

    # Return children
    return child_1, child_2


def randomly_mutate_population(population, mutation_probability):
    # Apply random mutation
    random_mutation_array = np.random.random(
        size=(population.shape))

    random_mutation_boolean = \
        random_mutation_array <= mutation_probability

    population[random_mutation_boolean] = \
        np.logical_not(population[random_mutation_boolean])

    # Return mutation population
    return population


# *************************************
# ******** MAIN ALGORITHM CODE ********
# *************************************

# Set general parameters
chromosome_length = 3  # Number of weights
population_size = 8
maximum_generation = 40
best_score_progress = []  # Tracks prpythogress

# Create reference solution
# (this is used just to illustrate GAs)
# Create starting population
population = create_starting_population(population_size, chromosome_length)
# population = np.array([[40, 66, 40], [32, 66, 30], [40, 66, 30], [40, 66, 30], [32, 66, 40], [32, 66, 30], [32, 66, 40], [32, 66, 30]])
# population = np.array([[45, 64, 86], [49, 96, 68], [49, 81, 87], [92, 83, 60], [45, 64, 68], [36, 64, 68], [36, 64, 68], [45, 64, 68]])
# Display best score in starting population
scores = calculate_fitness(population)
best_score = np.max(scores)
best_index = np.where(scores == best_score)
print('Starting best score: ', best_score)
print("Best weight: {}".format(population[best_index]))
with open("train.log", "a") as f:
    f.write("Generation 0 best score: {}\n".format(0, best_score))
    f.write("Weight set: {}\n".format(json.dumps(population.tolist())))
    f.write("Best weight: {}\n".format(population[best_index].tolist()))

# Add starting best score to progress tracker
best_score_progress.append(best_score)

# Now we'll go through the generations of genetic algorithm
for generation in range(maximum_generation):
    print("Processing Generation: ", generation)
    # Create an empty list for new population
    new_population = []

    # Create new popualtion generating two children at a time
    for i in range(int(population_size / 2)):
        parent_1 = select_individual_by_tournament(population, scores)
        parent_2 = select_individual_by_tournament(population, scores)
        child_1, child_2 = breed_by_crossover(parent_1, parent_2)
        new_population.append(child_1)
        new_population.append(child_2)

    # Replace the old population with the new one
    population = np.array(new_population)

    # Apply mutation
    mutation_rate = 0.002
    population = randomly_mutate_population(population, mutation_rate)

    # Score best solution, and add to tracker
    scores = calculate_fitness(population)
    best_score = np.max(scores)
    best_index = np.where(scores == best_score)
    best_score_progress.append(best_score)
    with open("train.log", "a") as f:
        f.write("Generation {} best score: {}\n".format(generation, best_score))
        f.write("Weight set: {}\n".format(json.dumps(population.tolist())))
        f.write("Best weight: {}\n".format(population[best_index].tolist()))
    print("Generation best score:", best_score)
    best_index = np.where(scores == best_score)
    print("Best weight: {}".format(population[best_index]))
    print("Weight sets:", population.tolist())
    print("Scores:", scores)

# GA has completed required generation
print('End best score: ', best_score)
print(population[0])
with open("train.log", "a") as f:
    f.write("Overall best score: {}\n".format(best_score))
    f.write("Weight set: {}\n".format(json.dumps(population)))
