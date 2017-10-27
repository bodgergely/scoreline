import numpy as np
from random_util import random_exponential, random_uniform


def simulate_progression(double Lambda_a, double Lambda_b):
    cdef double T
    cdef int score_a
    cdef int score_b
    T = 0.0
    score_a = 0
    score_b = 0
    progression = [(score_a, score_b)]

    while True:
        T = T + random_exponential(Lambda_a + Lambda_b)
        if T > 1 or score_a + score_b > 20:
            return progression
        if random_uniform() < Lambda_a / (Lambda_a + Lambda_b):
            score_a += 1
        else:
            score_b += 1
        progression.append((score_a, score_b))

def simulate_progression_data(double Lambda_a,double Lambda_b,int num_simulations=10000000):
    progressions = []
    for i in range(num_simulations):
        progression = simulate_progression(Lambda_a, Lambda_b)
        progressions.append(progression)
    return progressions


def win_from_behind(progressions):
    losing = [False, False]
    for score_a, score_b in progressions:
        if not losing[1] and score_b < score_a:
            losing[1] = True
        elif not losing[0] and score_a < score_b:
            losing[0] = True
    last_score_a, last_score_b = progressions[len(progressions)-1]
    if last_score_a > last_score_b and losing[0]:
        return (1,0)
    elif last_score_b > last_score_a and losing[1]:
        return (0,1)
    else:
        return (0,0)


def calculate_wins_from_behind(progression_data):
    wins = [0,0]
    num_sims = len(progression_data)
    for progression in progression_data:
        win_a, win_b = win_from_behind(progression)  
        wins[0] += win_a
        wins[1] += win_b
    return (wins[0]/float(num_sims) , wins[1]/float(num_sims))


