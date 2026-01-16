#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXN 5120000
#define READ_LEN 50
#define NUM_READS 10000000
#define GENOME_FILENAME "GCF_000005845.2_ASM584v2_genomic.fna"

int *p, *c, *pn, *cn;
int *cnt;
char *s;
char *bwt;

int C_table[256];
int *Occ[4];

int *diff_arr;

int get_id(char c) {
    switch(c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        default: return -1;
    }
}

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
        if (classes == n) break;
    }
}

void build_fm_tables(int n) {
    for (int i = 0; i < n; i++) {
        int idx = p[i] - 1;
        if (idx < 0) idx += n;
        bwt[i] = s[idx];
    }
    bwt[n] = '\0';

    int local_count[256] = {0};
    for (int i = 0; i < n; i++) local_count[(unsigned char)bwt[i]]++;

    int current = 0;

    C_table['$'] = 0; current += local_count['$'];
    C_table['A'] = current; current += local_count['A'];
    C_table['C'] = current; current += local_count['C'];
    C_table['G'] = current; current += local_count['G'];
    C_table['T'] = current; current += local_count['T'];

    for (int k = 0; k < 4; k++) {
        Occ[k] = (int*)calloc(n + 1, sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < 4; k++) Occ[k][i+1] = Occ[k][i];
        int id = get_id(bwt[i]);
        if (id != -1) Occ[id][i+1]++;
    }
}

void backward_search(const char *pattern, int n, int *start, int *end) {
    int l = 0;
    int r = n - 1;
    int m = strlen(pattern);

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
    *start = l;
    *end = r;
}

void read_genome_file() {
    FILE *f = fopen(GENOME_FILENAME, "r");

    char line[1024];
    int pos = 0;
    while(fgets(line, 1024, f)) {
        if (line[0] == '>') continue;
        for(int i=0; line[i]; i++) {
            char c = toupper(line[i]);
            if (strchr("ACGT", c)) {
                s[pos++] = c;
                if (pos >= MAXN - 2) break;
            }
        }
    }
    s[pos] = '$';
    s[pos+1] = '\0';
    fclose(f);
}

int main() {
    s = malloc(MAXN); bwt = malloc(MAXN);
    p = malloc(MAXN * 4); c = malloc(MAXN * 4);
    pn = malloc(MAXN * 4); cn = malloc(MAXN * 4);
    cnt = malloc(MAXN * 4);
    diff_arr = malloc(MAXN * 4);

    printf("Reading genome...\n");
    read_genome_file();
    int n = strlen(s);
    printf("Genome length: %d\n", n-1);

    printf("1. Building Suffix Array\n");
    build_suffix_array(n);

    printf("2. Building FM-Index Tables\n");
    build_fm_tables(n);

    printf("3. Simulating %d reads\n", NUM_READS);
    char **reads = malloc(NUM_READS * sizeof(char*));
    for(int i=0; i<NUM_READS; i++) {
        reads[i] = malloc(READ_LEN + 1);
        int start_pos = rand() % (n - READ_LEN - 1);
        strncpy(reads[i], s + start_pos, READ_LEN);
        reads[i][READ_LEN] = '\0';
    }

    printf("4. Mapping & Coverage\n");
    int mapped = 0, unique = 0, multi = 0;

    memset(diff_arr, 0, n * sizeof(int));

    for(int i=0; i<NUM_READS; i++) {
        int l, r;
        backward_search(reads[i], n, &l, &r);

        if (l <= r) {
            mapped++;
            if (l == r) unique++; else multi++;

            int genome_idx = p[l];

            if (genome_idx + READ_LEN < n) {
                diff_arr[genome_idx]++;
                diff_arr[genome_idx + READ_LEN]--;
            }
        }
    }

    long long covered_bases = 0;
    int current_val = 0;
    long long total_depth = 0;

    for(int i=0; i < n - 1; i++) {
        current_val += diff_arr[i];
        if (current_val > 0) covered_bases++;
        total_depth += current_val;
    }

    printf("\n");
    printf("Total Reads:     %d\n", NUM_READS);
    printf("Mapping Rate:    %.2f%%\n", (double)mapped / NUM_READS * 100);
    printf("Unique/Multi:    %.2f%% / %.2f%%\n",
           mapped ? (double)unique/mapped*100 : 0,
           mapped ? (double)multi/mapped*100 : 0);
    printf("Genome Coverage: %.2f%%\n", (double)covered_bases / (n-1) * 100);
    printf("Avg Depth:       %.2f\n", (double)total_depth / (n-1));

    return 0;
}
