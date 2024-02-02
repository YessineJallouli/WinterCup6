#### Dark Souls :

Before Competitive programming, Oussama used to play **a lot** of video games. There were plenty of memorable games from the hundreds he played, but **Dark Souls** is definitely his favorite.

Anyway, Oussama is now playing the game and reached *Anor Lando*. He is looking for *Solaire*.
*Anor Lando* can be modeled as a $N$*$M$ grid. Oussama is currently at cell (1,1) and wants to reach *Solaire* at the bonfire at cell ($N$,$M$).
Every Cell contains a silver knight. the knight at cell ($i$,$j$) drops $A_{i,j}$ souls when killed, qhich are collected by the players. A knight that drops $X$ souls can be killed only if the player already collected at least $X$ souls.
The player can move in any direction but he has to defeat the knight at his current cell first. After defeating a silver knight the cell becomes empty and he can revisit that cell multiple times.
Oussama currently has $S$ souls and will try to reach cell ($N$,$M$) and defeat the knight located there in order to find *Solaire*. Can he make it ?

**Input**

The first line in the input contains two integers $N$ and $M$, the dimensions of the grid  $(2 \le  $N$ , $M$  \le 10^{3})$
N lines come next, each containing $M$ integers

The $j$th integer in the $i$th line is the value  $(2 \le  $$A_{i,j}$$  \le 10^{9})$

**Output**

Print "PRAISE THE SUN" if Oussama can reach Solaire, or "YOU DIED" otherwise.