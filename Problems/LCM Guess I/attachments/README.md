**This is an optional tutorial file in case you are new to Interactive Problems, and/or you want to test your solution in depth. Note that it may take you 3-5 minutes to set up.**
# 1. Example Files 
The following archive contains example solution files found in the directory <code><span class = "interaction-src" style="font-size:11pt">examples</span></code>. Each file has the logic of each query implemented, so that you only have to focus on the guessing strategy. 

We have provided implementations in all supported programming languages.
# 2. Interaction Script

## 2.1 Interactors
**Normally, you do not need to execute Interactors manually. See the next section for how to start an interaction.**

We have made interactors in Bash as well in all supported programming languages:

| Interactor | Language | Source | Operating System |
| ---- | ---- | ---- | ---- |
| <code><span class="interaction-subcommand">bin/interactor.sh</span></code> | Bash | Same File | Linux |
| <code><span class="interaction-subcommand">bin/interactor.py</span></code> | Python | Same File | Windows, Linux |
| <code><span class="interaction-subcommand">bin/cinteractor</span></code> | C | <code><span class="interaction-src">src/interactor.c</span></code> | Linux |
| <code><span class="interaction-subcommand">bin/cxxinteractor</span></code> | C++ | <code><span class="interaction-src">src/interactor.cpp</span></code> | Linux |
| <code><span class="interaction-subcommand">bin/interactor.jar</span></code> | Java | <code><span class="interaction-src">src/interactor.java</span></code> | Windows, Linux |
| <code><span class="interaction-subcommand">bin/cinteractor.exe</span></code> | C | <code><span class="interaction-src">src/interactor.c</span></code> | Windows |
| <code><span class="interaction-subcommand">bin/cxxinteractor.exe</span></code> | C++ | <code><span class="interaction-src">src/interactor.cpp</span></code> | Windows |

> For C and C++. We have provided binaries for **both** Linux and Windows under the `x86_64` architecture.
> 
> If the binary does not work under your platform, you can regenerate the desired interactor by recompiling the source code under the directory `src`.
## 2.2 Interaction
**To quickly setup interaction: See the example corresponding to your Programming Language.**

To test your solution, you have to compile your application (if applicable) and start the interaction.
### 2.2.1 Windows
<div style="font-size:11pt">
<pre>
<code>
<span class="interaction-command">python ./start_interaction.py</span> <span class="interaction-test">TEST_FILE</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">INTERACTOR</span>  ARGS... <span class="interaction-sep">@</span> <span class="interaction-subcommand">EXECUTABLE</span> ARGS...
</code>
</pre>
</div>

### 2.2.2 Linux

In Linux, you **can** use the command provided in the Windows section, or use:
<div style="font-size:11pt">
<pre>
<code>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-test">TEST_FILE</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">INTERACTOR</span>  ARGS... <span class="interaction-sep">@</span> <span class="interaction-subcommand">EXECUTABLE</span> ARGS...
</code>
</pre>
</div>

### 2.2.3 Arguments

In both cases, the arguments are as follows: 

- <span class="interaction-test" style="font-size:small">TEST_FILE</span>  is the name of the test file, examples are found in the `example` directory. If you want to read from standard input, put dash: <code><span class="interaction-stdin">-</span></code>.

- <span class="interaction-subcommand" style="font-size:small">INTERACTOR</span>  is the name of the interactor. We **recommend** <code><span class="interaction-subcommand" style="font-size:11pt">./bin/interactor.py</span></code> for simplicity. Otherwise, choose the interactor matching your preference. The interactor and your solution **do not have** to be in the same programming language.

- <span class="interaction-subcommand" style="font-size:small">EXECUTABLE</span>  is the name of your application.

## 2.3 Programming Languages
The <span class="interaction-subcommand" style="font-size:small">EXECUTABLE</span> variable depend on **your solution's** programming language as follows:

| Solution Language | Interaction | Notes |
| ---- | ---- | ---- |
| C / C++ | - <span class="interaction-subcommand" style="font-size:small">EXECUTABLE</span> should be the path of the solution<br>- ARGS... should be empty  | You should first compile your solution to an executable.<br> |
| Java | - <span class="interaction-subcommand" style="font-size:small">EXECUTABLE</span> should be `java`<br>- ARGS... should be equal to the path of the java class | You should first compile your solution to a java class. |
| Python | - <span class="interaction-subcommand" style="font-size:small">EXECUTABLE</span> should be `python`<br>- ARGS... should be equal to the path of the java class |  |
## 2.4 What does it do?
The script will simulate the interaction process, and output the verdict. It will also **generate** a file called <span class="interaction-test">interaction.txt</span> that details the interaction process. As an example:

<div class="interaction">
Problem: Find a hidden array P of strictly positive integers given its size N
<ul>
<li> Query: <code><span class="interaction-query">? i j</span></code>. Query elements i and j. </li>
<li> Response: <code><span class="interaction-response">P[i] &times P[j]</span></code>. Get the product between P<sub>i</sub> and P<sub>j</sub></li>
<li> Answer: <code><span class="interaction-answer">! P<sub>1</sub> …  P<sub>N</sub></span> </code>. Guess the array P </li>
</ul>
<pre><p style="font-size:15pt;text-align:center">Example interaction with N=4.<br> Content of <span class="interaction-src">interaction.txt</span>:</p><code>Received Input: <span class="interaction-query">? 1 2</span>
Sent Output: <span class="interaction-response">1</span>
Received Input: <span class="interaction-query">? 1 3</span>
Sent Output: <span class="interaction-response">5</span>
Received Input: <span class="interaction-query">? 1 4</span>
Sent Output: <span class="interaction-response">4</span>
Received Input: <span class="interaction-answer">! 1 1 5 3</span>
Wrong answer!
Expected: 1 1 5 4
Found:    1 1 5 3     
</code></pre>
</div>

**If there is an unexpected error in the <code><span class="interaction-command" style="font-size:11pt">start_interaction.sh</span></code> script. Please send a clarification to the Judges.**

# 3. Examples
**Each section shows how to start an interaction depending in the programming language of your solution**.
## 3.1 C/C++ Solutions
1. Start executable named `solution` (found in the same directory) and read it from standard input using Bash interactor
<div class="interaction"style="font-size:11pt">
<pre>
<code>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-stdin">-</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">./bin/interactor.sh</span> <span class="interaction-sep">@</span> <span class="interaction-subcommand">./solution</span>
</code>
</pre>
</div>
2. Start executable named `solution` (found in the same directory) and read file `sample/02.in` using Python interactor
<div class="interaction" style="font-size:11pt">
<pre>
<code>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-test">sample/02.in</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">python</span> <span class="interaction-src">./bin/interactor.py</span> <span class="interaction-sep">@</span> <span class="interaction-subcommand">./solution</span>
</code>
</pre>
</div>
## 3.2 Python Solutions
1. Start python solution and read file `sample/01.in` using `cinteractor`
<div class="interaction" style="font-size:11pt">
<pre>
<code>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-test">sample/01.in</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">./bin/cinteractor</span> <span class="interaction-sep">@</span> <span class="interaction-subcommand">python</span> example.py
</code>
</pre>
</div>
2. Start python solution named `solution.py` (found in the same directory) and read it from standard input using Python interactor
<div class="interaction" style="font-size:11pt">
<pre>
<code>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-stdin">-</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">python</span> <span class="interaction-src">./bin/interactor.py</span> <span class="interaction-sep">@</span> <span class="interaction-subcommand">python</span> solution.py
</code>
</pre>
</div>
## 3.3 Java
1. Start a compiled Java solution with the name `solution.class` and read standard input. Interactor is <code><span class="interaction-subcommand" style="font-size:11pt">./bin/cxxinteractor</span></code>
<div class="interaction" style="font-size:11pt">
<pre>
<code>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-stdin">-</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">./bin/cxxinteractor</span> <span class="interaction-sep">@</span> <span class="interaction-subcommand">java</span> solution
</code>
</pre>
</div>
2. Start a compiled Java solution with the name `solution.class` and read standard input. Interactor is <code><span class="interaction-subcommand" style="font-size:11pt">./bin/interactor.jar</span></code>
<div class="interaction" style="font-size:11pt">
<pre>
<code>
<span class="interaction-command">./start_interaction.sh</span> <span class="interaction-stdin">-</span> <span class="interaction-sep">@</span> <span class = "interaction-subcommand">java</span> -jar <span class = "interaction-subcommand">./bin/interactor.jar</span>  <span class="interaction-sep">@</span> <span class="interaction-subcommand">java</span> solution
</code>
</pre>
</div>
