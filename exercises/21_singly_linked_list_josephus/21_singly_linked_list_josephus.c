#include <stdio.h>
#include <stdlib.h>

#include "singly_linked_list.h"

// 打印节点的值
void print_item(link p) { printf("%d ", p->item); }

// 创建单链表
void create_list(int n) {
    // 参数校验
    if (n <= 0) return;

    destroy();
    for (int i = n; i >= 1; i--) {
        link new_node = make_node(i);
        push(new_node);
    }
}

// 用单链表模拟约瑟夫环问题
// n: 总人数
// k: 起始位置（从 1 开始计数）
// m: 报数阈值（数到 m 的人出列）
void josephus_problem(int n, int k, int m) {
    // 参数校验
    if (n <= 0 || k <= 0 || m <= 0) {
        printf("参数错误：n, k, m 都必须大于 0\n");
        return;
    }

    // 为避免破坏通用链表模块里的全局 head，约瑟夫环使用局部循环链表来模拟。
    link head = NULL;
    link tail = NULL;
    for (int i = 1; i <= n; ++i) {
        link node = make_node((unsigned char)i);
        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    tail->next = head;

    // current 指向当前报数人，prev 指向 current 的前驱。
    link current = head;
    link prev = tail;

    // 移动到第 k 个位置（k 可能大于 n，循环链表天然支持回绕）。
    for (int i = 1; i < k; ++i) {
        prev = current;
        current = current->next;
    }

    int remaining = n;
    while (remaining > 0) {
        // 从 current 开始报数，走 m-1 步后 current 即为要出列的节点。
        for (int cnt = 1; cnt < m; ++cnt) {
            prev = current;
            current = current->next;
        }

        print_item(current);

        if (current == prev) {
            free_node(current);
            break;
        }

        prev->next = current->next;
        link removed = current;
        current = current->next;
        free_node(removed);
        --remaining;
    }

    printf("\n");
}

int main(void) {
    josephus_problem(5, 1, 2);  // 输出结果：2 4 1 5 3
    josephus_problem(7, 3, 1);  // 输出结果：3 4 5 6 7 1 2
    josephus_problem(9, 1, 8);  // 输出结果：8 7 9 2 5 4 1 6 3

    return 0;
}
