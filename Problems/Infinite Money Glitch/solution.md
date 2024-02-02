- Let $G=(V,E,W)$ be a functional graph where:
	- $V$ is a finite set of vertices
	- $E\subseteq V\times V$ is the set of edges:
	- $W:E\rightarrow \mathscr{F}(\mathbb{R},\mathbb{R})$ is the functional weights that are non-decreasing
- Let $\mathcal{W}$ be the set of walks over $G,$ and let $\mathcal{W}_{a,b}$ the set of walks that starts at $a\in V$ and finishes at $b\in V$ 

Let $x\in\mathbb{R},$ and $a,b\in V:$ we want to calculate:
$$
\DeclareMathOperator*{\BigCompose}{\Large \bigcirc}
\Phi(x,a,b)=\sup_{\Pi \in \mathcal{W}_{a,b}} \left(\BigCompose_{e\in \Pi^R}W(e)\right)(x)
$$
Let $(\varphi_n)_{n\in\mathbb{N}}$ as sequence of functions such that:
$$
\DeclareMathOperator{\Adj}{Adj}
\begin{align*}
\varphi_{0}(x,a,s)&=0 & \text{if} \ s\neq a \\
\varphi_{0}(x,a,s)&=x & \text{if} \ s= a \\
\varphi_{n}(x,a,s)&= \max\left(\varphi_{n-1}(x,a,s),\max_{t\in \Adj^{-1}s} W(t,s)(\varphi_{n-1}(x,a,t))\right) 
\end{align*}
$$

Now, by definition, this is a point-wise increasing function for fixed $x\in\mathbb{R},a,b\in V.$ 
So it converges point-wise to a function $\varphi^*$ that satisfies:
$$
\varphi^*(x,a,s)\ge\max_{t\in \Adj^{-1}s} W(t,s)(\varphi^*(x,a,t))
$$
Also, this is also verified in the sequence $(\varphi_n)_{n\in\mathbb{N}}$ as:
$$
\varphi_n(x,a,s)\ge\max_{t\in \Adj^{-1}s} W(t,s)(\varphi_{n-1}(x,a,t))
$$
## 2. Lower Bound
We will prove by induction that:
$$
\forall n\in\mathbb{N},\forall t \in V,\forall \Pi \in\mathcal{W}_{a,t} \ \text{of length}\ n, \quad \varphi_n(x,a,t)\ge \left(\BigCompose_{e\in \Pi^R} W(e)\right)(x)
$$
### 2.1 Base Case
For $n=0,$ we have $\Pi$ does not consist of any edges. And the result is trivial:
$$
\varphi_0(x,a,a)\ge \text{Id}(x) = x 
$$
### 2.2 Induction
- Let $n\in\mathbb{N}^*,$ and let $t\in V$ and $\Pi\in \mathcal{W}_{a,t}$ a path of length $n$
- Let $e_1,\dots,e_n$ be the edges of $\Pi$
- Let $\Pi^*=(e_1,\dots,e_{n-1})$
- Let $s$ be the second endpoint of $e_n$
$$
\begin{align*}
\varphi_n(x,a,t) &= \max\left(\varphi_{n-1}(x,a,s),\max_{r\in \Adj^{-1}t} W(r,t)(\varphi_{n-1}(x,a,r))\right)  \\
&\ge \max_{r\in \Adj^{-1}t} W(r,t)(\varphi_{n-1}(x,a,r)) \\
&\ge W(s,t)(\varphi_{n-1}(x,a,s)) \quad\text{as} \ s\in \Adj^{-1}t \\
\varphi_{n-1}(x,a,s) &\ge \left(\BigCompose_{e\in {\Pi^*}^R} W(e)\right)(x) \\
\implies \varphi_n(x,a,t) &\ge  W(s,t)\left(\left(\BigCompose_{e\in {\Pi^*}^R} W(e)\right)(x)\right) \quad\text{as} \ W(s,t) \ \text{is non-decreasing} \\
&\ge  W(e_n)\left(\left(\BigCompose_{i=n-1}^{1} W(e_i)\right)(x)\right) \\ 
&\ge \left(\BigCompose_{i=n}^{1} W(e_i)\right)(x) \\
&\ge \left(\BigCompose_{e\in \Pi^R} W(e)\right)(x)
\end{align*}
$$
With that, as $(\varphi_n)_{n\in\mathbb{N}}$ is an increasing sequence of functions:
$$
\begin{align*}
\varphi_n(x,a,b)&\ge \left(\BigCompose_{e\in \Pi^R} W(e)\right)(x) \\
\implies \varphi^*(x,a,b) & \ge  \left(\BigCompose_{e\in \Pi^R} W(e)\right)(x) \\
\end{align*}
$$
Now we have:
$$
\forall a,b\in V,\forall \Pi\in\mathcal{W}_{a,b}, \quad \varphi^{*}(x,a,b) \ge \left(\BigCompose_{e\in \Pi^R} W(e)\right)(x) 
$$
We conclude that:
$$
\forall a,b\in V,\quad \varphi^{*}(x,a,b) \ge \sup_{\Pi \in \mathcal{W}_{a,b}} \left(\BigCompose_{e\in \Pi^R}W(e)\right)(x) = \Phi(x,a,b)
$$
## 3. Upper Bound
Also, it is trivial that:
$$
\begin{align*}
\forall n\in\mathbb{N},\quad \varphi_n(x,a,b) & \le \sup_{\substack{\Pi \in \mathcal{W}_{a,b} \\ \lvert \Pi \rvert \le n   }} \left(\BigCompose_{e\in \Pi^R}W(e)\right)(x) \\
&\le \Phi(x,a,b) \\
\implies \varphi^*(x,a,b)& \le \Phi(x,a,b) 
\end{align*}
$$
With that the algorithm converges to the desired solution

