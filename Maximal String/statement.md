#### Maximal String

You are given a binary string of length $n$.

You can take two consecutive $1$, remove them, and replace them with $0$.

Also, you can take two consecutive $0$, remove them, and replace them with $1$.

You can do these operations any number of times (possibly zero).

You have to find the largest lexicographically string you can obtain.

**Input**

The first line contains one integer $t$ $( 1 \le t \le 10^4)$ — the number of test cases.

The first line of each test case contains one integer $n$ $(1 \le n \le 10^5)$ — the length of the binary string $s$.

The second line of each test case contains a binary string consisting of $n$ characters.

It is guaranteed that the sum of $n$ over all test cases does not exceed $2.10^5$

**Output**:

For each test case, output $t$ : the largest lexicographically string you can obtain.

**Input** 

```
6
6
000010
6
101000
6
011000
6
110101
14
01010101010100
14
10101010110101

```

**Output**

```
1110
110
110
110101
1
110101
```

