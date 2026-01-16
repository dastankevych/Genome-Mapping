## Program outcome
<img width="754" height="597" alt="image" src="https://github.com/user-attachments/assets/a2168bd4-6b71-457f-bdd1-3bef638d4728" />

## Algorithms Used

#### Suffix Array Construction

```
void build_suffix_array(int n) {
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
            int mid1 = cur + val; if (mid1 >= n) mid1 -= n;
            int mid2 = prev + val; if (mid2 >= n) mid2 -= n;

            if (c[cur] != c[prev] || c[mid1] != c[mid2]) classes++;
            cn[cur] = classes - 1;
        }

        int *temp = c; c = cn; cn = temp;
        if (classes == n) break; // Optimization
    }
}

```

#### Burrows-Wheeler Transform (BWT Construction)

Deriving the BWT string from the constructed Suffix Array.

```
for (int i = 0; i < n; i++) {
    int idx = p[i] - 1;
    if (idx < 0) idx += n;
    bwt[i] = s[idx];
}
```

#### Backward Search with LF-Mapping

Iteratively updating the range $[l, r]$ from the end of the pattern to the beginning using LF-mapping.

```
for (int i = m - 1; i >= 0; i--) {
        char char_sym = pattern[i];
        int id = get_id(char_sym);
        if (id == -1) { *start=1; *end=0; return; }

        l = C_table[(unsigned char)char_sym] + Occ[id][l];
        r = C_table[(unsigned char)char_sym] + Occ[id][r + 1] - 1;

        if (l > r) {
            *start = 1; *end = 0;
            return;
        }
    }
}
```
#### Difference Array

Using a difference array to mark coverage intervals $[L, R]$ in time.

```
if (genome_idx + READ_LEN < n) {
    diff_arr[genome_idx]++;
    diff_arr[genome_idx + READ_LEN]--;
}
```
#### Prefix Sum

Recovering actual coverage depth from the difference array.

```
int current_val = 0;
for(int i=0; i < n - 1; i++) {
    current_val += diff_arr[i]; // Accumulate sum
    if (current_val > 0) covered_bases++;
    total_depth += current_val;
}
```
