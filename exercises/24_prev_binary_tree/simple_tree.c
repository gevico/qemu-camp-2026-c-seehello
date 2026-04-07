#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    if(q == NULL || tree_node == NULL) {
        return;
    }

    QueueNode *new_node = (QueueNode*)malloc(sizeof(QueueNode));
    new_node->tree_node = tree_node;
    new_node->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

TreeNode* dequeue(Queue *q) {
    if (q == NULL || q->front == NULL) {
        return NULL;
    }

    QueueNode *temp = q->front;
    TreeNode *tree_node = temp->tree_node;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return tree_node;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    if (size == 0) {
        return NULL;
    }

    TreeNode *root = NULL;
    Queue *q = create_queue();

    for (int i = 0; i < size; i++) {
        if (level_order[i] == INT_MIN) {
            continue; // 跳过空节点
        }

        TreeNode *new_node = (TreeNode*)malloc(sizeof(TreeNode));
        new_node->val = level_order[i];
        new_node->left = new_node->right = NULL;

        if (root == NULL) {
            root = new_node;
            enqueue(q, root);
        } else {
            TreeNode *parent = dequeue(q);
            if (parent->left == NULL) {
                parent->left = new_node;
                enqueue(q, parent->left);
            } else {
                parent->right = new_node;
                enqueue(q, parent->right);
            }
        }
    }

    free_queue(q);
    return root;
}

void preorder_traversal(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    printf("%d ", root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    if (root == NULL) {
        return;
    }
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
