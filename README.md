# YingYangSolver

A collection of C programs to solve the Yazheng-YinYang puzzle on a 4×4 grid using three different traversal algorithms: **Breadth-First Search (BFS)**, **Depth-First Search (DFS)**, and a **Binary Tree Recursion** approach. Each solver generates all valid puzzle configurations that satisfy the constraints:

1. No 2×2 block is entirely the same color (all black or all white).
2. All black cells form one contiguous region.
3. All white cells form one contiguous region.

## Files and Algorithms

### 1. `yingyang_BFS.c`

**Algorithm**: Implements a **Breadth-First Search** using a queue data structure (`Queue`). Starting from an empty board, it enqueues all valid next states (flipping one cell at a time), level by level, until all 16 positions are filled.

* **Node Structure**: Stores a 16-bit `board` mask, a 16-bit `status` mask (which cells are assigned), and counters for black/white counts.
* **Constraint Checks**: `check2x2()` avoids invalid 2×2 blocks; `Continuity()` ensures contiguity of colors via recursive neighbor counting.
* **Output**: When a complete configuration is found, it is inserted into a linked list and printed immediately with `showBoard()`, using ANSI colors: `#` (black) in red, `O` (white) in cyan, and `+` (unassigned) in white. Finally, the total number of nodes visited is printed.

### 2. `yingyang_DFS.c`

**Algorithm**: Performs a **Depth-First Search** recursively. It explores one branch fully (assigning black or white at each next cell index) before backtracking.

* **Recursion**: `generate(head, board, status, blacks, whites)` drills down until `status == (1<<16)-1`.
* **Pruning**: At each step, both `check2x2()` and `Continuity()` are called to skip invalid partial assignments.
* **Output**: Valid solutions are inserted into a linked list. After generation, `showBoards()` prints each solution with colored grids (black `#`, white `O`), separated by lines, and summary count of found solutions.

### 3. `yingyang_binaryTree.c`

**Algorithm**: A specialized **Binary Tree Recursion** that treats the assignment of each of the 16 positions as a binary tree: left child = assign white, right child = assign black at position `status`.

* **Node Allocation**: No queue; uses pure recursion over index `status` from 0 to 16.
* **Check Functions**: Simplified `check2x2()` and `Continuity()` for the binary tree context.
* **Output**: After generating and collecting all valid boards, `showBoard(head, 0)` iterates linked list to print each solution board and a final line: `Number of correct boards: X`. It also reports total processing time in seconds.

## Output Format

All three programs print each valid 4×4 board using ASCII characters and ANSI color codes:

* **`#`** (black cell): printed in **red**.
* **`O`** (white cell): printed in **cyan**.
* **`+`** (unassigned or background in BFS): printed in **white**.

Example 4×4 solution snapshot (colors shown as labels):

```
# O # +
O # O #
# O O #
+ # O O
```

At the end of execution:

* **BFS**: prints count of visited nodes.
* **DFS**: prints total solutions found.
* **Binary Tree**: prints total solutions and processing time.

## Compilation and Usage

Compile any solver with:

```bash
gcc -o solver_name file.c -std=c99 -O2
```

Run:

```bash
./solver_name
```

The programs run on the fixed 4×4 grid and output directly to the console.
