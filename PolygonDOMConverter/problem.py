import logging
import xml
from typing import List
from xml.etree import ElementTree

import scripts

def retag(tag:str)->str:
    return tag.replace("-","_").lower().replace("main","accepted")


class SolutionFile:
    def __init__(self, path, tag):
        self.path = path
        self.tag = tag

class SubmissionDescription:
    def __init__(self, description_file):
        self.description_file = description_file
        with open(description_file, "r") as file:
            elements = file.readlines()
            map = {}
            for element in elements:
                key, value = element.split(":", 2)
                map[key] = value
            self.tag = map["Tag"]
            self.author = map["Author"]
            self.file = map["File name"]


class Statement:
    def __init__(self, path, statement_type):
        self.path = path
        for known_type in ["html","pdf","markdown","tex"]:
            if known_type in statement_type:
                logging.logger.log(f"\t\tStatement type: {known_type}")
                self.statement_type = known_type
                break
        else:
            raise Exception("Unknown statement type")

def convert_to_mb(value)->int:
    return value//1024**2

def convert_to_seconds(value)->float:
    return value/1000

class Problem:
    TIME_LIMIT_DEFAULT=1
    MEMORY_LIMIT_DEFAULT=512
    def __init__(self, index, short_name, full_name, statements: List[Statement], *, solution_files=None,checker_file=None,
                 validator_file=None, interactor_file=None,samples=None,time_limit=TIME_LIMIT_DEFAULT,
                 memory_limit=MEMORY_LIMIT_DEFAULT, author:str=None):
        if samples is None:
            samples=set()
        if solution_files is None:
            solution_files=[]
        self.full_name = full_name
        self.short_name = short_name
        self.index = index
        self.statements = statements
        self.checker_file = checker_file
        self.validator_file = validator_file
        self.interactor_file = interactor_file
        self.tests_script= scripts.Script(f"{self.short_name} generator",scripts.script_system_name("doall"))
        self.samples=samples
        self.time_limit=time_limit
        self.memory_limit=memory_limit
        self.author=author
        self.solution_files=solution_files

    @staticmethod
    def from_xml(file, index):
        if isinstance(file, str):
            file = open(file, "r")
        parsed = ElementTree.parse(file)
        root = parsed.getroot()
        author=root.attrib["url"].split('/')[-2]
        short_name = root.attrib["short-name"]
        full_name = parsed.find("names").find("name").attrib["value"]
        logging.logger.log(f"\tProblem Full Name: {full_name}")
        statements = []
        for statement in root.find("statements").findall("statement"):
            path = statement.attrib["path"]
            statement_type = statement.attrib["type"]
            statements.append(Statement(path, statement_type))
        assets=root.find("assets")
        checker_file = assets.find("checker")
        # TODO: Use monad
        if checker_file is not None:
            checker_file = checker_file.find("source")
            if checker_file is not None:
                checker_file = checker_file.attrib["path"]
                logging.logger.log(f"\tChecker file: {checker_file}")
        validator_file = assets.find("validator")
        if validator_file is not None:
            validator_file = validator_file.find("source")
            if validator_file is not None:
                validator_file = validator_file.attrib["path"]
                logging.logger.log(f"\tValidator file: {validator_file}")
        interactor_file = assets.find("interactor")
        if interactor_file is not None:
            interactor_file = interactor_file.find("source")
            if interactor_file is not None:
                interactor_file = interactor_file.attrib["path"]
                logging.logger.log(f"\tInteractor file: {interactor_file}")
        tests=root.find("judging").find("testset")
        time_limit=convert_to_seconds(int(tests.find("time-limit").text))
        memory_limit=convert_to_mb(int(tests.find("memory-limit").text))
        logging.logger.log(f"\tTime limit: {time_limit} seconds")
        logging.logger.log(f"\tMemory limit: {memory_limit} MB")
        samples=set()
        for index_,test in enumerate(tests.find("tests").findall("test")):
            if "sample" in test.attrib:
                samples.add(index_)

        solution_files=[]
        for solution in assets.find("solutions").findall("solution"):
            tag=solution.attrib["tag"]
            file=solution.find("source").attrib["path"]
            solution_files.append(SolutionFile(path=file,tag=tag))

        return Problem(index, short_name, full_name, statements, checker_file=checker_file,
                       validator_file=validator_file, interactor_file=interactor_file,solution_files=solution_files,
                       time_limit=time_limit,memory_limit=memory_limit,samples=samples,author=author)

    def __repr__(self):
        return self.full_name
