# Question 6: Graph Representations and Traversal

## Problem and assumptions

The undirected social network has vertices `A, B, C, D, E, F` and edges `A-B, A-C, B-D, B-E, C-F, E-F`. The start vertex is `A`. Since the question does not specify a search target, this solution searches for `F`. Neighbors are processed alphabetically, so traversal order is reproducible. Each undirected edge is stored in both directions.

## Representation

The adjacency matrix uses rows and columns in order A, B, C, D, E, F:

|   | A | B | C | D | E | F |
|---|---:|---:|---:|---:|---:|---:|
| A | 0 | 1 | 1 | 0 | 0 | 0 |
| B | 1 | 0 | 0 | 1 | 1 | 0 |
| C | 1 | 0 | 0 | 0 | 0 | 1 |
| D | 0 | 1 | 0 | 0 | 0 | 0 |
| E | 0 | 1 | 0 | 0 | 0 | 1 |
| F | 0 | 0 | 1 | 0 | 1 | 0 |

Adjacency list:

```text
A: B C
B: A D E
C: A F
D: B
E: B F
F: C E
```

## C program and execution

`graph.c` reads the vertex/edge count, start vertex, search target, and edge pairs from `input.txt`. It builds and prints both representations, runs BFS and DFS from A, and searches for F using BFS. `output.txt` contains the captured program output.

Compile and run in this folder:

```sh
cc -std=c11 -Wall -Wextra -pedantic graph.c -o graph
./graph input.txt
```

## BFS trace from A

BFS uses a FIFO queue and marks vertices visited when enqueued, preventing duplicate enqueues.

| Step | Vertex removed | Newly discovered (in order) | Queue after step |
|---:|:---:|:---|:---|
| 1 | A | B, C | B, C |
| 2 | B | D, E | C, D, E |
| 3 | C | F | D, E, F |
| 4 | D | — | E, F |
| 5 | E | — | F |
| 6 | F | — | empty |

**BFS order:** `A, B, C, D, E, F`.

## DFS trace from A

DFS recursively follows the first unvisited neighbor alphabetically, then backtracks when there are no unvisited neighbors.

| Step | Action | Current path / result |
|---:|:---|:---|
| 1 | Visit A; choose B | A, B |
| 2 | From B, choose D | A, B, D |
| 3 | D has no unvisited neighbor; backtrack to B | A, B |
| 4 | From B, choose E | A, B, E |
| 5 | From E, choose F | A, B, E, F |
| 6 | From F, choose C | A, B, E, F, C |
| 7 | C has no unvisited neighbor; finish/backtrack | complete |

**DFS order:** `A, B, D, E, F, C`.

Both representations produce the same order under this neighbor ordering.

## Search trace for F

The search is BFS from A and stops when F is removed from the queue. Counts are adjacency inspections; queue and visited-array operations are not included.

| Representation | Vertices expanded before F is found | Inspection trace | Checks |
|:---|:---|:---|---:|
| Matrix | A, B, C, D, E | 6 cells in each expanded row | 5 × 6 = **30 cells** |
| List | A, B, C, D, E | degrees 2 + 3 + 2 + 1 + 2 | **10 neighbor entries** |

F is discovered while processing C, but the search reports it when F is removed from the queue. Both searches return **FOUND**. Stating this stopping rule matters because stopping as soon as F is discovered would produce different counts.

## Complexity analysis

Let `V` be the number of vertices and `E` the number of undirected edges.

| Operation | Adjacency matrix | Adjacency list |
|:---|:---|:---|
| Storage | Θ(V²) | Θ(V + E) |
| Check whether edge (u,v) exists | Θ(1) | O(degree(u)); worst case O(V) |
| Enumerate neighbors of u | Θ(V) | Θ(degree(u)) |
| BFS / DFS | Θ(V²) | Θ(V + E) |
| Search for a vertex by traversal from A | O(V²) worst case | O(V + E) worst case |
| Auxiliary traversal space | O(V) | O(V) |

List traversal scans each reachable vertex and its stored neighbor entries. Matrix traversal scans a full row for each visited vertex. For a disconnected graph these traversal bounds concern the reachable component; matrix storage and initialization still require Θ(V²).

## Comparison and conclusion

Here `V = 6`, `E = 6`, and graph density is `2E / (V(V−1)) = 12/30 = 0.4`. The matrix reserves 36 cells, including 24 zeros; the list stores 12 neighbor entries (two per undirected edge). In the sample search, the matrix inspects 30 cells and the list inspects 10 neighbor entries. Both BFS and DFS visit all six vertices, but the matrix scans six cells per expanded vertex while the list scans only actual neighbors.

For a **sparse social network**, the **adjacency list is generally more suitable** because it uses Θ(V+E) space and traversal work follows existing connections. A matrix is useful when the graph is dense or constant-time edge-existence checks matter most. For this graph and these traversal/search tasks, the list is the better fit.

## Submission files

- `graph.c` — C source
- `input.txt` — graph input
- `output.txt` — execution output
- `report.md` — traces, complexity analysis, comparison, and conclusion
