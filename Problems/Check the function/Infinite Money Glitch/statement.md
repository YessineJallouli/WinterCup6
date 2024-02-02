*Rami* wants to get rich quickly. So, he decided to follow the money and become a cryptocurrency trader.

In fact, the cryptocurrency market has $n$ currencies. Also there are $m$ pairs of currencies that can be exchanged. For each pair $(u,v)$, there is a rate $r_{u,v}$ that represents the exchange rate of the currency $u$ to the currency $v$. But that exchange is not free, there is a fee $f_{u,v}$ that is deducted from the exchanged amount.

*Rami* has $x$ units of the currency $0$. He wants to know the **minimal integer** amount of money he should spend so that he can win money only by converting currencies.

*Rami* can exchange currencies as many times as he wants. But the only condition is the final amount must be in the currency $0$.

If it is not possible to win any amount of money. Output $-1$
## Input
- The first line contains three integers $n,m,x$ $(1\le n,m\le 1000)$ and $0\le x \le 10^9$
- The next $m$ lines contains each $3$ integers $u_i,v_i,r_{u_i,v_i},f_{u_i,v_i}$. with:
  - $u_i\rightarrow v_i:$ denotes an edge of the graph.
  - $r_{u_i,v_i}:$ is the exchange rate of the currency $u_i$ to the currency $v_i.$
  - $f_{u_i,v_i}:$ is the fee deducted from the exchanged amount.
