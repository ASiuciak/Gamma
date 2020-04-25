#include "stack.h"

Pair make_Pair(uint32_t x, uint32_t y) {
    Pair p;
    p.x = x;
    p.y = y;
    return p;
}

Stack new_Stack() {
    Stack n = NULL;
    n = safe_malloc(n, (sizeof(struct stack)));
    n->next = NULL;
    return n;
}

void push(Stack s, Pair p) {
    if (s == NULL) { exit(2); }
    Stack n = NULL;
    n = safe_malloc(n, (sizeof(struct stack)));
    n->element = p;
    n->next = s->next;
    s->next = n;
}

bool empty(Stack s) {
    if (s == NULL || s->next == NULL) { return true; }
    return false;
}

Pair top(Stack s) {
    if (empty(s)) { exit(2); }
    return s->next->element;
}

void pop(Stack s) {
    if (!empty(s)) {
        Stack first = s->next;
        Stack second = first->next;
        s->next = second;
        free(first);
    }
}

void del(Stack s) {
    if (s != NULL) {
        del(s->next);
        free(s);
    }
}



