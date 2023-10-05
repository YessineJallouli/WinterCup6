
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
- If $i=0\vee j=0$, Then $G[C']=2$
- Else, $G[C']=0.$ It is guaranteed that there exists such $C'$
In any case, this implies $G[C]=1$
### 2.3 Case 3
Let $\displaystyle C\rightarrow^{i,j}C'$
- If $i=0\vee j=0$, Then $G[C']=1$
- Else, $G[C']=0.$
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