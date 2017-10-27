
import pika
import sys


class RabbitMQClient:
    def __init__(self, source_file, host, queue_name):
        self.host = host
        self.queue_name = queue_name
        self.source_file = source_file
        self.connection = pika.BlockingConnection(pika.ConnectionParameters(self.host))
        self.channel = self.connection.channel()
        self.channel.queue_declare(queue=queue_name)
    def start(self):
        with open(self.source_file, 'r') as source:
            for line in source:
                self.channel.basic_publish(exchange='',
                                            routing_key=self.queue_name,
                                            body=line)
        self.connection.close()


def start_produce(filepath, queue_name, host):
    client = RabbitMQClient(filepath, host, queue_name)
    client.start()



if __name__ == "__main__":
    start_produce(sys.argv[1], sys.argv[2], 'localhost')
