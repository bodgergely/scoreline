
import logging
from parser import ParseError
import pika

class InputProcessor:
    def __init__(self,data_parser, simulator):
        self.data_parser = data_parser
        self.simulator = simulator
    def process(self, callback):
        raise NotImplementedError

class FileInputProcessor(InputProcessor):
    def __init__(self, filepath, data_parser, simulator):
        super().__init__(data_parser, simulator)
        self.filepath = filepath
    def process(self, callback):
        with open(self.filepath, 'r') as inputfile:
            i = 0
            for line in inputfile:
                try:
                    data = self.data_parser(line)
                    if data != None:
                        Pa, Pb = self.simulator(data)
                        callback(Pa, Pb)
                except ParseError:
                    logging.critical("Failed to parse line num{}: {}, ", i, line)                 
                i+=1
        
class RabbitMQProcessor(InputProcessor):
    def __init__(self, queue_name, host, data_parser, simulator):
        self.processed = 0
        super().__init__(data_parser, simulator)
        self.queue_name = queue_name
        self.host = host
    def process(self, callback):
        self.callback = callback
        self._setup_connection()
       
    def _process_line(self, channel, method, properties, line):
        line = str(line)[2:-3]
        try:
            data = self.data_parser(line)
            if data != None:
                Pa, Pb = self.simulator(data)
                self.callback(Pa,Pb)
        except ParseError:
            logging.critical("Failed to parse line num{}: {}, ", self.processed, line)                 
        self.processed += 1

    def _setup_connection(self):
        self.connection = pika.BlockingConnection(pika.ConnectionParameters(self.host))
        self.channel = self.connection.channel()
        self.channel.basic_consume(self._process_line, queue=self.queue_name, no_ack=True)
        print('Waiting for messages.')
        self.channel.start_consuming()
 
