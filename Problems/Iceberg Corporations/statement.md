#### Iceberg Corporation :

As stated in a previous problem, Yessine is a wealthy business man who founded *Iceberg Corporation* and hired Oussama to work for him. The start-up grew significantly so he bought a big office and hired one additional employee who can't be anyone other than Rami . The three of them would meet every evening (because none of them would wake up in the morning) and they would work together. Everything was perfect, *except for one thing*. Sometimes, Rami and Yessine will make a Shisha and start smoking in the office. Oussama doesn't really like smoking so he decided to stop them.

Rami has brought K pieces of charcoal and left them at the office. Oussama decided to hide them. The office is a $N,M$ grid. Oussama will hide a single piece in a grid cell. However, he won't hide them at random spots. Instead, he will hide a piece in a cell if has the maximum sum of Manhattan distances from all the other cells in the grid.

Formally, he wants to maximize :

$\sum\limits_{i=1}^N \sum\limits_{j=1}^M \sum\limits_{k=1}^K D(i,j,k)$  where $D(i,j,k)$ denotes the Manhattan distance between cell ($i$,$j$) and the $k$th piece.

Help Oussama before his lungs get sick.

**Input**

The first and only line contains three integers $N$, $M$, $K$  $(1 \le  $$N$,$M$,$K$$  \le 10^{6})$

**Output**

Print the maximum possible sum