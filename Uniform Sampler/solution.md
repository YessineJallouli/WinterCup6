- Let $I_n=\{0,\dots,n-1\}$
- Let $X_n(A)$ be the expected iterations of the algorithm if the current set is $A\subseteq B$ and elements can be drawn uniformly from $I_n$
- It can be proven that $X_n$ is only a function of $r=\lvert A\rvert$
- With that we will simply define $X_{n,r}$ the expected iterations starting from a set of size $n$


$$
\begin{align*}
\mathbb{E}[X_{n,r}] &= 1+\frac{1}{n} \sum_{k=0}^{n-1}
\mathbb{E}[X_{n,r} \mid k\ \text{is chosen}] \\
&= 1+\frac{1}{n} \sum_{k=0}^{r-1}
\mathbb{E}[X_{n,r} \mid k\ \text{is chosen}] + \frac{1}{n} \sum_{k=r}^{n-1}
\mathbb{E}[X_{n,r} \mid k\ \text{is chosen}] \\
&=1+\frac{1}{n} \sum_{k=0}^{r-1}
\mathbb{E}[X_{n,r}] + \frac{1}{n} \sum_{k=r}^{n-1}
\mathbb{E}[X_{n,r+1} ] \\
&=1+\frac{r}{n}\mathbb{E}[X_{n,r}] +\frac{n-r}{n}\mathbb{E}[X_{n,r+1}] \\
\iff \mathbb{E}[X_{n,r}]&=\frac{n}{n-r}+\mathbb{E}[X_{n,r+1}] \\
\mathbb{E}[X_{n,0}]&=\sum_{k=0}^{m-1}\frac{n}{n-k} \\
&= \sum_{k=n-m+1}^{n}\frac{n}{k}\\
&=n\cdot (H_n-H_{n-m})
\end{align*}
$$
With $(H_n)_{n\in\mathbb{N}^*}$ the harmonic series, and we will define $H_0=0$
