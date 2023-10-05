#### Universe Algorithm (Easy Version) : 

**This is the easy version of the problem. The differences between the two versions are the values of k and m. In this version, k = 2**

Universe algorithm is an algorithm that takes three parameters $n$, $m$, and $k$ and create $n$ random arrays $a_{i,j} \ (1 \le i \le n \ , 1 \le j \le m)$  of length $m$ which are composed of positive integers with maximum value $k$ $(1 \le a_{i,j} \le k )$.

Universe algorithm is called perfect if this condition is guaranteed whatever the result of the algorithm: 

* There exist two **different** integers $i$ and $j$ $(1 \le i,j \le n)$ such that $a_{i,p} \le a_{j,p}$ for every $1 \le p \le m$

You are given two integers $m$ and $k = 2$, the length of the arrays and the maximum value, find the minimum value $n$ that gives a perfect algorithm.

Notice that the answer might be large, please output the desired value modulo $998,244,353$

**Input**

The input contains one integer $m$ $(1 \le m \le 10^{5})$ — The length of the arrays.

**Output**

Output one integer, representing the desired answer modulo $998,244,353$

