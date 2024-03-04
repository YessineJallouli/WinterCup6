#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 300007

int a[N], p[N];
int st[N]; // Using a simple array as a set substitute
int indexes[N];

void test_case() {
    int n;
    scanf("%d", &n);
    memset(a, -1, sizeof(a));
    memset(st, 0, sizeof(st));
    for (int i = 0; i < n; i++) {
        scanf("%d", &p[i]);
    }
    if (p[0]) {
        printf("-1\n");
        return;
    }
    a[0] = 1;
    int cur = 2;
    for (int i = 1; i < n; i++) {
        if (p[i] >= i + 1) {
            printf("-1\n");
            return;
        }
        int len = p[i - 1];
        if (p[i] > p[i - 1] + 1) {
            printf("-1\n");
            return;
        }
        if (p[i] == 0) {
            a[i] = cur++;
            continue;
        }
        int st_size = 0;
        while (len && len + 1 != p[i]) {
            if (!st[a[len]])
            {
                st[a[len]] = 1;
                indexes[st_size] = a[len];
                st_size++;
            }
            len = p[len - 1];
            if (len + 1 < p[i]) {
                printf("-1\n");
                return;
            }
        }
        for(int j = 0; j < st_size; j++)
            st[indexes[j]] = 0;
        if (len + 1 == p[i]) {
            a[i] = a[len];
            if (st[a[i]]) {
                printf("-1\n");
                return;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        test_case();
    }
    return 0;
}

