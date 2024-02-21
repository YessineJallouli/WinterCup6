#!/usr/bin/env python3
import atexit
import subprocess
import pathlib

import os
import sys
import tempfile

import abc
import contextlib
import os

old_path = os.getcwd()
captured_files = set()


class Logger(abc.ABC):
    """
    Abstract class to define the interface for a logger
    """
    @abc.abstractmethod
    def log(self, message, log_level="INFO"):
        pass


class ConsoleLogger(Logger):
    """
    Logger that prints the messages to the console
    """
    def log(self, message, log_level="INFO"):
        print("[" + log_level + "]: " + message)


class FileLogger(Logger):
    """
    Logger that writes the messages to a file
    """
    def __init__(self, file_name):
        self.file_name = file_name

    def log(self, message, log_level="INFO"):
        with open(self.file_name, "a") as file:
            file.write("[" + log_level + "]: " + message + "\n")


class NullLogger(Logger):
    """
    Null logger that does nothing
    """
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


class BidirectionalPipe:
    """
    Class to manage a bidirectional pipe. It is used to communicate between two processes.
    """
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
    """
    Wrapper for subprocess.Popen to provide a more intuitive interface. It also allows for easy redirection of the
    standard streams.
    """

    def __init__(self, name, executable, input_binding, output_binding, err_binding=sys.stderr):
        """

        :param name: Name or identifier of the process
        :param executable: Path to the executable
        :param input_binding: Stream to be used as input
        :param output_binding: Stream to be used as output
        :param err_binding: Stream to be used as error output
        """
        self.name = name
        self.executable = executable
        self.input_binding = input_binding
        self.output_binding = output_binding
        self.err_binding = err_binding
        self.process = None

    def run(self, *args):
        """
        Starts the process with the given arguments
        :param args: Arguments to be passed to the process
        :return: The process object
        """
        self.process = subprocess.Popen([self.executable, *args],
                                        stdin=self.input_binding,
                                        stdout=self.output_binding,
                                        stderr=self.err_binding)
        return self.process

    def poll(self):
        """
        Checks if the process has finished
        :return: The process completion metadata
        """
        if self.process is None:
            raise ValueError("Process was not started")
        return self.process.poll()

    def wait(self, timeout=None):
        """
        Waits for the process to finish. If a timeout is provided, it will throw a TimeoutExpired exception if the
        process does not finish in time.
        :param timeout: Maximum time to wait
        :return: The process completion metadata
        """
        if self.process is None:
            raise ValueError("Process was not started")
        return self.process.wait(timeout)

    def kill(self):
        """
        Kills the process
        :return: The process completion metadata
        """
        if self.process is None:
            raise ValueError("Process was not started")
        return self.process.kill()

    def __str__(self):
        return self.name


def split_arguments(args, sep="@"):
    """
    Splits the arguments into three groups using the provided separator
    :param args: Arguments to be split
    :param sep: Separator
    :return: A list with three elements
    """
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


def remove_directory_recursive(directory):
    """
    Removes a directory and all its contents
    :param directory: Directory to be removed
    """
    for root, dirs, files in os.walk(directory, topdown=False):
        for name in files:
            os.remove(os.path.join(root, name))
        for name in dirs:
            os.rmdir(os.path.join(root, name))
    os.rmdir(directory)


def remove_resources(resource):
    """
    Removes a resource (file or directory)
    :param resource: Resource to be removed
    """
    if os.path.exists(resource):
        if os.path.isdir(resource):
            remove_directory_recursive(resource)
        else:
            os.remove(resource)


class TemporaryResources:
    """
    Class to manage temporary resources. It will remove all the resources when the program finishes.
    """

    def __init__(self):
        atexit.register(self.clean)
        self.resources = []

    def append(self, resource):
        self.resources.append(resource)

    def clean(self):
        for resource in self.resources:
            remove_resources(resource)


def process_testfile_argument(testfile, cleanup=None):
    """
    Processes the testfile argument. If it is empty or a dash, it will create a temporary file with the provided input.
    :param testfile: Testfile argument
    :param cleanup: If set to True, it will remove the temporary file when the program finishes. If it is a
    TemporaryResources object, it will be tracked by it. Otherwise, nothing will
    be done.
    :return:
    """
    if len(testfile) > 1:
        raise ValueError("At most one testfile can be provided. Got: {}".format(len(testfile)))

    testfile = testfile[0] if len(testfile) > 0 else "-"
    if testfile == "-":
        tmpdir = tempfile.mkdtemp()
        if cleanup:
            atexit.register(remove_directory_recursive, tmpdir)
        elif isinstance(cleanup, TemporaryResources):
            cleanup.append(tmpdir)
        testfile = os.path.join(tmpdir, "test.in")
        with open(testfile, "w") as file:
            file.write(input())
            file.write("\n")
            file.write(input())
            file.flush()
    return testfile


def main(args):
    # Initiate the logger
    global logger
    logger = init_logger()

    # Setup the environment variables
    if os.getenv("NDEBUG") is not None:
        os.unsetenv("DEBUG")
    elif os.getenv("DEBUG") is None:
        os.putenv("DEBUG", "interaction.txt")

    if os.getenv("TIME_LIMIT") is not None:
        time_limit = int(os.getenv("TIME_LIMIT"))
    else:
        time_limit = 3

    # Remove the interaction file if it exists
    interaction_path = os.getenv("DEBUG") or "interaction.txt"
    if os.path.exists(interaction_path):
        os.remove(interaction_path)

    # Create a temporary resource manager
    tmpResources = TemporaryResources()
    # Split the arguments
    testfile, interaction_script, solution_script = split_arguments(args, sep="@")
    # Check if the testfile is stdin
    is_stdin = "-" in testfile or len(testfile) == 0
    # Get the paths and arguments
    interactor_path, *interactor_args = interaction_script
    solution_path, *solution_args = solution_script
    if len(testfile) == 0:
        logger.log("No testfile provided. Defaulting to STDIN", "WARNING")
        logger.log("Use dash '-' instead for this same behaviour.", "WARNING")
    logger.log("Starting Interaction with the following parameters:", "INFO")
    logger.log("\tTest File: {}".format("STDIN" if is_stdin else testfile[0]), "INFO")
    logger.log("\tInteractor: {}".format(interaction_path), "INFO")
    logger.log("\tSolution: {}".format(solution_path), "INFO")
    logger.log("\tTime Limit: {} seconds".format(time_limit), "INFO")
    # Process the testfile argument
    testfile = process_testfile_argument(testfile, cleanup=tmpResources)
    interactor_args.append(testfile)
    # Create the bidirectional pipe
    pipe = BidirectionalPipe()
    interactor = Executable("Interactor", interactor_path, pipe[BidirectionalPipe.DR], pipe[BidirectionalPipe.RW])
    solution = Executable("Solution", solution_path, pipe[BidirectionalPipe.RR], pipe[BidirectionalPipe.DW])
    # Run the interactor and the solution
    interactor.run(*interactor_args)
    solution.run(*solution_args)
    try:
        # Wait for both the interactor and solution to finish
        interactor.wait(timeout=time_limit)
        solution.wait(timeout=time_limit)
    except subprocess.TimeoutExpired:
        # If the time limit is exceeded, log the error and kill the processes
        logger.log("Time limit exceeded ({} seconds)".format(time_limit), "ERROR")
        interactor.kill()
        solution.kill()


if __name__ == "__main__":
    main(sys.argv[1:])
