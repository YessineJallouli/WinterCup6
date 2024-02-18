import os
from typing import List
from xml.etree import ElementTree

import logs
from problem import Problem

def parse_contest_file(file):
    parsed=ElementTree.parse(file)


class Contest:
    def __init__(self, index, name, problems: List[Problem]):
        self.index = index
        self.name = name
        self.problems = problems

    @staticmethod
    def from_xml(file):
        if isinstance(file, str):
            file = open(file, "r")
        parsed = ElementTree.parse(file)
        root = parsed.getroot()
        contest_index = int(root.attrib["url"].split("/")[-1])
        logs.logger.log(f"Contest ID: {contest_index}")
        name = root.find("names").find("name").attrib["value"]
        logs.logger.log(f"Contest Name: {name}")
        problems = []
        directories=set(os.listdir("problems"))
        problems_list=root.find("problems")
        for problem in problems_list.findall("problem"):
            try:
                url=problem.attrib["url"]
                index=problem.attrib["index"].upper()
                short_name=url.split("/")[-1]
                logs.logger.log(f"Processing problem {index}: {short_name}")
                if short_name not in directories:
                    logs.logger.log(f"Problem {short_name} not found",log_level="ERROR")
                    continue
                problem_desc = os.path.join("problems",short_name,"problem.xml")
                problems.append(Problem.from_xml(problem_desc,index))
            except Exception as e:
                logs.logger.log(f"Exception raised while parsing {problem}. What: {e}","ERROR")

        return Contest(contest_index, name, problems)
    def __str__(self):
        return self.name

    @property
    def problems_with_validator(self):
        problems=[]
        for problem in self.problems:
            if problem.validator_file is not None:
                problems.append(problem)
        return problems

    @property
    def problems_with_checker(self):
        problems=[]
        for problem in self.problems:
            if problem.checker_file is not None:
                problems.append(problem)
        return problems

    @property
    def problems_with_interactor(self):
        problems=[]
        for problem in self.problems:
            if problem.interactor_file is not None:
                problems.append(problem)
        return problems



