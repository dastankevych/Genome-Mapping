#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 1000005

char s[MAXN];
int p[MAXN], c[MAXN], pn[MAXN], cn[MAXN];
int cnt[MAXN]; 

int main() {
    if (scanf("%s", s) != 1) return 0;
    
    int n = strlen(s);
    s[n] = '$';
    s[n+1] = '\0';
    n++;

    memset(cnt, 0, sizeof(cnt));

    for (int i = 0; i < n; ++i) cnt[(int)s[i]]++;
    
    for (int i = 1; i < 256; ++i) cnt[i] += cnt[i-1];
    
    for (int i = 0; i < n; ++i) p[--cnt[(int)s[i]]] = i;
    
    c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; ++i) {
        if (s[p[i]] != s[p[i-1]]) classes++;
        c[p[i]] = classes - 1;
    }

    for (int h = 0; (1 << h) < n; ++h) {
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
            
            int mid1 = (cur + val) % n;
            int mid2 = (prev + val) % n;
            
            if (c[cur] != c[prev] || c[mid1] != c[mid2]) {
                classes++;
            }
            cn[cur] = classes - 1;
        }
        
        memcpy(c, cn, n * sizeof(int));
    }

    for (int i = 0; i < n; ++i) {
        int idx = (p[i] - 1 + n) % n;
        putchar(s[idx]);
    }
    putchar('\n');

    return 0;
}

