#### WinterCup Maze

We have a grid with $N$ horizontal rows and $M$ vertical columns. We denote by $(i,j)$ the cell at the $i$-th row from the top and $j$-th column from the left. Each cell in the grid has a lowercase English letter written on it. The letter written on $(i,j)$ equals the $j$-th character of a given string $S_{i}$.

You will repeat moving to an adjacent cell sharing a side to travel from $(1,1)$ to $(N,M)$. Determine if there is a path in which the letters written on the visited cells (including initial $(1,1)$ and final $(N,M)$) are `w` →→ `i` →→ `n` →→ `t` →→ `e` →→ `r` →→ `c` →→ `u` →→ `p`→→ `w` →→ `i` →…→…, in the order of visiting. Here, a cell $(i_{1},j_{1})$ is said to be an adjacent cell of $(i_{2},j_{2})$ sharing a side if and only if $∣i_{1}−i_{2}∣+∣j_{1}−j_{2}∣=1$

Formally, determine if there is a sequence of cells $(i_{1},j_{1}),(i_{2},j_{2}),...,(i_{k},j_{k})$ such that:

- $(i_{1},j_{1}) = (1,1)$, $(i_{k},j_{k}) = (N,M)$;

* $(i_{t+1},j_{t+1})$ is an adjacent cell of $(i_{t},j_{t})$ sharing a side, for all  $t \ (1 \le t \le k)$
* the letter written on $(i_{t},j_{t})$ coincides with the $(((t−1)\mod9)+1)$-th character of `wintercup`, for all  $(1\le t \le k)$.