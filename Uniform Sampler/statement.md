# Uniform Sampler

**Rami** was having a very serious problem in his internship. He has to sample $m$ **distinct** elements from the set $\{0,\dots,n-1\}.$
To achieve this, he implemented the following algorithm:
1. Initialise $S=\varnothing$
2. Select a random $a$ uniformly from $\{0,\dots,n-1\}$
3. If $a\not\in S,$ append $a$ to $S$
4. Repeat $2$ while the size of $S$ is strictly less than $m$

**Rami** wants to estimate the computational complexity of his algorithm. And to do that, he will count the number of times step $2$ will be executed in **average**

Please help him

## Input
- $1\le T \le 10^6:$ The number of test cases
- For each test case:
	- $0\le n \le 10^6:$ The size of the original set
	- $0\le m\le n:$ The size of the sample set

## Output
- For each test case, output a real $N,$ the  expected number of times step $2$ will be executed.