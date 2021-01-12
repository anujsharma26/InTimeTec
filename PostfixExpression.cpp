#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#define MAX 100

struct Stack {
    int items[MAX];
    int top;
} stack;

void stackInit() {
    stack.top = -1;
}

void stackPush(int item) {
    if (stack.top == (MAX - 1)) {
        printf("Stack overflow.");
    }
    stack.top++;
    stack.items[stack.top] = item;
}

int stackPop() {
    if (stack.top == -1) {
        return -2;
    }
    int item = stack.items[stack.top];
    stack.top--;
    return item;
}

int stackPeak() {
    if (stack.top == -1) {
        return -2;
    }
    int item = stack.items[stack.top];
    return item;
}

bool isInt(char input) {
    return isdigit(input);
}

bool isOperator(char input) {
    return (input == '+' || input == '-' || input == '*' || input == '/');
}

int getRes(char * str) {
    int i = 0;
    stackInit();
    while(i < strlen(str)) {
        char input_ch = str[i];

        if (isdigit(input_ch)) {
            int out_num = (input_ch - '0');
            char next_input_ch = str[++i];

            if (isdigit(next_input_ch)) {
                while (isdigit(next_input_ch)) {
                    out_num = (out_num * 10 + ((next_input_ch) - '0'));
                    next_input_ch = str[++i];
                }
            }
            stackPush(out_num);
        }
        else if (isOperator(input_ch)) {
            int firstOperand = stackPop();
            int nextOperand = stackPop();

            if (firstOperand == -2 || nextOperand == -2)
                return 0;

            switch(input_ch)
            {
                case '+':
                    stackPush(nextOperand + firstOperand);
                    break;
                case '-':
                    stackPush(nextOperand - firstOperand);
                    break;
                case '*':
                    stackPush(nextOperand * firstOperand);
                    break;
                case '/':
                    if (firstOperand != 0)
                    stackPush(nextOperand / firstOperand);
                    else return 0;
                    break;
            }
        }
        i++;
    }
    return stackPop();
}

int main() {
    char name[100];
    scanf("%[^\n]%*c", name);
    int ans = getRes(name);
    printf("%d", ans);
    return 0;
}

