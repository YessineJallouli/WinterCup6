# Solution

- Remove any edge that has a probability $0$
- Construct a graph $\mathcal{G}=(\mathcal{V},\mathcal{E})$ representing the problem
- Let $s$ be the initial position of *Rami* and $t$ be the initial position of *Yessine*. And we suppose without a loss of generality that $s\ne t$

- Let $\mathbb{E}[W_{i,j}]$ be the expected length of the walk if *Rami* is at $i$ and *Yessine* is at $j$

- Let $\mathcal{A}$ be the condensation graph of $\mathcal{G}$ and $\{C_1,\dots,C_m\}$ be its nodes. That is $C_1,\dots,C_m$ are the strongly connected components of $\mathcal{G}$

- Let $\mathcal{S}(\mathcal{A})=\{C_i/\quad C_i\nrightarrow C_j \quad\forall j\},$ that is $\mathcal{S}(\mathcal{A})$ is the set of sinks of $\mathcal{A}.$ This set is not empty as $\mathcal{A}$ is a directed acyclic graph.

- Let $\mathcal{G}^2=(\mathcal{V}^2,\mathcal{E}^2)$ be the product graph of $\mathcal{G}$ and $\mathcal{G}.$ It is the graph of the simultaneous walk of both *Rami* and *Yessine*

  

## 1. Infinite Walk

- First of all, we will observe that eventually, both persons will get stuck at $(C_i,C_j)\in\mathcal{S}(\mathcal{A})^2$

- Note that the expected walk length is infinite if and only if both persons can go with $k$ steps to $(u,v)$ without a crossover such that $\mathbb{E}[W_{u,v}]$ is infinite and the path has a **non-zero** probability (conditioning on the number of steps $k$). The latter is guaranteed as we removed any edge with $0$ probability.
- Now, we can reduce the problem to finding such $u,v$ with the requirement that each of $u,v$ is on some **sink** connected component:
  - If $u,v$ are in the same connected component $C\in\mathcal{S}(\mathcal{A}),$ then the $\mathbb{E}[W_{u,v}]=+\infty$ if and only if $(u,v)$ cannot go simultaneously to a common node $(i,i)$ 
  - Otherwise if $u\in C,v \in C'$ and $C,C'\in\mathcal{S}(\mathcal{A})$ and $C\ne C'$, It is trivial that $\mathbb{E}[W_{u,v}]=+\infty$
  - If no $u,v$ exists, each person cannot go to some **sink** connected component without a crossover. It can be shown that in that case $\mathbb{E}[W_{s,t}]<+\infty$

With that the expected walk length is infinite if and only if:

1. $\exists i,j\in\{1,\dots,m\}/\quad s\rightarrow^*u\in C_i,t\rightarrow^*v\in C_j$ and $C_i,C_j\in\mathcal{S}(\mathcal{A})$
2. There exists a path $(s,t)\rightarrow ^* (u,v)$ in the product graph that does not cross $(k,k)\quad \forall k\in\{0,\dots,n-1\}$
3. Every path that goes from $(u,v)$ does not cross $(k,k) \quad \forall k\in\{0,\dots,n-1\}$

Now with that, we will do a BFS starting from $(s,t)$ on the graph $\mathcal{G}'$ that is equal to $\mathcal{G}^2$ after removing the nodes $(i,i)\quad \forall i\in\{0,\dots,n-1\},$ and we mark all reachable nodes on the boolean matrix `visited`. Such BFS can be implemented in $\mathcal{O}(n^2+m^2)$

Now for each reachable node $(i,j),$ let $C(i)$ the connected component of $i,$ and $C(j)$ the connected component of $j$ 

1. If $C(i)\notin \mathcal{S}(\mathcal{A})$ or $C(j)\notin \mathcal{S}(\mathcal{A}),$ skip
2. Otherwise, if $C(i)\neq C(j),$ set $\mathbb{E}[W_{s,t}]\leftarrow +\infty$ and exit the loop
3. Otherwise we have $C(i)=C(j)$, we do a BFS on the product graph $\mathcal{G}^2$ starting from $(i,j)$ (without removing the nodes $(k,k)$), and we save the reachable nodes on `marked` 
   - If there is some node $(i,i)$ such that `marked[i][i]` is True, skip
   - Otherwise, set $\mathbb{E}[W_{s,t}]\leftarrow +\infty$ and exit the loop

After that if there is no assignation $\mathbb{E}[W_{s,t}]\leftarrow +\infty,$ then we are sure that $\mathbb{E}[W_{s,t}]$ is finite

With that the time complexity of such brute forcing BFS is $\mathcal{O}(n^2(n^2+m^2)).$

The overall time complexity of such check is:
$$
\mathcal{O}(n^4+n^2m^2)=\mathcal{O}(n^6)
$$

## 2. Finite Walk

We have:
$$
\mathbb{E}[W_{u,v}]=\begin{cases}
0 & \text{if}\space u=v\\
\displaystyle 1+\sum_{0\le i,j< n} p_{u,i}p_{v,j}\mathbb{E}[W_{i,j}] &\text{otherwise} 
\end{cases}
$$
This is a system of $n^2$ equations on $n^2$ variables.

It can be solved using Gaussian elimination on:
$$
\mathcal{O}(n^6)
$$

## 3. Time Complexity

The time complexity of the overall algorithm is:
$$
\mathcal{O}(n^6)
$$
