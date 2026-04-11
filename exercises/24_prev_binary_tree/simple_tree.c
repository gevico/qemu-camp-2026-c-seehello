#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL) {
        return NULL;
    }
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
    if (q == NULL) {
        return true;
    }
    return q->front == NULL;
}

void free_queue(Queue *q) {
    if (q == NULL) {
        return;
    }
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    if (level_order == NULL || size == 0 || level_order[0] == INT_MIN) {
        return NULL;
    }

    TreeNode *root = (TreeNode*)malloc(sizeof(TreeNode));
    if (root == NULL) {
        return NULL;
    }
    root->val = level_order[0];
    root->left = root->right = NULL;

    Queue *q = create_queue();
    if (q == NULL) {
        free(root);
        return NULL;
    }
    enqueue(q, root);

    int i = 1;
    while (i < size && !is_empty(q)) {
        TreeNode *parent = dequeue(q);

        if (i < size && level_order[i] != INT_MIN) {
            TreeNode *left = (TreeNode*)malloc(sizeof(TreeNode));
            if (left == NULL) {
                free_queue(q);
                free_tree(root);
                return NULL;
            }
            left->val = level_order[i];
            left->left = left->right = NULL;
            parent->left = left;
            enqueue(q, left);
        }
        i++;

        if (i < size && level_order[i] != INT_MIN) {
            TreeNode *right = (TreeNode*)malloc(sizeof(TreeNode));
            if (right == NULL) {
                free_queue(q);
                free_tree(root);
                return NULL;
            }
            right->val = level_order[i];
            right->left = right->right = NULL;
            parent->right = right;
            enqueue(q, right);
        }
        i++;
    }

    free_queue(q);
    return root;
}

void preorder_traversal(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    if (root->val != INT_MIN) {
        printf("%d ", root->val);
    }
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    TreeNode *stack[100]; // 简单的栈实现，假设树的深度不会超过 100
    int top = -1;

    stack[++top] = root;

    while (top >= 0) {
        TreeNode *node = stack[top--];
        if (node->val != INT_MIN) {
            printf("%d ", node->val);
        }
        

        // 先右后左入栈，这样出栈时先访问左子树
        if (node->right) {
            stack[++top] = node->right;
        }
        if (node->left) {
            stack[++top] = node->left;
        }
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
