#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

static bool enqueue(Queue *q, People p) {
    if (q->count >= MAX_PEOPLE) {
        return false;
    }

    q->data[q->tail] = p;
    q->tail = (q->tail + 1) % MAX_PEOPLE;
    q->count++;
    return true;
}

static bool dequeue(Queue *q, People *out) {
    if (q->count <= 0) {
        return false;
    }

    *out = q->data[q->head];
    q->head = (q->head + 1) % MAX_PEOPLE;
    q->count--;
    return true;
}

int main() {
    Queue q = {0};
    int total_people = 50;
    int report_interval = 5;

    for (int i = 1; i <= total_people; i++) {
        People p = { .id = i };
        enqueue(&q, p);
    }

    while (q.count > 1) {
        for (int i = 1; i < report_interval; i++) {
            People p;
            dequeue(&q, &p);
            enqueue(&q, p);
        }

        People eliminated;
        dequeue(&q, &eliminated);
        printf("淘汰: %d\n", eliminated.id);
    }

    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}