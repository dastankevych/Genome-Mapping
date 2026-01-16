#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 1000005
#define MAX_STATES (2 * MAXN)

typedef struct {
    int len;
    int link;
    int next[4];
    long long count;
    int first_pos;
} State;

State st[MAX_STATES];
int sz, last;
char s[MAXN];
char read[105];

int get_id(char c) {
    if (c == 'A') return 0;
    if (c == 'C') return 1;
    if (c == 'G') return 2;
    return 3;
}

void sam_init() {
    st[0].len = 0;
    st[0].link = -1;
    memset(st[0].next, -1, sizeof(st[0].next));
    st[0].count = 0;
    sz = 1;
    last = 0;
}

void sam_extend(int c, int pos) {
    int cur = sz++;
    st[cur].len = st[last].len + 1;
    memset(st[cur].next, -1, sizeof(st[cur].next));
    st[cur].count = 1;
    st[cur].first_pos = pos;

    int p = last;
    while (p != -1 && st[p].next[c] == -1) {
        st[p].next[c] = cur;
        p = st[p].link;
    }

    if (p == -1) {
        st[cur].link = 0;
    } else {
        int q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        } else {
            int clone = sz++;
            st[clone].len = st[p].len + 1;
            memcpy(st[clone].next, st[q].next, sizeof(st[q].next));
            st[clone].link = st[q].link;
            st[clone].count = 0;
            st[clone].first_pos = st[q].first_pos;

            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
}

int sorted_nodes[MAX_STATES];
int cnt_sort[MAXN];

int diff[MAXN];

int main() {
    if (scanf("%s", s) != 1) return 0;
    int n = strlen(s);

    sam_init();
    for (int i = 0; i < n; i++) {
        sam_extend(get_id(s[i]), i);
    }

    memset(cnt_sort, 0, sizeof(int) * (n + 1));
    for (int i = 0; i < sz; i++) cnt_sort[st[i].len]++;
    for (int i = 1; i <= n; i++) cnt_sort[i] += cnt_sort[i-1];
    for (int i = 0; i < sz; i++) sorted_nodes[--cnt_sort[st[i].len]] = i;

    for (int i = sz - 1; i > 0; i--) {
        int u = sorted_nodes[i];
        int link = st[u].link;
        if (link != -1) {
            st[link].count += st[u].count;
        }
    }

    int num_reads;
    if (scanf("%d", &num_reads) != 1) return 0;

    memset(diff, 0, sizeof(int) * (n + 1));

    for (int k = 0; k < num_reads; k++) {
        if (scanf("%s", read) != 1) break;
        int m = strlen(read);
        if (m > n) continue;

        int curr = 0;
        int valid = 1;
        for (int j = 0; j < m; j++) {
            int c = get_id(read[j]);
            if (st[curr].next[c] == -1) {
                valid = 0;
                break;
            }
            curr = st[curr].next[c];
        }

        if (valid && st[curr].count == 1) {
            int end_pos = st[curr].first_pos;
            int start_pos = end_pos - m + 1;

            diff[start_pos]++;
            diff[end_pos + 1]--;
        }
    }

    int coverage = 0;
    int current_val = 0;
    for (int i = 0; i < n; i++) {
        current_val += diff[i];
        if (current_val > 0) {
            coverage++;
        }
    }

    printf("%d\n", coverage);

    return 0;
}
