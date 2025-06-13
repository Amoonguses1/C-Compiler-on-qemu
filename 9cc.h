#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Tokenizer
//

// kind of tokens
typedef enum
{
    TK_RESERVED, // symbols
    TK_IDENT,    // identifiers
    TK_NUM,      // integer
    TK_EOF,      // End of File
} TokenKind;

typedef struct Token Token;

// Token Structure
struct Token
{
    TokenKind kind; // a Token type
    Token *next;    // the next token
    int val;        // this field is used when kind == TK_NUM
    char *str;      // the token string
    int len;        // Token length
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
char *strndup(char *p, int len);
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize();

extern char *user_input;
extern Token *token;

//
// Parser
//

typedef enum
{
    ND_ADD,            // +
    ND_SUB,            // -
    ND_MUL,            // *
    ND_DIV,            // /
    ND_NUM,            // Integer
    ND_EQ,             // "=="
    ND_NE,             // "!="
    ND_LT,             // "<"
    ND_LE,             // "<="
    ND_ASSIGN,         // =
    ND_RETURN,         // "return"
    ND_EXPR_STATEMENT, // expression statement
    ND_VAR,            // variable
} NodeKind;

// Local variable
typedef struct Var Var;
struct Var
{
    Var *next;
    char *name; // variable name
    int offset; // offset from RBP
};

// AST node type
typedef struct Node Node;
struct Node
{
    NodeKind kind; // Node kind
    Node *next;    // next node
    Node *lhs;     // Left-hand side
    Node *rhs;     // Right-hand side
    Var *var;      // Used if kind == ND_VAR
    int val;       // Used if kind == ND_NUM
};

typedef struct
{
    Node *node;
    Var *locals;
    int stack_size;
} Program;

Program *program();

//
// codegen
//

void codegen(Program *prog);
