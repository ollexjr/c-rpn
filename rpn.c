#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STACK_SIZE 100
float stack[STACK_SIZE];
int stackptr = 0;
float up(int index)
{
    return stack[index];
}
int size()
{
    return stackptr;
}
int top()
{
    return stack[stackptr];
}
float push(float v)
{
    stack[++stackptr] = v;
    return v;
}
float pop()
{
    return stack[stackptr--];
}

typedef float (*func_ptr)(float);
struct op
{
    char symbol;
    char nargs;
    func_ptr func;
};
float fadd(int nargs)
{
    float b = pop();
    float a = pop();
    return push(a + b);
}
float fsub(int nargs)
{
    float b = pop();
    float a = pop();
    return push(a - b);
}
float fmul(int nargs)
{
    float b = pop();
    float a = pop();
    return push(a * b);
}
float fdiv(int nargs)
{
    float b = pop();
    float a = pop();
    if (b == 0) { 
        printf("warning cannot divide by zero\n");
        return push(a);
    }
    return push(a / b);
}
float fpow (int nargs) {
    float b = pop();
    float a = pop();
    return push(pow(a, b));
}
float fshiftup(int nargs) {
    int b = pop();
    float a = pop();
    return push((int)a << b);
}
float fshiftdown(int nargs) {
    int b = pop();
    float a = pop();
    return push((int)a >> b);
}
#define OPS_SIZE 12
struct op ops[OPS_SIZE] = {
    {'+', 2, fadd},
    {'a', 2, fadd},
    {'-', 2, fsub},
    {'s', 2, fsub},
    {'*', 2, fmul},
    {'m', 2, fmul},
    {'/', 2, fdiv},
    {'d', 2, fdiv},
    {'^', 2, fpow},
    {'p', 2, fpow},
    {'u', 2, fshiftup},
    {'d', 2, fshiftdown}
};

int try_op(char *in, int len)
{
    for (int i = 0; i < OPS_SIZE; i++)
    {
        struct op *o = &ops[i];
        if (o->symbol == *in)
        {
            if (stackptr < 0 || stackptr < o->nargs)
            {
                // can't op it
                continue;
            }
            // can op it
            printf("op: %c, args: %d, stack: %d ", o->symbol, o->nargs, size());
            o->func(o->nargs);
            printf("ret: %d\n", top());
            return 1;
        }
    }
    return -1;
}
int try_value(char *in, int len)
{
    if (len < 1 || len > 1000)
    {
        return -1;
    }
    char *endptr;
    int v = strtol(in, &endptr, 10);
    if (in == endptr)
    {
        //no digits found!
        return -1;
    }
    if (*endptr != 0 && (*endptr < '0' || *endptr > '9'))
    {
        //bad input
        return -1;
    }
    push(v);
    return v;
}
int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        //printf("%s\n", argv[i]);
        char *p = argv[i];
        int len = strlen(p);
        if (try_op(p, len) > 0)
        {
            continue;
        }
        else
        {
            try_value(p, len);
            continue;
        }
    }
    if (size() > 1)
    {
        //values left over!
        printf("values left on stack\n");
    }
    printf("%g\n", up(1));
}