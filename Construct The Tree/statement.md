#### Construct the tree : 

Yessine would like to construct a tree with $n$ vertices, indexed from $1$ to $n$. For this, he have $n$ informations about the tree :

For each node $i$ $(1 \le i \le n)$, he know one of the furthest node from node $i$.

Construct any tree satisfying those $n$ informations, if there is no answer, print $-1$.

**Input**

The first line in the input contains one integer $n$ — the number of vertices in the tree $(2 \le n \le 10^{5})$ 

Each of the next $n$ lines contains one integer $v_{i}$ — the furthest node from node $i$ $(1 \le v_{i} \le n)$

**Output**

Print $n-1$ lines,  each line containing a description of an edge of the tree — a pair of positive integers $x_{i},y_{i}$,which means that the $i$th edge connects vertices $x_{i}$ and $y_{i}$.