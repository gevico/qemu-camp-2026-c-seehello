#include "singly_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

// 全局头指针
static link head = NULL;

// 创建新节点
link make_node(unsigned char item) {
    link temp = (link)malloc(sizeof(struct node));
    temp->next = NULL;
    temp->item = item;
    return temp;
}

// 释放节点
void free_node(link p) { free(p); }

// 查找节点
link search(unsigned char key) {
    link temp = head;
    while(temp != NULL)
    {
        if(temp->item == key)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// 在链表头部插入节点
void insert(link p) {
    // TODO: 在这里添加你的代码
    if(p == NULL)
        return;
    
    if(head == NULL)
    {
        head = p;
        return;
    }
    p->next = head->next;
    head->next = p;
}

// 删除指定节点
void delete(link p) {
    if(p == NULL || head == NULL)
        return;

    link temp = head;
    while(temp != NULL)
    {
        if(temp->next == p)
        {
            temp->next = p->next;
            free_node(p);
        }
    }
}

// 遍历链表
void traverse(void (*visit)(link)) {
    if (visit == NULL || head == NULL) {
        return;
    }

    link cur = head;        // 跳过头结点
    while (cur != NULL) {
        visit(cur);
        cur = cur->next;
    }
}

// 销毁整个链表
void destroy(void) {
    if (head == NULL) {
        return;
    }

    link cur = head;
    while (cur != NULL) {
        link next = cur->next;
        free_node(cur);
        cur = next;
    }

    head = NULL;

}

// 在链表头部推入节点
void push(link p) { 
    if(p == NULL)
        return;
    
    p->next = head;
    head = p;
 }

// 从链表头部弹出节点
link pop(void) {
    if(head == NULL || head->next == NULL)
    {
        return NULL;
    }

    link temp = head;
    head = temp->next;
    return temp;
}

// 释放链表内存
void free_list(link list_head) {
    if (list_head == NULL) {
        return;
    }

    link cur = list_head;
    while (cur != NULL) {
        link next = cur->next;
        free_node(cur);
        cur = next;
    }

    list_head = NULL;
}
