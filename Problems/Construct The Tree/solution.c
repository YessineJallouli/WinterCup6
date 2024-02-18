#include <stdio.h>
#include <stdbool.h>
#include <memory.h>

const int N = 1e5+7;

int main() {
    int n;
    int furthest[N];
    bool leaf[N];
    memset(leaf,false,sizeof(bool)*N);
    scanf("%d",&n);
    for (int i = 1; i <= n; i++) {
        scanf("%d",&furthest[i]);
        if (furthest[i] == i) {
            printf("NO\n");
            return 0;
        }
        leaf[furthest[i]]=true;
    }
    bool all_leafs=true;
    for(int i=1;i<=n;i++) if(!leaf[i]) all_leafs=false;
    if (all_leafs)
        printf("NO\n");
    else
        printf("YES\n");

}
