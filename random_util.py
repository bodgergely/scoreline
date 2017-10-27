
import numpy as np

def random_uniform():
    return np.random.rand()

def random_exponential(Lambda):
    return (-1.0 * np.log(random_uniform())) / float(Lambda)
    
