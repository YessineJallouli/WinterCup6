#### New Language :

Oussama has decided to make his own programming language. He prepared the specifications and sent them to Rami who will write a compiler for him. While doing this task, Rami encountered a problem : sometimes, Rami will take some $N$ data chunks of different sizes and try to store them in memory. The $i$th  data chunk has size $D_i$. The memory is composed of words, each word has size $W$. His algorithm will put the data in memory using the following the specs :

* A single word can store multiple data chunks if the sum of their sizes is less than or equal to $W$
* A data chunk of size $D_i$  can be divided into smaller chunks where the sum of their sizes is $D_i$
* The data chunks can be processed in any order
* The number of words used should be minimal

Rami's implementation can't pass the tests prepared by Oussama, hence we ask you to write an algorithm that can pass them.

**Input**

The first line contains two integers $N$ and $W$ .  ( $ 1\le N \le10^5 $ , $ 1 \le W \le 10^9 $)

The second line contains $N$ integers, the $i$th integer denotes $D_i$ . ( $ 1 \le D_i \le 10^9 $)

**Output**

Print the minimum number of words used to store the data.