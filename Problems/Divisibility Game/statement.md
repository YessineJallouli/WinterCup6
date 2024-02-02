#### Divisibility Game

Yessine and Rami are playing a game on an array $a$ of $n$ positive integers. Yessine and Rami make alternating moves with Yessine going first.

In each turn, the player makes the following move:

* If all elements are divisible by an **odd** number $k$, the player loses the game, otherwise : 
* The player chooses two elements $a_{i}$ and $a_{j}$ removes them and adds $a_{i}+a_{j}$ to the array.

Determine the winner of the game if both players play optimally. It's guaranteed that the sum of the elements of array is divisible by $k$.

**Input**

The first line of the input contains two integers $n$ $(1 \le n \le 10^5)$ and an **odd** number $k$ $(1 \le k \le 10^9)$

The second line contains $n$ integers, the elements of the array $a_{i}$ $(1 \le a_{i} \le 10^9)$

**Output** 

*Yessine* if *Yessine* will win. Otherwise *Rami* if he will win
