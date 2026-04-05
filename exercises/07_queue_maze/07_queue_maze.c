#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5

typedef struct {
	int row;
	int col;
} Node;

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

int main(void)
{
	Node queue[MAX_ROW * MAX_COL];
	Node parent[MAX_ROW][MAX_COL];
	Node path[MAX_ROW * MAX_COL];
	int visited[MAX_ROW][MAX_COL] = {0};
	int dr[4] = {-1, 0, 1, 0};
	int dc[4] = {0, -1, 0, 1};
	int front = 0;
	int rear = 0;
	int found = 0;
	Node end = {-1, -1};

	for (int r = 0; r < MAX_ROW; r++) {
		for (int c = 0; c < MAX_COL; c++) {
			parent[r][c].row = -1;
			parent[r][c].col = -1;
		}
	}

	queue[rear].row = 0;
	queue[rear].col = 0;
	rear++;
	visited[0][0] = 1;

	while (front < rear) {
		Node cur = queue[front];
		front++;

		if (cur.row == MAX_ROW - 1 && cur.col == MAX_COL - 1) {
			found = 1;
			end = cur;
			break;
		}

		for (int dir = 0; dir < 4; dir++) {
			int next_row = cur.row + dr[dir];
			int next_col = cur.col + dc[dir];

			if (next_row < 0 || next_row >= MAX_ROW || next_col < 0 || next_col >= MAX_COL) {
				continue;
			}
			if (maze[next_row][next_col] != 0 || visited[next_row][next_col]) {
				continue;
			}

			queue[rear].row = next_row;
			queue[rear].col = next_col;
			rear++;
			visited[next_row][next_col] = 1;
			parent[next_row][next_col] = cur;
		}
	}

	if (!found) {
		printf("No path!\n");
		return 0;
	}

	int path_len = 0;
	Node cur = end;

	while (cur.row != -1 && cur.col != -1) {
		path[path_len++] = cur;
		cur = parent[cur.row][cur.col];
	}

	for (int i = 0; i < path_len; i++) {
		printf("(%d, %d)\n", path[i].row, path[i].col);
	}

	return 0;
}