#include "9cc.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        error("invalid argument");
    }

    // tokenize and parse.
    user_input = argv[1];
    token = tokenize();
    Node *node = expr();

    codegen(node);
}
