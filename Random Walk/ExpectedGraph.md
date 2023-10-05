# E. Expected Crossover



*Rami* and *Yessine* were planning to meet at the city of *Sfax*. Unfortunately, they both forgot their phones, so they lost contact of each other. So,each one of them will follow a **random** direction **at the same time and independently from each other** until meeting at the same location.

*Sfax* is a directed graph with $n$ nodes $0,\dots,n-1$ and $m$ edges. An edge $i\rightarrow j$ represents a potential direction that a person can follow if he is currently at the node $i.$ Also, that edge $i\rightarrow j$ has a probability $p_{i,j}$ of being selected, which does not depend on the person.

Furthermore, the edge $i\rightarrow i$ represents that a person will stay on the edge $i$. If a person is on the node $i$; he has a probability $p_{i,i}$ that he will stay on that node.

*Rami* starts at the node $s$ and *Yessine* starts at node $t.$ 

What is the expected length of the walk both persons will follow until meeting each other.

Note that the length of a walk is the number of followed edges.



## Input

- A line containing 4 integers $n,m,s,t:$ 
  - $n:$ the number of nodes
  - $m:$ the number of edges
  - $s:$ starting node of *Rami*
  - $t$ starting node of *Yessine* 
- The next $m$ lines contains each $3$ integers $u_i,v_i,p_{u_i,v_i}$. with:
  - $u_i\rightarrow v_i:$ denotes an edge of the graph
  - $p_{u_i,v_i}:$ is the probability of selecting this edge if the person is at node $u_i$

## Output

The expected length of the walk of *Rami* and *Yessine* until they meet each other. 

If the expected length is infinity; output `inf`



## Constraints

- $1\le n \le 25$
- $1\le m \le n^2$
- $0\le s,q \le n-1 $
- $0\le u_i,v_i \le n-1$
- $0 \le p_{u_i,v_i} \le 1$

It is guaranteed that:
$$
\forall i\in\{0,\dots,n-1\},\quad \sum_{j=0}^{n-1}p_{i,j}=1
$$
It is also guaranteed that:
$$
\forall i,j\in\{0,\dots,n-1\},\quad p_{i,j}=0 \space\text{or}\space p_{i,j}\ge0.04
$$


## Sample

| 3 6 0 2<br/>0 1 0.5<br/>0 2 0.5<br/>1 2 0.5<br/>1 0 0.5<br/>2 1 0.5<br/>2 0 0.5 | 4    |
| ------------------------------------------------------------ | ---- |
| 5 5 0 4<br/>0 1 1<br/>1 2 1<br/>4 3 1<br/>3 2 1<br/>2 2 1    | 2    |
| 3 4 0 2<br/>0 1 0.5<br/>0 2 0.5<br/>2 2 1<br/>1 1 1          | inf  |

