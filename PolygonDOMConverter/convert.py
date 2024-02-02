import datetime
import os
import time
import typing

import yaml
import logging
from itertools import cycle
import shutil

import scripts
from contest import Contest
from problem import Problem, retag

PALETTE = ["#9486EA", "#E93603", "#9B630C"]


def prepare_problemset_yaml(path: str, contest: Contest):
    problemset = []
    for problem, color in zip(contest.problems, cycle(PALETTE)):
        problemset.append({"color": color,
                           "short-name": problem.short_name,
                           "rgb": color
                           })
    with open(path, "w") as file:
        yaml.dump({"problemset": problemset}, stream=file)


def prepare_problems_yaml(path: str, contest: Contest):
    problems = []
    for problem, color in zip(contest.problems, cycle(PALETTE)):
        problems.append({"id": problem.short_name,
                         "label": problem.index,
                         "rgb": color,
                         "time_limit": problem.time_limit
                         })
    problems.sort(key=lambda x: x["label"])
    for k, problem in enumerate(problems):
        problem["ordinal"] = k + 1
    with open(path, 'w') as file:
        yaml.dump(problems, file)


def current_time_iso_8601():
    return time.strftime("%Y-%m-%dT%H:%M:%S", time.localtime())


def prepare_contest_yaml(path: str, contest: Contest):
    contest_info = {"duration": "5:00:00",
                    "name": contest.name,
                    "penalty-time": 20,
                    "scoreboard-freeze-length": "1:00:00",
                    "short-name": f"contest-{contest.index}",
                    "start-time": current_time_iso_8601()
                    }
    with open(path, "w") as file:
        yaml.dump(contest_info, file)


def prepare_problem_directory(problem: Problem):
    os.makedirs(problem.short_name, exist_ok=True)
    os.makedirs(os.path.join(problem.short_name, "problem_statement"),exist_ok=True)
    os.makedirs(os.path.join(problem.short_name, "data/secret"),exist_ok=True)
    os.makedirs(os.path.join(problem.short_name, "data/sample"),exist_ok=True)
    os.makedirs(os.path.join(problem.short_name, "submissions"),exist_ok=True)
    if problem.checker_file is not None or problem.interactor_file is not None:
        os.makedirs(os.path.join(problem.short_name, "output_validators","validator"),exist_ok=True)
    if problem.validator_file is not None:
        os.makedirs(os.path.join(problem.short_name, "input_validators","validator"),exist_ok=True)
    pass


def ignore_answer_extension(x: str):
    if x[-2:] == ".a":
        return x[:-2]
    else:
        return x


def prepare_problem_yaml(path: str, contest_directory: str, problem: Problem):
    with open(path, 'w') as file:
        problem_info = {
            "author": problem.author,
            "uuid": problem.index,
            "limits": {
                "memory": problem.memory_limit,
                "time": problem.time_limit
            }
        }
        if problem.interactor_file is not None:
            problem_info["validation"] = "custom interactive"
        elif problem.checker_file is not None:
            problem_info["validation"] = "custom"
        yaml.dump(problem_info,file)


def copy_test_files(contest_directory: str, problem: Problem):
    path = os.path.join(contest_directory,"problems", problem.short_name, "tests")
    dirs = os.listdir(path)
    dirs.sort(key=ignore_answer_extension)
    n = len(dirs)
    k = 1
    test_pairs = []
    index = 0
    while k < n:
        if ignore_answer_extension(dirs[k]) == ignore_answer_extension(dirs[k - 1]):
            if ignore_answer_extension(dirs[k]) == k:
                test_pairs.append((dirs[k], dirs[k - 1]))
            else:
                test_pairs.append((dirs[k - 1], dirs[k]))
        else:
            logging.logger.log("Files mismatch. Ignoring current pair", "WARNING")
            k += 1
            continue
        test_in, test_ans = test_pairs[-1]
        dst = "sample" if index in problem.samples else "secret"
        shutil.copy(os.path.join(path, test_in), os.path.join(problem.short_name,"data", dst, f"{test_in}.in"))
        shutil.copy(os.path.join(path, test_ans), os.path.join(problem.short_name,"data", dst, f"{test_in}.ans"))
        k += 2
        index+=1


def copy_submission_files(contest_directory: str, problem: Problem):
    for solution in problem.solution_files:
        path = os.path.join(problem.short_name,"submissions", retag(solution.tag))
        os.makedirs(path, exist_ok=True)
        shutil.copy(os.path.join(contest_directory,"problems", problem.short_name, solution.path), path)
    pass

def copy_custom_files(contest_directory:str, problem:Problem):
    if problem.checker_file is not None:
        shutil.copy(os.path.join(contest_directory,"problems", problem.short_name, problem.checker_file), os.path.join(problem.short_name,"output_validators"))
    if problem.interactor_file is not None:
        shutil.copy(os.path.join(contest_directory,"problems", problem.short_name, problem.interactor_file),
                    os.path.join(problem.short_name,"output_validators","validator"))
    if problem.validator_file is not None:
        shutil.copy(os.path.join(contest_directory,"problems", problem.short_name, problem.validator_file),
                    os.path.join(problem.short_name,"input_validators", "validator"))


def copy_problem_statements(contest_directory: str, problem: Problem, preferred_type: str):
    by_type = {}
    for statement in problem.statements:
        by_type[statement.statement_type] = statement
    if preferred_type in by_type:
        statement = by_type[preferred_type]
    else:
        statement = next(iter(by_type))
    shutil.copy(os.path.join(contest_directory,"problems", problem.short_name, statement.path), os.path.join(problem.short_name,"problem_statement"))
    shutil.copy(os.path.join(contest_directory,"problems", problem.short_name, statement.path), problem.short_name)


def prepare_contest_archive(contest_directory: str, working_directory: str, contest: Contest, preferred_type="pdf"):
    with scripts.change_directory(working_directory):
        prepare_problemset_yaml("problemset.yaml", contest)
        prepare_problems_yaml("problems.yaml", contest)
        prepare_contest_yaml("contest.yaml", contest)
        for problem in contest.problems:
            logging.logger.log(f"Processing problem {problem.short_name}")
            prepare_problem_directory(problem)
            logging.logger.log("\tCreating problem YML file")
            prepare_problem_yaml(os.path.join(problem.short_name,"problem.yaml"), contest_directory, problem)
            logging.logger.log("\tCopying test files")
            copy_test_files(contest_directory, problem)
            logging.logger.log("\tCopying submission files")
            copy_submission_files(contest_directory, problem)
            logging.logger.log("\tCopying problem statements")
            copy_problem_statements(contest_directory, problem, preferred_type=preferred_type)
            logging.logger.log("\tCopying custom files")
            copy_custom_files(contest_directory, problem)
            logging.logger.log("\tCreating DOMJudge-related files")
            with open(os.path.join(problem.short_name,".timelimit"),'w') as file:
                file.write(f"{problem.time_limit}")
            with open(os.path.join(problem.short_name,"domjudge-problem.ini"),'w') as file:
                file.write(f"""name={problem.full_name}
timelimit={problem.time_limit}""")
                pass
