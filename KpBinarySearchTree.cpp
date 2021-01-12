#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct node
{
    int value;
    struct node* left;
    struct node* right;
} node;

node* insert(int value, node *tnode)
{
    node * curr_node = tnode;
    if (curr_node ==  NULL){
        curr_node = (node *) malloc(sizeof (node));
        curr_node -> value = value;
        curr_node -> left = curr_node -> right = NULL;
        return curr_node;
    }
    if (value >= curr_node -> value){
        curr_node -> right = insert(value, curr_node -> right);
    }
    else if (value < curr_node -> value){
        curr_node -> left = insert(value, curr_node -> left);
    }
    return curr_node;
}

void display(node* tnode)
{
    if(tnode==NULL)
    {
        return;
    }
    display(tnode->left);
    printf("%d ", tnode->value);
    display(tnode->right);
}

int main() {
    int i, num, value;
    node *root = NULL;
    scanf("%d", &num);
    for (i = 0; i < num; i++){
        scanf("%d",&value);
        root = insert(value, root);
    }
    display(root);
    return 0;
}