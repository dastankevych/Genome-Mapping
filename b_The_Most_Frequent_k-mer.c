#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 1000005

typedef unsigned long long ull;

typedef struct {
    ull hash;
    int id;
} Entry;

char s[MAXN];
Entry entries[MAXN];

int cmp(const void *a, const void *b) {
    ull h1 = ((Entry*)a)->hash;
    ull h2 = ((Entry*)b)->hash;
    if (h1 < h2) return -1;
    if (h1 > h2) return 1;
    return 0;
}

int val(char c) {
    if (c == 'A') return 1;
    if (c == 'C') return 2;
    if (c == 'G') return 3;
    return 4;
}

int main() {
    int k;
    
    if (scanf("%s", s) != 1) return 0;
    if (scanf("%d", &k) != 1) return 0;
    
    int n = strlen(s);
    if (k > n) return 0;

    ull base = 37;
    
    ull power = 1;
    for (int i = 0; i < k - 1; i++) {
        power *= base;
    }

    ull current_hash = 0;
    for (int i = 0; i < k; i++) {
        current_hash = current_hash * base + val(s[i]);
    }

    int count = 0;
    entries[count].hash = current_hash;
    entries[count].id = 0;
    count++;

    for (int i = 1; i <= n - k; i++) {
        current_hash -= val(s[i - 1]) * power;
        current_hash *= base;
        current_hash += val(s[i + k - 1]);
        
        entries[count].hash = current_hash;
        entries[count].id = i;
        count++;
    }

    qsort(entries, count, sizeof(Entry), cmp);

    int max_freq = 0;
    int best_id = 0;
    
    int current_freq = 1;
    for (int i = 1; i < count; i++) {
        if (entries[i].hash == entries[i - 1].hash) {
            current_freq++;
        } else {
            if (current_freq > max_freq) {
                max_freq = current_freq;
                best_id = entries[i - 1].id;
            }
            current_freq = 1;
        }
    }
    if (current_freq > max_freq) {
        max_freq = current_freq;
        best_id = entries[count - 1].id;
    }

    for (int i = 0; i < k; i++) {
        putchar(s[best_id + i]);
    }
    putchar('\n');

    return 0;
}

