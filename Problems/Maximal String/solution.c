#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char c;
    int count;
} CharBlock;

char* contract(char* S) {
    int length = strlen(S);
    CharBlock* blocks = (CharBlock*)malloc((length+1) * sizeof(CharBlock));
    int blockCount = 0;
    blocks[0].c = '1';
    blocks[0].count = 0;

    for (int i = 0; i < length; ++i) {
        if (S[i] == blocks[blockCount].c) {
            blocks[blockCount].count++;
        } else {
            blockCount++;
            blocks[blockCount].c = S[i];
            blocks[blockCount].count = 1;
        }
    }
    blockCount++;

    for (int i = 1; i < blockCount; ++i) {
        if (blocks[i].c == '0') {
            int r = blocks[i].count / 2;
            blocks[i].count -= r * 2;
            blocks[i - 1].count += r;
        }
    }

    for (int i = blockCount - 1; i >= 0; --i) {
        if (i > 0 && blocks[i].c == '1' && blocks[i - 1].count && blocks[i].count >= 2) {
            blocks[i].count -= 2;
            blocks[i - 1].count--;
            blocks[i - 2].count++;
        }
    }

    char* ans = (char*)malloc((length + 1) * sizeof(char));
    int pos = 0;
    for (int i = 0; i < blockCount; ++i) {
        for (int j = 0; j < blocks[i].count; ++j) {
            ans[pos++] = blocks[i].c;
        }
    }
    ans[pos] = '\0';

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
    return 0;
}
