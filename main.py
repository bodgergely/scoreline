import sys
import input_processor
import simulator
import parser
from client import RabbitMQClient


def callback(Pa_win_from_behind, Pb_win_from_behind):
    print("Prob A win behind: {}\nProb B win behind: {}\n".format(Pa_win_from_behind, Pb_win_from_behind))

def process_from_file(filepath, simulations):
    sim = simulator.Simulator(num_simulations=simulations)
    fir = input_processor.FileInputProcessor(filepath, parser.csv_parse, sim)
    fir.process(callback)


def networked_process(simulations, queue_name, host):
    sim = simulator.Simulator(num_simulations=simulations)
    nir = input_processor.RabbitMQProcessor(queue_name, host, parser.csv_parse, sim)
    nir.process(callback)


def is_networked(args):
    for i in args:
        if i == '--networked':
            return True
    return False


if __name__ == "__main__":
    networked = False
    if not is_networked(sys.argv):
        filepath = sys.argv[1]
        simulations = int(sys.argv[2])
        process_from_file(filepath, simulations)
    else:
        simulations = int(sys.argv[1])
        queue_name = sys.argv[2]
        networked_process(simulations, queue_name, 'localhost')
