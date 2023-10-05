#### Le compte est bon : 

***Des chiffres et des lettres*** is a well known French television program. In **des chiffres** round, the goal is to arrive at a chosen number (from 101 to 999) using the four basic arithmetic operations (+, -, ×, ÷) applied to six numbers chosen randomly from the following alternatives: 1 to 10; 25; 50; 75; 100 (each number is drawn from the entire set, so the same number may appear more than once).

The contestants may use each of the six numbers originally selected once, and the result of each operation performed with them once – for example, if a contestant multiplies 4 by 25 to obtain 100, he may no longer use the 4 or 25, but may use the 100 in further calculations. The contestants are not required to use all six numbers, and must obtain only **positive** integers as a result of any step.

You are given six integers and a target. Find a sequence of operations to find the closest number to target.

**Input** 

The first line contains a single integer $t$ $(1 \le t \le 10)$ — the number of test cases.

The first line of each test case contains six integers $a_{1},a_{2},...,a_{6} (a_{i} \in [1..10,25,50,75,100])$

The second line of each test case contains one integer $x$ — the target.

**Output** 

For each test case, output two lines : 

**Test case:** 

```
3
1 4 5 8 75 100
389
1 1 6 7 8 25
786
6 9 10 25 50 100
523
```

```
4
1 + 100 = 101
4 * 101 = 404
75 / 5 = 15
404 - 15 = 389

5
1 + 6 = 7
1 + 8 = 9
7 * 7 = 49
25 - 9 = 16
49 * 16 = 784

5
6 + 9 = 15
10 + 25 = 35
100 / 50 = 2
15 * 35 = 525
525 - 2 = 523
```

**Note** 

* You can use division only if the first number is **divisible** by the second one.
* In any operation, you should obtain only **positive** integer. 
* Respect the output format as you can see in the test-cases.
* If there is many sequence to obtain the closest number to target, output any of them.
* In the second test case, there is no way to obtain 786, the closest number is 784.