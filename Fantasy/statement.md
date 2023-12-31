#### Fantasy :

Yessine plays a game called Fantasy. In this game, each players picks $L$ footballers into his team out of $F$ possible footballers, then each footballer gets a score each week according to their performance. The maximum possible score is $M$ and the minimum possible score is -$M$. The weekly score of a Fantasy player is the sum of the scores of each one of his footballers. There are $W$ weeks left in this season. The total score of the player at the end of the season is the sum of his weekly scores.

There are $N$ people playing this game. They are ranked in a scoreboard $S$ where the $i$th player has a total score $S_i$. Yessine currently has rank $R$ . He wants to finish first so badly. He got addicted to the game and started skipping his competitive programming training. Oussama has noticed that so he decided to save him from his addiction.  Since he knows that Yessine likes challenges only, He will help him win this season so he would get bored and quit the game for good.

The $i$th footballer usually gets weekly score $P_i$ . Each week, Oussama can do the following operation :               He will travel to put $x$ kilos of Harissa in the footballer's meal, so his stomach would hurt and his score will decrease by $x$ (but he can't make it less than -$M$) . He can do this to any number of footballers any number of times  (he has no ethics or morals, plus he's from Nabeul so he has an infinite amount of Harissa).

Find out if Oussama can help Yessine rank first in the game.

**Input**

The first line contains six integers $N$,$M$,$L$,$F$,$W$,$R$ .  $(  1\le N,F,R \le10^5 , 1 \le M,L \le 10,  1 \le W \le 50 )$

The second line contains $N$ integers (sorted in descending order) denoting the scoreboard $S$ $(  1 \le S_i \le 10^6 )$ where Yessine is the $R$th player.

The second line contains $F (  1\le F \le10^5 )$ integers denoting the sequence $P(  -M  \le P_i \le M )$

$N$ lines follow, each line contains $L$ integers. The $i$th line denotes the footballers chosen by the $i$th player.

**Output**

Print "Yes" if Oussama can make Yessine win or "No" otherwise