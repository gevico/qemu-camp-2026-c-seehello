#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    if (n <= 0) return NULL;

    Node* head = (Node*)malloc(sizeof(Node));
    head->id = 1;
    Node* prev = head;

    for (int i = 2; i <= n; ++i) {
        Node* node = (Node*)malloc(sizeof(Node));
        node->id = i;
        prev->next = node;
        prev = node;
    }

    prev->next = head;  // Make it circular
    return head;
}

void free_list(Node* head) {
    if (head == NULL) return;

    Node* current = head->next;
    while (current != head) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(head);
}
