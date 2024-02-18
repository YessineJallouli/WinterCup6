import contextlib
import subprocess

import logs
import os
import platform
from multiprocessing import Process

old_path = os.getcwd()
captured_files = set()

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
    logs.logger.log(f"Script exited with code {result.returncode}", "WARNING" if result.returncode != 0 else "INFO")
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


def mark_scripts_as_executable(dir):
    for root,dirs,files in os.walk(dir):
        for file in files:
            if file.endswith(".sh"):
                os.chmod(os.path.join(root,file), 0o754)
        for dir in dirs:
            mark_scripts_as_executable(os.path.join(root,dir))

class Script:
    def __init__(self, name, script):
        self.name = name
        self.script = script

    def run(self, working_dir=None):
        logs.logger.log(f"Running script {self.name}")
        return run_script(self.script, working_dir=working_dir)

    def __str__(self):
        return self.name

def script_system_name(script):
    import platform
    plt = platform.system()

    if plt == "Windows":
        return script + ".bat"
    else:
        return script + ".sh"