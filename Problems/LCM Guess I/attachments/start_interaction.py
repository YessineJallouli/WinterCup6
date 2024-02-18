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

@contextlib.contextmanager
def change_directory(x,create=False,stack=None):
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

def run_script_impl(script,*args):
    path=os.path.join(os.getcwd(),script)
    result = subprocess.Popen([path,*args],
                               stderr=subprocess.STDOUT)
    result.wait()
    logger.log(f"Script exited with code {result.returncode}", "WARNING" if result.returncode != 0 else "INFO")
    return result.returncode

def run_script(script, *args,working_dir=None):
    """
    Runs a script in a separate process.

    :param script: The script to run.
    :param working_dir: The working directory to run the script in.
    :return: The process object.
    """
    if working_dir is None:
        return run_script_impl(script,*args)
    with change_directory(working_dir, create=False):
        return run_script_impl(script,*args)


class Executable:
    def __init__(self, name, executable, input_binding, output_binding , err_binding=subprocess.STDOUT):
        self.name = name
        self.executable = executable
        self.input_binding = input_binding
        self.output_binding = output_binding
        self.err_binding = err_binding

    def run(self,*args, working_dir=None):
        logger.log(f"Running script {self.name}")
        path = os.path.join(os.getcwd(), self.executable)
        result = subprocess.Popen([path, *args],
                                  stderr=subprocess.STDOUT)
        result.wait()
        logger.log(f"Script exited with code {result.returncode}", "WARNING" if result.returncode != 0 else "INFO")
        return result.returncode

    def __str__(self):
        return self.name

def split_arguments(args, sep="@"):
    L=[]
    L.append([])
    for arg in args:
        if arg==sep:
            L.append([])
        else:
            L[-1].append(arg)
    return L

def main(args):
    global logger
    logger = init_logger()
    testfile, interaction_script, solution_script=  split_arguments(args,sep="@")
    if len(testfile) != 1:
        raise ValueError("Exactly one test file must be specified")
    testfile = testfile[0]
    subprocess.PIPE
if __name__ == "__main__":
    main(sys.argv[1:])