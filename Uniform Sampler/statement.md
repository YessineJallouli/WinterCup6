# Uniform Sampler

*Oussama* loves basketball, and he loves to improve his throwing technique.

After finishing his work at Iceberg Corporation, he will prepare $n$ baskets aligned as an array and start throwing basketballs. *Oussama* may not be the best basketball player, but he has learnt to not miss any ball.

In fact, he never misses his shot. He will always scores at one of the baskets, but also, his accuracy is average as best: At each attempt, the probability that he will score a goal to the $i^\text{th}$ basket is $\frac{1}{n}.$ Also, if he scores a goal to the $i^\text{th}$ basket, its score will increase by one.

Initially, the score of each basket is $0$, and he will make shots until at least $m$ different baskets have **non-zero** scores. Also, he has infinite amount of basketballs.

*Oussama* is still making attempts, and *Yessine* is getting worried that he will his work next day. Can you help him estimate **the expected** number of attempts he will make until scoring at least a goal to $m$ baskets.
## Input
- $1\le T \le 10^6:$ The number of test cases
- For each test case:
	- $0\le n \le 10^6:$ The size of the original set
	- $0\le m\le n:$ The size of the sample set

## Output
- For each test case, output a real $N,$ the  expected number of times step $2$ will be executed.