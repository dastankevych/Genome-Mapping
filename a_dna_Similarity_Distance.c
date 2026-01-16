#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 1000005
#define K 100
#define INF 0x3f3f3f3f

char s1[MAXN];
char s2[MAXN];
int dp[MAXN];

int min3(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

int main() {
    if (scanf("%s", s1) != 1) return 0;
    if (scanf("%s", s2) != 1) return 0;

    int n = strlen(s1);
    int m = strlen(s2);

    for (int j = 0; j <= m; j++) {
        if (j <= K) dp[j] = j;
        else dp[j] = INF;
    }

    for (int i = 1; i <= n; i++) {
        int lower = i - K;
        if (lower < 1) lower = 1;

        int upper = i + K;
        if (upper > m) upper = m;

        int prev_diag;
        if (lower == 1) prev_diag = i - 1;
        else prev_diag = dp[lower - 1];

        for (int j = lower; j <= upper; j++) {
            int temp = dp[j];

            int del;
            if (j > (i - 1) + K) del = INF;
            else del = temp + 1;

            int ins;
            if (j == lower) ins = INF;
            else ins = dp[j - 1] + 1;

            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            int match = prev_diag + cost;

            dp[j] = min3(del, ins, match);

            prev_diag = temp;
        }
    }

    printf("%d\n", dp[m]);

    return 0;
}

