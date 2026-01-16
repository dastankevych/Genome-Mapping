#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 1000005

char s[MAXN];
int T[MAXN];
int count[256];
int start[256];
char res[MAXN];

int main() {
    if (scanf("%s", s) != 1) return 0;
    int n = strlen(s);

    for (int i = 0; i < n; i++) {
        count[(unsigned char)s[i]]++;
    }

    int current = 0;
    for (int i = 0; i < 256; i++) {
        if (count[i] > 0) {
            start[i] = current;
            current += count[i];
        }
    }

    int idx = 0;
    for (int i = 0; i < n; i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == '$') idx = i;

        T[i] = start[c];

        start[c]++;
    }

    int curr = idx;
    for (int i = 0; i < n - 1; i++) {
        curr = T[curr];
        res[n - 2 - i] = s[curr];
    }
    res[n-1] = '\0';

    fwrite(res, 1, n - 1, stdout);
    putchar('\n');

    return 0;
}
