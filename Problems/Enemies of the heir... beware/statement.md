#### Check the function

You are given an array $P$, consisting of $n$ non-negative integers. 

Your task is to check if there exist an array $a$ of length $n$ which prefix function is $P$.

The prefix function of an array $a$ of size $n$ is a sequence $p_{1},p_{2}...p_{n}$, where $p_{i}$ is the maximum value of $k$ such that $k<i$ and $a[1..k] = a[i-k+1..i]$ ($a[l..r]$denotes a contiguous sub-array of an array $a$ from a position $l$ to a position $r$, inclusive). In other words, it's the longest proper prefix of the string $a[1..i]$ that is equal to its suffix of the same length.

For example, for the array $a = [1,2,1,3,1,2,3]$ the values of the prefix function in positions $1,2,...,7$ are equal to $P=[0,0,1,0,1,2,3]$

**Input**

Each test contains multiple test cases. The first line contains the number of test cases $t \ (1 \le t≤10^4 )$. Description of the test cases follows. 

The first line of each test case contains a single number $n$ $(1 \le n \le 10^{5})$ — the length of the array $P$.

The second line of each test case contains $n$ integers $P_{1},P_{2}...P_{n}$ $(0 \le P_{i} ≤ n)$.

It is guaranteed that the sum of the values of $n$ for all test cases does not exceed $2⋅10^{5}$.

**Output**

For each test case output "YES" if there is an array which prefix function is P and "NO" otherwise.

**Tests**

```in
4
7 
0 0 1 0 1 2 3
6
0 1 0 1 2 1
6
0 1 2 3 4 1
8
0 1 0 1 2 2 3 0
```

```
YES
NO
NO
YES
```

