#!/usr/bin/env python
import subprocess

import contextlib
import subprocess

import os
import platform
import sys
from multiprocessing import Process

old_path = os.getcwd()
captured_files = set()

import abc
import contextlib
import os


class Logger(abc.ABC):
    @abc.abstractmethod
    def log(self, message, log_level="INFO"):
        pass


class ConsoleLogger(Logger):
    def log(self, message, log_level="INFO"):
        print("[" + log_level + "]: " + message)


class FileLogger(Logger):
    def __init__(self, file_name):
        self.file_name = file_name

    def log(self, message, log_level="INFO"):
        with open(self.file_name, "a") as file:
            file.write("[" + log_level + "]: " + message + "\n")


class NullLogger(Logger):
    def log(self, message, log_level="INFO"):
        pass


logger = NullLogger()
MAX_ERROR_MESSAGE_LENGTH = 100


def init_logger():
    global logger
    logger = ConsoleLogger()
    return logger


def set_logger(new_logger):
    global logger
    logger = new_logger


@contextlib.contextmanager
def change_directory(x, create=False, stack=None):
    if stack is None:
        stack = []
    d = os.getcwd()
    stack.append(d)

    # This could raise an exception, but it's probably
    # best to let it propagate and let the caller
    # deal with it, since they requested x
    if create:
        os.makedirs(x, exist_ok=True)
    os.chdir(x)

    try:
        yield

    finally:
        # This could also raise an exception, but you *really*
        # aren't equipped to figure out what went wrong if the
        # old working directory can't be restored.
        os.chdir(d)
        stack.pop()


class BidirectionalPipe:
    DR = 0
    DW = 1
    RR = 2
    RW = 3
    DIRECT_READ = 0
    DIRECT_WRITE = 1
    REVERSE_READ = 2
    REVERSE_WRITE = 3

    def __init__(self):
        self.pipes = os.pipe(), os.pipe()
        self.pipe_files = [os.fdopen(self.pipes[0][0], "r"), os.fdopen(self.pipes[0][1], "w"),
                             os.fdopen(self.pipes[1][0], "r"), os.fdopen(self.pipes[1][1], "w")]

    def __getitem__(self, item):
        return self.pipe_files[item]


class Executable:
    def __init__(self, name, executable, input_binding, output_binding, err_binding=sys.stderr):
        self.name = name
        self.executable = executable
        self.input_binding = input_binding
        self.output_binding = output_binding
        self.err_binding = err_binding
        self.process = None

    def run(self, *args, working_dir=None):
        logger.log("Running script {}".format(self.name))
        path = os.path.join(os.getcwd(), self.executable)
        self.process = subprocess.Popen([path, *args],
                                        stdin=self.input_binding,
                                        stdout=self.output_binding,
                                        stderr=self.err_binding)
        return self.process

    def poll(self):
        if self.process is None:
            raise ValueError("Process was not started")
        return self.process.poll()

    def wait(self):
        if self.process is None:
            raise ValueError("Process was not started")
        return self.process.wait()

    def __str__(self):
        return self.name


def split_arguments(args, sep="@"):
    L = []
    L.append([])
    for arg in args:
        if arg == sep:
            L.append([])
        else:
            L[-1].append(arg)
    if len(L) != 3:
        raise RuntimeError("Usage: <testfile> @ <interaction_script> @ <solution_script>")
    return L


def main(args):
    global logger
    logger = init_logger()



    if os.getenv("NDEBUG") is not None:
        os.unsetenv("DEBUG")
    elif os.getenv("DEBUG") is None:
        os.putenv("DEBUG", "interaction.txt")

    if os.path.exists("interaction.txt"):
        os.remove("interaction.txt")


    testfile, interaction_script, solution_script = split_arguments(args, sep="@")
    if len(testfile) != 1:
        raise ValueError("Exactly one test file must be specified")
    interactor_path, *interactor_args = interaction_script
    interactor_args = interactor_args + testfile
    solution_path, *solution_args = solution_script
    pipe = BidirectionalPipe()
    interactor = Executable("Interactor", interactor_path, pipe[BidirectionalPipe.DR], pipe[BidirectionalPipe.RW])
    solution = Executable("Solution", solution_path, pipe[BidirectionalPipe.RR], pipe[BidirectionalPipe.DW])
    interactor.run(*interactor_args)
    solution.run(*solution_args)
    interactor.wait()
    solution.wait()


if __name__ == "__main__":
    main(sys.argv[1:])
