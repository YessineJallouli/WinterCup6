#### LCM Guess :

*This is an interactive problem.*

There a hidden permutation $p$ of length $n$ and you have to find it. For this you have two choose two different integers $i$ and $j$ and the jury will give you $lcm(p_{i}, p_{j})$.

You can make no more than $2n-4$ queries in order to guess $p$.

**Interaction**

The interaction starts after reading the single integer $(3 \le n \le 10^{5})$ — the length of permutation $p$.

To get $lcm(p_{i}, p_{j})$, output the query in format $? \ i \  j$ $(1 \le i, j \le n)$ and $i \neq j$

You can make at most $2n-4$ queries. After the incorrect query you will receive $-1$ and you should exit immediately to get Wrong answer verdict.

When you find $p$, output $p$ in format $! \ p_{1} \ p_{2} ...  p_{n}$. Printing the permutation is not counted as one of $2n-4$ operations.

After printing a query or the answer, do not forget to output a the end of line and flush the output. Otherwise, you will get *Idleness limit exceeded*. To do this, use:

- *fflush(stdout)* or *cout.flush()* in C++;
- *System.out.flush()* in Java;
- *flush(output)* in Pascal;
- *stdout.flush()* in Python;
- see the documentation for other languages.
