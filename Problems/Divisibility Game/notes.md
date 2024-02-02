
## 1. Reduction
- Let $C$ the frequency array of $A$
- Let $S'=\sum_{k>0}C[k]$
- Let $S=S'+C[0]$


The game is fully specified by $C$

## 2. Conjectures
- We can set $C[0]\leftarrow C[0] \bmod 2$
- The winning condition:

$$ 
G(C) = \begin{cases}
0 & \text{if} \ C[0]=n \\
1 &\text{otherwise if} \ C[0]=n-2 \ \text{and} \ n \ \text{is even}\\
2 & \text{otherwise if} \ C[0]=n-2  \ \text{and} \ n \ \text{is odd}\\
1 &\text{otherwise if} \ C[0]<n-2  \ \text{and} \ n \ \text{is even}\\
0 & \text{otherwise if} \ C[0]<n-2\ \text{and} \ n \ \text{is odd}\\
\end{cases}
$$

$$
\forall n\in\mathbb{N}^*,\forall A \in \mathbb{N}^{n},G(C(A))= \begin{cases}
0 & \text{if} \ C[0]=n \\
1 &\text{otherwise if} \ C[0]=n-2 \ \text{and} \ n \ \text{is even}\\
2 & \text{otherwise if} \ C[0]=n-2  \ \text{and} \ n \ \text{is odd}\\
1 &\text{otherwise if} \ C[0]<n-2  \ \text{and} \ n \ \text{is even}\\
0 & \text{otherwise if} \ C[0]<n-2\ \text{and} \ n \ \text{is odd}\\
\end{cases}
$$

### 2.1 Case 1
Trivial
### 2.2 Case 2
Let $\displaystyle C\rightarrow^{i,j}C'$
- If $i=0\vee j=0$, Then $G(C')=2$
- Else, $G(C')=0.$ It is guaranteed that there exists such $C'$
In any case, this implies $G(C)=1$
### 2.3 Case 3
We have $n$ is odd, so that $C[0]=n-2\ge 1$
Let $\displaystyle C\rightarrow^{i,j}C'$
- If $i=0\vee j=0$, Then $G(C')=1$
- Else, $G(C')=0.$
Both cases are guaranteed to exist. This implies $G[C]=2$

### 2.4 Case 4
- In this case: $n\ge 4$
- Let $\displaystyle C\rightarrow^{i,j}C'$
#### 2.4.1 $C[0]=0$
##### 2.4.1.1 $C'[0]=n-3$
We have $0 \le C'[0]-C[0] \le 1.$ Which implies that $0 \le C'[0] \le 1.$
which implies that $3\le n \le 4.$ As $n$ is even, then $n=4$  

We claim that we have another $i',j'$ so that $C\rightarrow^{i',j'}C''$  and $C''[0]=0.$ 
This is guaranteed as $K\ge 3$ is odd. 

With that $G[C]=1$
##### 2.4.1.2 $C'[0]<n-3$
In this case, we have by force $G(C)=1$

#### 2.4.2 $C[0]>0$
##### 2.4.2.1 $C'[0]=n-3$
We have $C'[0]-1\le C[0] \le C'[0]+1$ which implies $n-4\le C[0]\le n-2.$
By assumption, we have $C[0]< n-2,$ which implies that $n-4 \le C[0] \le n-3$

In any case. As $K$ is odd, we claim that there exists another move $(i',j')$  such that $C\rightarrow^{i,j}C''$ and $C''[0]<n-3.$

This implies $G(C)=1$
##### 2.4.2.2 $C'[0]<n-3$
In this case, we have by force $G(C)=1$

### 2.5 Case 5
- In this case: $n\ge 3$
- Let $\displaystyle C\rightarrow^{i,j}C'$

We have $C'[0]\le C[0]+1.$ Which implies $C'[0] < n-1.$ 
With that, we have by force $G(C)=0$

### 2.6 Base Case
The base case is $n=1.$ Which is trivial


- Let $N$ the size of the queries
- Let $B$ the size of the block
- Let $B_i$ be the $i^\text{th}$ block
- Let $t_k$ be the $k^\text{th}$ timestamp
- Let $Q$ be the number of queries
- Let $U$ be the number of updates
- Let $C(X)$ be the cost of a group $X$ of instructions

$$
\begin{align*}
C(B_i,B_j,t_k)&= B \\
C(B_i,B_j) &=\sum_{k}C(B_i,B_j,t_k) B= U\cdot B \\
C(B_i)&=\sum_{j\ge i}C(B_i,B_j)+(j-i)B=(\tfrac{N}{B}-i)U\cdot B + \tfrac{1}{2}(\tfrac{N}{B}-i)(\tfrac{N}{B}-i-1) B \\
C&= \sum_{i}C(B_i) + 1= \sum_{i} {i \choose 1}UB + {i \choose 2}  B+\frac{N}{B} \\
&= UB{\tfrac{N}{B}\choose 2} + B {\tfrac{N}{B} \choose 3} + \frac{N}{B} \\
&= \alpha \frac{UN^2}{B} + \beta\frac{N^3}{B^2} + \gamma \frac{N}{B} \\
&=\alpha' \frac{N^3}{B} + \beta\frac{N^3}{B^2} + \gamma \frac{N}{B} \\
&=\alpha' \frac{N^3}{B} +  \beta \frac{N}{B^2}\\
\frac{\partial C}{\partial B}&=-\alpha' \frac{N^3}{B^2} - \beta'\frac{N}{B^3}\\
\frac{\partial C}{\partial B} = 0 &\iff \alpha' BN^3+\beta' N=0 \\
&\iff B = \left(\frac{}{}\right) 
\end{align*}
$$


S -> E
E -> Declaration
Declaration -> Type VarName Assignment
Type -> ??
VarName -> ??
Assignment -> ??
VarName -> Alphabet AlphaNumericString
VarName -> _ AlphaNumericString
AlphaNumericString -> AlphaNumericString AlphaNumeric 
AlphaNumericString -> $\varepsilon$
AlphaNumeric -> Alpha
AlphaNumeric -> Numeric
AlphaNumeric -> _



Type -> FundamentalType

```c
 int foo;
 typedef int foo;
 foo x;
```

Name