#include "9cc.h"

Type *int_type()
{
    Type *ty = calloc(1, sizeof(Type));
    ty->kind = TY_INT;
    return ty;
}

Type *pointer_to(Type *base)
{
    Type *ty = calloc(1, sizeof(Type));
    ty->kind = TY_PTR;
    ty->base = base;
    return ty;
}

Type *array_of(Type *base, int size)
{
    Type *ty = calloc(1, sizeof(Type));
    ty->kind = TY_ARRAY;
    ty->base = base;
    ty->array_size = size;
    return ty;
}

int size_of(Type *ty)
{
    if (ty->kind == TY_INT || ty->kind == TY_PTR)
        return 8;
    assert(ty->kind == TY_ARRAY);
    return size_of(ty->base) * ty->array_size;
}

void visit(Node *node)
{
    if (!node)
        return;

    visit(node->lhs);
    visit(node->rhs);
    visit(node->cond);
    visit(node->then);
    visit(node->els);
    visit(node->init);
    visit(node->inc);

    for (Node *n = node->body; n; n = n->next)
        visit(n);
    for (Node *n = node->args; n; n = n->next)
        visit(n);

    switch (node->kind)
    {
    case ND_MUL:
    case ND_DIV:
    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
    case ND_FUNCALL:
    case ND_NUM:
        node->ty = int_type();
        return;
    case ND_VAR:
        node->ty = node->var->ty;
        return;
    case ND_ADD:
        if (node->rhs->ty->base)
        {
            Node *tmp = node->lhs;
            node->lhs = node->rhs;
            node->rhs = tmp;
        }
        if (node->rhs->ty->base)
            error_tok(node->tok, "invalid pointer arithmetic operands");
        node->ty = node->lhs->ty;
        return;
    case ND_SUB:
        if (node->rhs->ty->base)
            error_tok(node->tok, "invalid pointer arithmetic operands");
        node->ty = node->lhs->ty;
        return;
    case ND_ASSIGN:
        node->ty = node->lhs->ty;
        return;
    case ND_ADDR:
        if (node->lhs->ty->kind == TY_ARRAY)
            node->ty = pointer_to(node->lhs->ty->base);
        else
            node->ty = pointer_to(node->lhs->ty);
        return;
    case ND_DEREF:
        if (!node->lhs->ty->base)
            error_tok(node->tok, "invalid pointer dereference");
        node->ty = node->lhs->ty->base;
        return;
    case ND_SIZEOF:
        node->kind = ND_NUM;
        node->ty = int_type();
        node->val = size_of(node->lhs->ty);
        node->lhs = NULL;
        return;
    }
}

void add_type(Function *prog)
{
    for (Function *fn = prog; fn; fn = fn->next)
        for (Node *node = fn->node; node; node = node->next)
            visit(node);
}
