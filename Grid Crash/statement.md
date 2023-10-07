# Grid Crash

**Rami** is playing a new game called *Grid Crash*. It is a game played on a grid $G$ of size $n\times m.$
Each cell in the grid either has a colour, or is **empty.**

Initially, each cell $(i,j)$ in the grid contains some colour $c_{i,j}.$ 


Now the game is played as follow:
1. **Rami** selects some cell $(i,j)$  with colour $c_{i,j}$ and removes the colour.
2. This induces a chain reaction, on which adjacent cells of $(i,j)$ **with the same colour** are also removed. Also, the adjacent cells of those that have colour $c_{i,j}$ are removed, and so on.
3. After that, each **non empty** cell $(a-1,b)$ that is on top of an **empty cell** $(a,b)$ will swap their content. That is $(a-1,b)$ will be empty, and $(a,b)$ will contain $c_{a-1,b}.$ This will be repeated until all empty cells will be **on top** of colourful cells.
4. After that, if we have a column $C_{i}$ of empty cells, then we swap its content with $C_{i+1}.$ This will be repeated until empty columns will be a the rightmost.
5. **Rami** will repeat step 1 until the grid has no remaining colour.

Now, **Rami** wants to finish the game in the **fewest** moves possible. Please help him.

## Input
1. The first line contain two integers $1\le n,m \le 5$
2. The next $n$ lines contain each $m$ characters. with the $m^{\text{th}}$ character of the $n^{\text{th}}$ line containing the colour of the $(i,j)^{\text{th}}$ cell $0 \le c_{i,j} \le 9$
## Output
Output a single integer $r:$ the number of fewest moves possible on which **Rami** can finish the game.

## Example
### Input
```
4 5
11111
12321
12221
11111
```

### Output
```
3
```


## Notes
It is guaranteed that the grid contain **at most** $4$ distinct colours.

In the first game. **Rami** can win in $3$ moves as folllow:
1. Move 1: He selects the cell $(0,0)$ and removes it. This will give us the following grid:
```
.....
.....
.232.
.222.   
```
2. Move 2: He selects the cell $(3,2)$ and removes it. This will give us the following grid:
```
.....
.....
.....
..3..      
```
3. Finally, he removes the cell $(3,2)$ and finish the game.


