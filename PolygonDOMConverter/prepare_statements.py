import os
import shutil

from bs4 import BeautifulSoup
import argparse
import logs
import scripts

import time


def current_time_iso_8601():
    return time.strftime("%Y-%m-%dT%H:%M:%S", time.localtime())


def prepare_public_web_directory(contest_directory: str, install_mathjax: bool):
    os.makedirs(os.path.join(contest_directory, "public"), exist_ok=True)
    os.makedirs(os.path.join(contest_directory, "public", "css"), exist_ok=True)
    os.makedirs(os.path.join(contest_directory, "public", "media"), exist_ok=True)
    os.makedirs(os.path.join(contest_directory, "public", "mathjax"), exist_ok=True)
    os.makedirs(os.path.join(contest_directory, "public", "pdf"), exist_ok=True)
    if len(os.listdir(os.path.join(contest_directory, "public", "mathjax"))) == 0 and install_mathjax:
        logs.logger.log("MathJax not found in public directory, installing")
        scripts.run_script("install-mathjax.sh", os.path.join(contest_directory, "public"))
    os.makedirs(os.path.join(contest_directory, "public", "js"), exist_ok=True)


def patch_css(soup: BeautifulSoup):
    result = soup.find("link", attrs={"href": "problem-statement.css"})
    if result is not None:
        result.attrs["href"] = "/css/problem-statement.css"
        logs.logger.log("\tPatched problem-statement.css")
    else:
        logs.logger.log("\tTag link with href=problem-statement.css not found", "WARNING")


def patch_scripts(soup: BeautifulSoup):
    for script in soup.findAll("script"):
        script.decompose()
    math_jax_script = soup.new_tag(name="script", src="/mathjax/tex-chtml.js", id="MathJax-script")
    math_jax_script["async"] = ""
    math_jax_config_script = soup.new_tag(name="script")
    math_jax_config_script.string = """
            MathJax = {
       tex: {
         inlineMath: [['$$$', '$$$'], ['\\(', '\\)']],
         displayMath: [['$$$$$$','$$$$$$']]
       },
       svg: {
         fontCache: 'global'
       }
     };
    """
    soup.head.append(math_jax_config_script)
    soup.head.append(math_jax_script)
    logs.logger.log("\tPatched scripts")


def patch_img_resources(soup: BeautifulSoup, statement_directory: str, dest_directory: str = None):
    for img in soup.findAll("img"):
        if img["src"].startswith("http"):
            logs.logger.log("\tSkipping external resource " + img["src"])
            continue
        if dest_directory is not None:
            if not os.path.exists(os.path.join(dest_directory, img["src"])):
                logs.logger.log("\tResource " + img["src"] + " not found in statement directory", "ERROR")
                continue
            else:
                shutil.copy(os.path.join(statement_directory, img["src"]),
                            os.path.join(dest_directory, "media", img["src"]))
                shutil.copy(os.path.join(statement_directory, img["src"]),
                            os.path.join(os.path.dirname(statement_directory), "attachments", img["src"]))
                pass
        img["src"] = "/media/" + img["src"]
        logs.logger.log("\tPatched img resource " + img["src"])


def patch_file(input_path, output_path, contest_directory=None):
    with open(input_path, "r") as file:
        soup = BeautifulSoup(file, "html.parser")
        previous_patch = soup.find("meta", attrs={"data-patch-time": True})
        if previous_patch is not None:
            logs.logger.log(f"\tThis file has already been patched on {previous_patch['data-patch-time']}", "ERROR")
            return
        patch_css(soup)
        patch_scripts(soup)
        if contest_directory is not None:
            patch_img_resources(soup, os.path.dirname(input_path), os.path.join(contest_directory, "public"))
        else:
            patch_img_resources(soup, os.path.dirname(input_path), os.path.dirname(output_path))
        meta_patch = soup.new_tag("meta")
        meta_patch.attrs["data-patch-time"] = current_time_iso_8601()
        soup.head.append(meta_patch)
    with open(output_path, "w") as file:
        file.write(str(soup))


if __name__ == "__main__":
    logs.init_logger()
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", help="The input file", required=False)
    parser.add_argument("--output", help="The output file", required=False)
    parser.add_argument("--contest", help="The contest directory", required=False)
    parser.add_argument("--install-mathjax", help="Install MathJax", action="store_true")
    args = parser.parse_args()
    inputs = []
    outputs = []
    css_file=None
    if not args.input and not args.contest:
        raise ValueError("Either input or contest must be specified")
    if not args.output:
        args.output = args.input
    if args.contest:
        prepare_public_web_directory(args.contest, args.install_mathjax)
        for potential_dir in os.listdir(args.contest):
            subdir = os.path.join(args.contest, potential_dir)
            if os.path.isdir(subdir):
                for file in os.listdir(subdir):
                    if file.endswith(".html"):
                        inputs.append(os.path.join(subdir, file))
                        outputs.append(os.path.join(subdir, file))
                    if css_file is None and file == "problem-statement.css":
                        css_file=os.path.join(subdir, file)
                        shutil.copy(css_file, os.path.join(args.contest, "public", "css", "problem-statement.css"))
                    if file.endswith(".pdf"):
                        shutil.copy(os.path.join(subdir, file),
                                    os.path.join(args.contest, "public", "pdf", potential_dir + ".pdf"))
                        shutil.copy(os.path.join(subdir, "problem_statement", file),
                                    os.path.join(subdir, "attachments", file))
                if os.path.exists(os.path.join(subdir, "problem_statement")):
                    for file in os.listdir(os.path.join(subdir, "problem_statement")):
                        if css_file is None and file == "problem-statement.css":
                            css_file = os.path.join(subdir,"problem_statement", file)
                            shutil.copy(css_file, os.path.join(args.contest, "public", "css", "problem-statement.css"))
                        if file.endswith(".html"):
                            inputs.append(os.path.join(subdir, "problem_statement", file))
                            outputs.append(os.path.join(subdir, "problem_statement", file))
                        if file.endswith(".pdf"):
                            shutil.copy(os.path.join(subdir, "problem_statement", file),
                                        os.path.join(args.contest, "public", "pdf", file))
                            shutil.copy(os.path.join(subdir, "problem_statement", file),
                                        os.path.join(subdir, "attachments", file))
    else:
        inputs.append(args.input)
        outputs.append(args.output)
    for file_in, file_out in zip(inputs, outputs):
        logs.logger.log(f"Patching file {file_in} into {file_out}")
        patch_file(file_in, file_out)
