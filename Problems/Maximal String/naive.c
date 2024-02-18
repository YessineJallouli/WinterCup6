#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char first;
    int second;
} Pair;

char* contract(char* S) {
    int length = strlen(S);
    Pair* blocks = (Pair*)malloc((length + 1) * sizeof(Pair));
    int blockCount = 0;
    blocks[blockCount].first = '1';
    blocks[blockCount].second = 0;
    for (int i = 0; i < length; i++) {
        if (S[i] == blocks[blockCount].first)
            blocks[blockCount].second++;
        else {
            blockCount++;
            blocks[blockCount].first = S[i];
            blocks[blockCount].second = 1;
        }
    }
    for (int i = 1; i <= blockCount; i++) {
        if (blocks[i].first == '0') {
            int r = blocks[i].second / 2;
            blocks[i].second -= r * 2;
            blocks[i - 1].second += r;
        } else {
            if (blocks[i - 1].second == 1 && blocks[i].second >= 2) {
                blocks[i].second -= 2;
                blocks[i - 1].second++;
            }
        }
    }
    char* ans = (char*)malloc((length + 1) * sizeof(char));
    int ansIndex = 0;
    for (int i = 0; i <= blockCount; i++) {
        for (int j = 0; j < blocks[i].second; j++) {
            ans[ansIndex++] = blocks[i].first;
        }
    }
    ans[ansIndex] = '\0';
    free(blocks);
    return ans;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int n;
        scanf("%d", &n);
        char* S = (char*)malloc((n + 1) * sizeof(char));
        scanf("%s", S);
        char* contracted = contract(S);
        while (strcmp(S, contracted) != 0) {
            free(S);
            S = contracted;
            contracted = contract(S);
        }
        printf("%s\n", contracted);
        free(S);
        free(contracted);
    }
}