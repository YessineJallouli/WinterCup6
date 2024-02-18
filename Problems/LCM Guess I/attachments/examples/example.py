import math
def query(i:int,j:int):
	print("?",i,j,sep=' ',flush=True)
	return int(input())
	
def answer(permutation):
	print("!",*permutation, sep=' ')
	


n=int(input())
#Since the permutation is unknown, it is initialized to zero. You have to guess it!
permutation=[0]*n
#Your solution goes here. Do not forget to output the permutation once you are able to guess it.

