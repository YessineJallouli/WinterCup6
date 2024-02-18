**This is an optional tutorial file in case you are new to Interactive Problems, and/or you want to test your solution in depth. Note that it may take you 3-5 minutes to set up.**
# 1. Example Files 
The following archive contains example solution files found in the directory `examples`. Each file has the logic of each query implemented, so that you only have to focus on the guessing strategy. 

We have provided implementations in all supported programming languages.
# 2. Interaction Script

## 2.1 Interactors
We have made interactors in Bash as well in all supported programming languages:

| Interactor | Language | Source | Execution |
| ---- | ---- | ---- | ---- |
| `bin/interactor.sh` | Bash |  | `./bin/interactor.sh TEST_FILE` |
| `bin/cinteractor` | C | `src/interactor.c` | `./bin/cinteractor TEST_FILE` |
| `bin/cxxinteractor` | C++ | `src/interactor.cpp` | `./bin/cxxinteractor TEST_FILE` |
| `bin/interactor.jar` | Java | `src/interactor.java` | `java -jar ./bin/interactor.jar TEST_FILE` |
| `bin/interactor.py` | Python |  | `python /bin/interactor.py TEST_FILE` |

> For C and C++. We have provided binaries for **Linux.** If the binary does not work under your platform, you can regenerate the desired interactor by compiling the source code.  
## 2.2 Interaction
To test your solution, you have to compile your application (if applicable) and start the following command
<div class="interaction-small">
<pre>
<code class=language-python>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-test">TEST_FILE</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">INTERACTOR</span>  ARGS...<span class="interaction-sep">@</span> <span class="interaction-subcommand">EXECUTABLE</span> ARGS...
</code>
</pre>
</div>
Where: 
- <span class="interaction-test" style="font-size:small">TEST_FILE</span>  is the name of the test file, examples are found in the `example` directory. If you want to read from standard input, put dash: `-`
- <span class="interaction-subcommand" style="font-size:small">INTERACTOR</span>  is the name of the interactor. We **recommend** `interactor.sh` for simplicity. Otherwise, choose the interactor matching your preference.
- <span class="interaction-subcommand" style="font-size:small">EXECUTABLE</span>  is the name of your application.

| Solution Language | Interaction | Notes |
| ---- | ---- | ---- |
| C / C++ | - <span class="interaction-subcommand" style="font-size:small">EXECUTABLE</span> should be the path of the solution<br>- ARGS... should be empty  | You should first compile your solution to an executable. |
| Java | - <span class="interaction-subcommand" style="font-size:small">EXECUTABLE</span> should be `java`<br>- ARGS... should be equal to the path of the java class | You should first compile your solution to a java class. |
| Python | - <span class="interaction-subcommand" style="font-size:small">EXECUTABLE</span> should be `python`<br>- ARGS... should be equal to the path of the java class |  |
## 2.3 What does it do?
The script will simulate the interaction process, and output the verdict. It will also **generate** a file called `interaction.txt` that details the interaction process.

**If there is an error in the `start_interaction.sh` script. Please send a clarification to the Judges.**
# 3. Examples
## 3.1 C/C++ Solutions
1. Start executable named `solution` (found in the same directory) and read it from standard input using Bash interactor
	<div class="interaction"style="font-size:11pt">
<pre>
<code class=language-python>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-stdin">-</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">./bin/interactor.sh</span> <span class="interaction-sep">@</span> <span class="interaction-subcommand">./solution</span>
</code>
</pre>
</div>
2. Start executable named `solution` (found in the same directory) and read file `sample/02.in` using Python interactor
	<div class="interaction" style="font-size:11pt">
<pre>
<code class=language-python>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-test">sample/02.in</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">python</span> ./bin/interactor.py <span class="interaction-sep">@</span> <span class="interaction-subcommand">./solution</span>
</code>
</pre>
</div>
## 3.2 Python Solutions
1. Start python solution and read file `sample/01.in` using `cinteractor`
<div class="interaction" style="font-size:11pt">
<pre>
<code class=language-python>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-test">sample/01.in</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">./bin/cinteractor</span> <span class="interaction-sep">@</span> <span class="interaction-subcommand">python</span> example.py
</code>
</pre>
</div>
2. Start python solution named `solution.py` (found in the same directory) and read it from standard input using Python interactor
	<div class="interaction" style="font-size:11pt">
<pre>
<code class=language-python>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-stdin">-</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">python</span> ./bin/interactor.py <span class="interaction-sep">@</span> <span class="interaction-subcommand">python</span> ./solution.py
</code>
</pre>
</div>
## 3.3 Java
1. Start a compiled Java solution with the name `solution.class` and read standard input. Interactor is `cxxinteractor`
<div class="interaction" style="font-size:11pt">
<pre>
<code class=language-python>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-stdin">-</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">./bin/cxxinteractor</span> <span class="interaction-sep">@</span> <span class="interaction-subcommand">java</span> solution
</code>
</pre>
</div>
2. Start a compiled Java solution with the name `solution.class` and read standard input. Interactor is `interactor.jar`
<div class="interaction" style="font-size:11pt">
<pre>
<code class=language-python>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-stdin">-</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">java</span> -jar ./bin/interactor.jar <span class="interaction-sep">@</span> <span class="interaction-subcommand">java</span> solution
</code>
</pre>
</div>
# 4. FAQ
If you have a Permission Denied access, you may need to give the files the permission to executed:
```bash
chmod u+x start_interaction.sh bin/interactor.sh
```
You may also need to do the same for your solution executable:
```bash
chmod u+x ./solution
```


