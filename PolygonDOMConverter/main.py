import os
import tempfile

import convert
from contest import Contest
import argparse
import logging
import scripts
import zipfile

from pathlib import Path


def remove_directory_tree(start_directory: Path):
    """Recursively and permanently removes the specified directory, all of its
    subdirectories, and every file contained in any of those folders."""
    for path in start_directory.iterdir():
        if path.is_file():
            path.unlink()
        else:
            remove_directory_tree(path)
    start_directory.rmdir()

def custom_file_warning(custom_file,problems):
    if len(problems) > 0:
        logger.log(
            f"This contest contains problems custom {custom_file}. You have to edit them manually to adhere to DOMJudge format.",
            "WARNING")
        logger.log(f"Custom {custom_file}: {', '.join([str(problem) for problem in problems])}","WARNING")




if __name__ == "__main__":
    logger=logging.init_logger()
    parser = argparse.ArgumentParser()
    parser.add_argument("--directory", help="The contest directory")
    parser.add_argument("--tmp",help="Temporary directory for work",default=None)
    parser.add_argument("--target", help="The target file",default=None)
    parser.add_argument("--tests", help="Generate test files",action="store_true")
    parser.add_argument("--tex", help="Compile Tex files",action="store_true")
    parser.add_argument("--preferred-statement",help="Preferred statement type",default="pdf")
    args = parser.parse_args()
    if args.tmp is None:
        logger.log("No TMP directory specified, creating one")
        args.tmp = tempfile.mkdtemp()
        logger.log(f"TMP directory: {args.tmp}")
    contest:Contest
    with scripts.change_directory(args.directory):
        with open("contest.xml", "r") as file:
            logger.log("Marking scripts as executable")
            scripts.mark_scripts_as_executable(args.directory)
            logger.log("Parsing contest file")
            try:
                contest = Contest.from_xml(file)
                logger.log("Contest parsed successfully")
                custom_file_warning("checkers", contest.problems_with_checker)
                custom_file_warning("validators", contest.problems_with_validator)
                custom_file_warning("interactors", contest.problems_with_interactor)

            except Exception as e:
                logger.log(f"Exception raised when parsing contest.xml. What: {e}.", "ERROR")
        if args.tests:
            logger.log("Generating test files")
            for problem in contest.problems:
                try:
                    logger.log(f"Running tests generator for problem {problem}")
                    problem.tests_script.run(working_dir=os.path.join(args.directory, "problems", problem.short_name))
                except Exception as e:
                    logger.log(f"Exception raised when running tests generator. What: {e}", "ERROR")
        if args.target is not None:
            logger.log("Converting files to DOMJudge format")
            convert.prepare_contest_archive(args.directory,args.tmp,contest,preferred_type="pdf")
        else:
            logger.log("Deleting TMP directory")
            remove_directory_tree(Path(args.tmp))