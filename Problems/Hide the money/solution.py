import heapq
from functools import cached_property


def H(x:int,n:int):
    return n*(n-1)//2-x*(n-1-x)

class GridCell:
    n:int
    m:int
    def __init__(self, x, y):
        self.x=x
        self.y=y
        self.value=GridCell.m*H(self.x,GridCell.n)+GridCell.n*H(self.y,GridCell.m)

    def __lt__(self, other):
        return self.value>other.value
    def __ge__(self, other):
        return self.value<=other.value
    def __le__(self, other):
        return self.value>=other.value
    def __gt__(self, other):
        return self.value<other.value
    def __eq__(self, other):
        return self.x==other.x and self.y==other.y

    def __ne__(self, other):
        return not self.__eq__(other)

    def __hash__(self):
        return hash((self.x,self.y))


class BigBool:
    def __init__(self,*dimensions,block_size=64):
        self.dimensions=dimensions
        self.size=1
        for d in dimensions:
            self.size*=d
        self.data=[0 for _ in range((self.size+block_size-1)//block_size)]
        self.block_size=block_size
    def __getitem__(self, pos):
        index=0
        for i in range(len(pos)):
            index=pos[i]+index*self.dimensions[i]
        return bool(self.data[index//self.block_size]&(1<<(index%self.block_size)))

    def __setitem__(self, pos, value:bool):
        index = 0
        for i in range(len(pos)):
            index = pos[i] + index * self.dimensions[i]
        if value:
            self.data[index//self.block_size]|=1<<(index%self.block_size)
        else:
            self.data[index//self.block_size]&=~(1<<(index%self.block_size))

T=int(input())
N_max=10000
M_max=10000
for _ in range(T):
    n,m,k=map(int,input().split())
    GridCell.n=n
    GridCell.m=m
    total_sum=0
    L = []
    visited = set()
    for i in [0,n-1]:
        for j in [0,m-1]:
            heapq.heappush(L,GridCell(i,j))
            visited.add((i,j))
    count = 0
    last_value=0
    while count< k:
        cell=heapq.heappop(L)
        last_value=cell.value
        total_sum+=cell.value
        count+=1
        for i in [-1,0,1]:
            for j in [-1,0,1]:
                if 0<=cell.x+i<n and 0<=cell.y+j<m and not (i==0 and j==0) and (cell.x+i,cell.y+j) not in visited:
                    heapq.heappush(L,GridCell(cell.x+i,cell.y+j))
                    visited.add((cell.x+i,cell.y+j))
    print(total_sum)
