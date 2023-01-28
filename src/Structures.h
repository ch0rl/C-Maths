/* A C implementation of tree, queue, and stack structures */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Tokenise.h"
#include "Errors.h"

#ifndef STRUCTURES_H
#define STRUCTURES_H

#define C_BUFF_N 2
#define STACK_INC 5

typedef struct Tree_Node {
    T_Token token;
    unsigned int c_num;
    unsigned int c_buff_len;
    struct Tree_Node *children;
} T_Tree_Node;

typedef struct Stack {
    T_Token *_stack;
    unsigned int len;
    unsigned int buff_len;
} T_Stack;

typedef struct Node_Stack {
    T_Tree_Node *_stack;
    unsigned int len;
    unsigned int buff_len;
} T_Node_Stack;

typedef struct Queue {  // Circular queue
    T_Token *_queue;
    unsigned int top;  // Next free space
    unsigned int bottom;  // Dequeue from
    unsigned int size;
} T_Queue;

typedef struct Symbol {
    char *repr;
    
    bool has_val;
    long double val;

    bool has_expr;
    T_Tree_Node expr;
} T_Symbol;

/* Creates a new tree node */
T_Tree_Node new_tree_node(T_Token tok) {
    T_Tree_Node n;
    n.token = tok;
    n.children = NULL;
    n.c_num = 0;
    n.c_buff_len = 0;

    return n;
}

/* Adds a child node to head */
void add_tree_child(T_Tree_Node *head, T_Tree_Node n) {
    if (head->c_buff_len == 0) {
        head->c_buff_len = C_BUFF_N;
        head->children = (T_Tree_Node *)malloc(sizeof(T_Tree_Node) * head->c_buff_len);
        
        if (head->children == NULL) {
            err_red_location("[Structures.h; add_tree_child]", "Failed to allocate children.");
        }
    } else if (head->c_num >= head->c_buff_len) {
        head->c_buff_len += C_BUFF_N;
        head->children = (T_Tree_Node *)realloc(head->children, sizeof(T_Tree_Node) * head->c_buff_len);

        if (head->children == NULL) {
            err_red_location("[Structures.h; add_tree_child]", "Failed to allocate children.");
        }
    }

    head->children[head->c_num++] = n;
}

/* Adds a new child node to head */
void add_tree_child_data(T_Tree_Node *head, T_Token child_tok) {
    T_Tree_Node n = new_tree_node(child_tok);
    add_tree_child(head, n);
}

/* Frees a tree */
void free_tree(T_Tree_Node head) {
    for (unsigned int i = 0; i < head.c_buff_len; i++) {
        free_tree(head.children[i]);
    }
    free(head.children);
}

/* Creates a new stack */
T_Stack *new_stack(unsigned int min_size) {
    T_Stack *stack = (T_Stack *)malloc(sizeof(T_Stack));
    if (stack == NULL) {
        err_red_location("[Structures.h; new_stack]", "Failed to allocate stack.");
    }

    stack->len = 0;
    stack->buff_len = min_size;
    stack->_stack = (T_Token *)malloc(sizeof(T_Token) * min_size);

    if (stack->_stack == NULL) {
        err_red_location("[Structures.h; new_stack]", "Failed to allocate _stack.");
    }

    return stack;
}

/* Push an item to a stack */
bool stack_push(T_Stack *stack, T_Token item) {
    if (stack->len == stack->buff_len) {
        stack->buff_len += STACK_INC;
        stack->_stack = (T_Token *)realloc(stack->_stack, stack->buff_len);

        if (stack->_stack == NULL) {
            err_red_location("[Structures.h; stack_push]", "Failed to allocate _stack.");
        }
    }

    stack->_stack[stack->len++] = item;
    return true;
}

/* Pops an item from `stack` */
T_Token stack_pop(T_Stack *stack) {
    if (stack->len == 0) {
        err_red_location("[Structures.h; stack_pop]", "Can't pop from an empty stack.");
    }

    stack->len--;
    return stack->_stack[stack->len];
}

/* Returns the item at the top of `stack` */
T_Token stack_peek(T_Stack *stack) {
    if (stack->len == 0) {
        err_red_location("[Structures.h; stack_peek]", "Can't peek from an empty stack.");
    }

    return stack->_stack[stack->len - 1];
}

/* Frees a stack */
void stack_free(T_Stack *stack) {
    free(stack->_stack);
    free(stack);
}

/* Creates a new node stack */
T_Node_Stack *new_node_stack(unsigned int min_size) {
    T_Node_Stack *stack = (T_Node_Stack *)malloc(sizeof(T_Node_Stack));
    if (stack == NULL) {
        err_red_location("[Structures.h; new_node_stack]", "Failed to allocate stack.");
    }

    stack->len = 0;
    stack->buff_len = min_size;
    stack->_stack = (T_Tree_Node *)malloc(sizeof(T_Tree_Node) * min_size);

    if (stack->_stack == NULL) {
        err_red_location("[Structures.h; new_node_stack]", "Failed to allocate _stack.");
    }

    return stack;
}

/* Push an item to a stack */
void stack_push_node(T_Node_Stack *stack, T_Tree_Node item) {
    if (stack->len == stack->buff_len) {
        stack->buff_len += STACK_INC;
        stack->_stack = (T_Tree_Node *)realloc(stack->_stack, stack->buff_len);

        if (stack->_stack == NULL) {
            err_red_location("[Structures.h; new_node_stack]", "Failed to allocate _stack.");
        }
    }

    stack->_stack[stack->len++] = item;
}

/* Pops an item from `stack` */
T_Tree_Node stack_pop_node(T_Node_Stack *stack) {
    if (stack->len == 0) {
        err_red_location("[Structures.h; stack_pop]", "Cannot pop from an empty stack.");
    }

    stack->len--;
    return stack->_stack[stack->len];
}

/* Returns the item at the top of `stack` */
T_Tree_Node stack_peek_node(T_Node_Stack *stack) {
    if (stack->len == 0) {
        err_red_location("[Structures.h; stack_peek]", "Can't peek from an empty stack.");
    }

    return stack->_stack[stack->len - 1];
}

/* Frees a stack */
void stack_free_node(T_Node_Stack *stack) {
    free(stack->_stack);
    free(stack);
}

/* Creates a new queue */
T_Queue *new_queue(unsigned int size) {
    T_Queue *queue = (T_Queue *)malloc(sizeof(T_Queue));
    if (queue == NULL) {
        err_red_location("[Structures.h; new_queue]", "Failed to allocate queue.");
    }

    queue->size = size;
    queue->_queue = (T_Token *)malloc(sizeof(T_Token) * size);
    if (queue->_queue == NULL) {
        err_red_location("[Structures.h; new_queue]", "Failed to allocate _queue.");
    }

    queue->bottom = 0;
    queue->top = 0;

    return queue;
}

/* Enqueues an item */
void enqueue(T_Queue *queue, T_Token item) {
    queue->_queue[queue->top] = item;
    queue->top = (queue->top + 1) % queue->size;

    if (queue->top == queue->bottom) {
        err_red_location("[Structures.h; enqueue]", "Queue is full.");
    }
}

/* Dequeues an item */
T_Token dequeue(T_Queue *queue) {
    if (queue->top == queue->bottom) {
        err_red_location("[Structures.h; dequeue]", "Cannot dequeue from an empty queue.");
    }

    T_Token item = queue->_queue[queue->bottom];
    queue->bottom = (queue->bottom + 1) % queue->size;
    return item;
}

/* Returns the item at the front of the queue */
T_Token queue_peek(T_Queue *queue) {
    if (queue->top == queue->bottom) {
        err_red_location("[Structures.h; queue_peek]", "Cannot peek from an empty queue.");
    }

    return queue->_queue[queue->bottom];
}

/* Frees a queue */
void queue_free(T_Queue *queue) {
    free(queue->_queue);
    free(queue);
}


#endif  // STRUCTURES_H
