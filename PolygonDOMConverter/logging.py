import abc
import contextlib
import os


class Logger(abc.ABC):
    @abc.abstractmethod
    def log(self, message,log_level="INFO"):
        pass

class ConsoleLogger(Logger):
    def log(self, message,log_level="INFO"):
        print("["+log_level + "]: " + message)


class FileLogger(Logger):
    def __init__(self, file_name):
        self.file_name = file_name

    def log(self, message,log_level="INFO"):
        with open(self.file_name, "a") as file:
            file.write("["+log_level + "]: " + message + "\n")

class NullLogger(Logger):
    def log(self, message,log_level="INFO"):
        pass


logger : Logger
MAX_ERROR_MESSAGE_LENGTH = 100
def init_logger():
    global logger
    logger = ConsoleLogger()
    return logger

def set_logger(new_logger):
    global logger
    logger = new_logger

