#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 6
#define MAX_DEGREE 5
static const char labels[N] = {'A','B','C','D','E','F'};
static int matrix[N][N];
static int adj[N][MAX_DEGREE];
static int degree[N];

static int index_of(char c) {
    for (int i = 0; i < N; ++i) if (labels[i] == c) return i;
    return -1;
}
static void add_edge(char a, char b) {
    int u = index_of(a), v = index_of(b);
    if (u < 0 || v < 0) return;
    matrix[u][v] = matrix[v][u] = 1;
    adj[u][degree[u]++] = v;
    adj[v][degree[v]++] = u;
}
static void print_matrix(void) {
    printf("Adjacency matrix (row/column order A B C D E F):\n    ");
    for (int i=0;i<N;++i) printf("%c ", labels[i]);
    printf("\n");
    for (int i=0;i<N;++i) {
        printf("%c : ", labels[i]);
        for (int j=0;j<N;++j) printf("%d ", matrix[i][j]);
        printf("\n");
    }
}
static void print_list(void) {
    printf("Adjacency list (neighbors in alphabetical order):\n");
    for (int i=0;i<N;++i) {
        printf("%c:", labels[i]);
        for (int j=0;j<degree[i];++j) printf(" %c", labels[adj[i][j]]);
        printf("\n");
    }
}
static void bfs_matrix(int start) {
    int seen[N]={0}, q[N], front=0, back=0;
    seen[start]=1; q[back++]=start;
    printf("BFS matrix: ");
    while(front<back) {
        int u=q[front++]; printf("%c ",labels[u]);
        for(int v=0;v<N;++v) if(matrix[u][v] && !seen[v]) { seen[v]=1; q[back++]=v; }
    }
    printf("\n");
}
static void bfs_list(int start) {
    int seen[N]={0}, q[N], front=0, back=0;
    seen[start]=1; q[back++]=start;
    printf("BFS list:   ");
    while(front<back) {
        int u=q[front++]; printf("%c ",labels[u]);
        for(int j=0;j<degree[u];++j) { int v=adj[u][j]; if(!seen[v]) { seen[v]=1; q[back++]=v; } }
    }
    printf("\n");
}
static void dfs_matrix_visit(int u, int seen[]) {
    seen[u]=1; printf("%c ",labels[u]);
    for(int v=0;v<N;++v) if(matrix[u][v] && !seen[v]) dfs_matrix_visit(v,seen);
}
static void dfs_list_visit(int u, int seen[]) {
    seen[u]=1; printf("%c ",labels[u]);
    for(int j=0;j<degree[u];++j) { int v=adj[u][j]; if(!seen[v]) dfs_list_visit(v,seen); }
}
static void search_matrix(int start, int target) {
    int seen[N]={0}, q[N], front=0, back=0, checks=0, found=0;
    seen[start]=1; q[back++]=start;
    while(front<back && !found) {
        int u=q[front++];
        if(u==target) { found=1; break; }
        for(int v=0;v<N;++v) {
            ++checks;
            if(matrix[u][v] && !seen[v]) { seen[v]=1; q[back++]=v; }
        }
    }
    printf("Search F with matrix (BFS from A): %s; adjacency-cell checks=%d\n", found?"FOUND":"NOT FOUND",checks);
}
static void search_list(int start, int target) {
    int seen[N]={0}, q[N], front=0, back=0, checks=0, found=0;
    seen[start]=1; q[back++]=start;
    while(front<back && !found) {
        int u=q[front++];
        if(u==target) { found=1; break; }
        for(int j=0;j<degree[u];++j) {
            int v=adj[u][j]; ++checks;
            if(!seen[v]) { seen[v]=1; q[back++]=v; }
        }
    }
    printf("Search F with list (BFS from A):   %s; neighbor-entry checks=%d\n", found?"FOUND":"NOT FOUND",checks);
}
int main(int argc, char **argv) {
    if(argc != 2) { fprintf(stderr,"Usage: %s input.txt\n",argv[0]); return 1; }
    FILE *f=fopen(argv[1],"r");
    if(!f) { perror("Cannot open input file"); return 1; }
    int n,m; char start_label,target_label;
    if(fscanf(f,"%d %d %c %c",&n,&m,&start_label,&target_label)!=4 || n!=N) {
        fprintf(stderr,"Input must contain: 6 edge_count start target, followed by edges.\n"); fclose(f); return 1;
    }
    for(int i=0;i<m;++i) { char a,b; if(fscanf(f," %c %c",&a,&b)!=2) { fprintf(stderr,"Invalid edge on line %d.\n",i+2); fclose(f); return 1; } add_edge(a,b); }
    fclose(f);
    int s=index_of(start_label), t=index_of(target_label);
    if(s<0 || t<0) { fprintf(stderr,"Start/target must be A-F.\n"); return 1; }
    printf("Vertices=%d, undirected edges=%d, start=%c, search target=%c\n\n",n,m,start_label,target_label);
    print_matrix(); printf("\n"); print_list(); printf("\n");
    bfs_matrix(s); bfs_list(s);
    printf("DFS matrix: "); int seen1[N]={0}; dfs_matrix_visit(s,seen1); printf("\n");
    printf("DFS list:   "); int seen2[N]={0}; dfs_list_visit(s,seen2); printf("\n\n");
    search_matrix(s,t); search_list(s,t);
    return 0;
}
