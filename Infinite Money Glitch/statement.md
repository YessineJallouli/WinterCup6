*Rami* wants to get rich quickly. So, he decided to follow the money and become a cryptocurrency trader.

In fact, the cryptocurrency market has $n$ currencies. Also there are $m$ pairs of currencies that can be exchanged. For each such pair $(u,v)$, there is a rate $r_{u,v}$ that represents the exchange rate of the currency $u$ to the currency $v$. But that exchange is not free, there is a fee $f_{u,v}$ that is deducted from the exchanged amount as following:
$$
\text{amount}(v)=r_{u,v}\times (\text{amount}(u)-f_{u,v})
$$
Now, such an exchange can occur only if $\text{amount}(u) \ge f_{u,v}.$ Otherwise, it is rejected as there is not enough money to pay for the fees.

*Rami* is broke, he has no money. But fortunately, he can convince his friends *Yessine* and *Oussama* to lend him up to $x$ amount of money. Note that the borrowed amount is always in currency $0.$

He wants to know the **minimal integer** amount of money he should borrow so that he can win money only by converting currencies.

*Rami* can exchange currencies as many times as he wants. But the only condition is the final amount must be in currency $0$.

If it is not possible to win any amount of money. Output $-1$
## Input
- The first line contains three integers $n,m,x$ $(1\le n,m\le 1000)$ and $0\le x \le 10^9$
- The next $m$ lines contains each $4$ numbers $u_i,v_i,r_{u_i,v_i},f_{u_i,v_i}$. with:
  - $1\le u_i,v_i\le n$ are **two distinct** integers and $u_i\rightarrow v_i:$ denotes an edge of the graph.
  - $0.1\le r_{u_i,v_i} \le 10:$ is the exchange rate of the currency $u_i$ to the currency $v_i,$ represented with $2$ decimal places.
  - $0\le f_{u_i,v_i}\le 10:$ is the fee deducted from the exchanged amount, represented with $2$ decimal places.
## Output
An integer $0\le y \le x$ representing the least **integer** amount he needs to borrow to win money. If such integer does not exist, output $-1.$

## Notes
- A real number $x$ is represented with $2$ decimal places if it is written as $x.ab$ where $a,b$ are digits and $x$ is an integer.
- A pair of nodes $(u,v)$ may have **multiple edges**. This is translate to a pair of currencies having multiple exchange prices.
- It is guaranteed that for any sequence of conversions $\Pi$ of length $\lvert \Pi \rvert \le n.$ We have the following:
$$
10^{-4}\le \sum_{\substack{\text{rates}\ r \\ \text{of} \ \Pi}}\log r \le 10^4 
$$
