#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 1000005

char s[MAXN];
int p_arr[MAXN], c_arr[MAXN], pn_arr[MAXN], cn_arr[MAXN];
int cnt[MAXN];
char out[MAXN];

int main() {
    int *p = p_arr;
    int *c = c_arr;
    int *pn = pn_arr;
    int *cn = cn_arr;

    if (scanf("%s", s) != 1) return 0;

    int n = strlen(s);
    s[n] = '$';
    n++;
    s[n] = '\0';

    memset(cnt, 0, 256 * sizeof(int));

    for (int i = 0; i < n; ++i) cnt[(unsigned char)s[i]]++;
    for (int i = 1; i < 256; ++i) cnt[i] += cnt[i-1];
    for (int i = 0; i < n; ++i) p[--cnt[(unsigned char)s[i]]] = i;

    c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; ++i) {
        if (s[p[i]] != s[p[i-1]]) classes++;
        c[p[i]] = classes - 1;
    }

    for (int h = 0; (1 << h) < n; ++h) {
        if (classes == n) break;

        int val = (1 << h);

        for (int i = 0; i < n; ++i) {
            pn[i] = p[i] - val;
            if (pn[i] < 0) pn[i] += n;
        }

        memset(cnt, 0, classes * sizeof(int));

        for (int i = 0; i < n; ++i) cnt[c[pn[i]]]++;
        for (int i = 1; i < classes; ++i) cnt[i] += cnt[i-1];
        for (int i = n - 1; i >= 0; --i) p[--cnt[c[pn[i]]]] = pn[i];

        cn[p[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; ++i) {
            int cur = p[i];
            int prev = p[i-1];

            int mid1 = cur + val;
            if (mid1 >= n) mid1 -= n;

            int mid2 = prev + val;
            if (mid2 >= n) mid2 -= n;

            if (c[cur] != c[prev] || c[mid1] != c[mid2]) {
                classes++;
            }
            cn[cur] = classes - 1;
        }

        int *temp = c;
        c = cn;
        cn = temp;
    }

    for (int i = 0; i < n; ++i) {
        int idx = p[i] - 1;
        if (idx < 0) idx += n;
        out[i] = s[idx];
    }

    fwrite(out, 1, n, stdout);
    putchar('\n');

    return 0;
}
