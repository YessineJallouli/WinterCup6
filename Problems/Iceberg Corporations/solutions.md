Let $S(a,b)$ be defined as follows:
$$
\begin{align}
S(a,b)&=\sum_{i=0}^{n-1}\sum_{j=0}^{m-1} \lvert a-i\rvert + \lvert b-j\rvert \\
&=m\sum_{i=0}^n \lvert a-i \rvert + n\sum_{j=0}^m \lvert b-j\rvert \\
&=m H(a,n)+ nH(b,m) \\
H(a,n)&= \sum_{i=0}^n\lvert a-i\rvert \\
&=\sum_{i=0}^{a-1} a-i + \sum_{i=a}^{n-1} i-a \\
&=\sum_{i=1}^{a} i + \sum_{i=a}^{n-1}i-a \\
&=\sum_{i=0}^{n-1} i-(n-a)a +a\\
&= \frac{n(n-1)}{2}-(n-a-1)a \\
H(b,m)&=\frac{m(m-1)}{2}-(m-b-1)b
\end{align}
$$