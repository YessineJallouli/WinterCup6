#### Construct the tree : 

Yessine would like to construct a tree with $n$ vertices, indexed from $1$ to $n$. For this, he have $n$ details about the tree :

For each node $i$ $(1 \le i \le n)$, he knows one of the furthest node from node $i$.

Your task is to check if there is any construction satisfying those $n$ details.

**Input**

The first line in the input contains one integer $n$ — the number of vertices in the tree $(2 \le n \le 10^{5})$ 

The second line contains $n$ integer $v_{i}$ — the furthest node from node $i$ $(1 \le v_{i} \le n)$

**Output**

Print "YES" if you can construct the tree and "NO" otherwise.

**Input**

```
5
4 5 2 2 2
```

**Output**

```
YES
```

**Input**

```
5
5 2 1 3 2
```

**Output**

```
NO
```
