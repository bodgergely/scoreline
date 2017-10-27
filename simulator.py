from simulate_progression import simulate_progression_data
from simulate_progression import calculate_wins_from_behind

class InputData:
    def __init__(self, La, Lb, Pa, Pb):
        self.La = La
        self.Lb = Lb
        self.Pa = Pa
        self.Pb = Pb


class Simulator:
    def __init__(self, num_simulations=10000000):
        self.num_simulations = num_simulations
    def __call__(self, data):
        data = InputData(data[0],data[1], data[2], data[3])
        progression_data = simulate_progression_data(data.La, data.Lb, self.num_simulations) 
        Pa, Pb = calculate_wins_from_behind(progression_data) 
        return (Pa, Pb)

