#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node_t
{
    void* data;
    struct node_t *next;
} node;

typedef struct linked_list_t {
    node *head,* tail;
} linked_list;

typedef struct slot_type
{
    int in_use;
    int offset;
    int size;
    int size_in_use;
} slot_t;

typedef struct block_type {
    linked_list *slots;
    int size;
    int size_in_use;
    void* buffer;
} block_t;

block_t block;

void list_init(linked_list **list);
int add(linked_list *list, void* item);
void init_slot(slot_t **slot);

slot_t * InitSlot(slot_t * new_slot, int size) {
    init_slot(&new_slot);
    new_slot -> in_use = 1;
    new_slot -> size_in_use = size;
    new_slot -> size = size;
    return new_slot;
}

void * kpalloc(int size) {

    int curr_buff_in_use = 0, curr_x= 0;
    struct block_type * temp_block = &block;
    linked_list * slots_list = temp_block -> slots;
    node * curr_node = slots_list -> head;

    if (slots_list -> head == NULL){
        slot_t * new_slot ;
        new_slot = InitSlot(new_slot, size);
        new_slot -> offset = 0;
        temp_block -> size_in_use = size;
        curr_buff_in_use += size;
        add(slots_list, (void *) new_slot);
        return temp_block -> buffer;

    }
    while (curr_node != NULL){
        slot_t * slot = (slot_t*)curr_node -> data;
        if (((slot->in_use) == 0) && ((slot -> size) >= size)){
            slot -> in_use = 1;
            slot -> size_in_use = size;
            return temp_block -> buffer + curr_buff_in_use;

        }
        else{
            curr_buff_in_use += slot->size;
            curr_node = curr_node -> next;
            continue;
        }
    }
    if (curr_buff_in_use + size > temp_block-> size) {
        return NULL;
    }
    slot_t * new_slot;
    new_slot = InitSlot(new_slot, size);
    new_slot -> offset = curr_buff_in_use;
    curr_x = temp_block->size_in_use;
    temp_block->size_in_use += size;
    add(slots_list, (void *) new_slot);
    return temp_block -> buffer + curr_x;
}

void  kpfree(void * buffer){

    struct block_type * temp_block = &block;
    linked_list * slots_list = temp_block -> slots;
    node * curr_node = slots_list -> head;
    while (curr_node != NULL){
        slot_t * slot = (slot_t*)curr_node -> data;
        if ((slot -> offset) == (buffer - (temp_block->buffer))){
            slot-> in_use = 0;
            return;
        }
        else{
            curr_node = curr_node -> next;
        }
    }

}

void init_block(block_t * block,int block_size)
{       printf("Reserved buffer size = %d\n",block_size);
    block->buffer=(void*)malloc(block_size);
    block->size = block_size;
    block->size_in_use= 0;
    list_init(&(block->slots));
}
void init_slot(slot_t ** slot)
{
    *slot = (slot_t*)malloc(sizeof(slot_t));
    (*slot)->in_use = 0;
    (*slot)->offset = -1;
    (*slot)->size=-1;
    (*slot)->size_in_use = -1;
}

typedef struct mem_request_type
{
    int type;
    int index;
    int size;
} mem_request_t;

mem_request_t  parse_mem_request(char *line)
{
    mem_request_t mem_request;
    mem_request.index = -1;
    mem_request.size= -1;
    mem_request.type= -1;
    char * request = strtok(line," ");
    char * index = strtok(NULL," ");
    char * size = strtok(NULL, " ");
    if(!strcmp(request,"kpalloc"))
    {
        mem_request.type= 0;
        mem_request.index = atoi(index);
        mem_request.size = atoi(size);
    }else
    {
        mem_request.type= 1;
        mem_request.index = atoi(index);
    }
    return mem_request;

}
void writeBuffer(char * buffer,int size,char c)
{
    for(int i =0;i<size;i++)
    {
        buffer[i] = c;
    }
}
void dump_request(mem_request_t mem_request)
{
    printf("Mem Request\n");
    printf("Type: %d, Index: %d, Size: %d, \n",
           mem_request.type,mem_request.index,mem_request.size);
}

void dump_block(block_t * block)
{
    printf("Block size in use:%d\n",block->size_in_use);
    printf("Dump of Block Memory:");
    for(int i=0;i<block->size_in_use;i++)
    {
        printf("%c",((char*)block->buffer)[i]);
    }
    node *current = block->slots->head;
    printf("\nSlot report\n");
    int slot_index = 0;
    while(current!=NULL)
    {
        slot_t *slot = (slot_t*)current->data;
        if(slot->size == -1)

        {
            break;
        }
        printf("Slot[%d]: in_use=%d,offset=%d,size=%d,size_in_use=%d\n",
               slot_index,slot->in_use,slot->offset,slot->size,slot->size_in_use);
        current = current->next;
        slot_index++;
    }

    printf("\n");
}



void list_init(linked_list **list)
{
    *list = malloc(sizeof(linked_list));
    (*list)->head = NULL;
    (*list)->tail = NULL;
}

int add(linked_list *list, void* item)
{
    node *new_node= (node*)malloc(sizeof(node));
    new_node->data=item;
    new_node->next = NULL;

    if(list->head == NULL)
    {
        list->head=new_node;
        list->tail=new_node;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    return 0;
}
int add_last(linked_list *list, void * item)
{
    return add(list,item);
}
int add_first(linked_list *list, void* item)
{
    node *new_node= (node*)malloc(sizeof(node));
    new_node->data=item;
    new_node->next = NULL;

    if(list->head == NULL)
    {
        list->head=new_node;
        list->tail=new_node;
    }
    else
    {
        new_node->next = list->head;
        list->head = new_node;
    }

    return 0;
}

node * traverse_to(linked_list *list, int index)
{
    int current_index = 0;
    node *current = list->head;

    while(current!=NULL)
    {
        if(index == current_index)
        {
            return current;
        }
        current = current->next;
        current_index++;

    }
    return NULL;
}
int add_at_index(linked_list *list, int index,void* item)
{
    node *new_node= (node*)malloc(sizeof(node));
    new_node->data=item;
    new_node->next = NULL;

    if(list->head == NULL)
    {
        list->head=new_node;
        list->tail=new_node;
    }
    else if(index == 0)
    {
        new_node->next = list->head;
        list->head = new_node;
    }
    else
    {
        node *previous_node = traverse_to(list,index-1);
        if(previous_node == NULL)
        {
            printf("Index not found\n");
            return -1;
        }
        new_node->next=previous_node->next;
        previous_node->next=new_node;
    }

    return 0;
}

node* traverse_to_2nd_last(linked_list *list)
{
    int current_index = 0;
    node *current = list->head;

    while(current!=NULL)
    {
        if(current->next->next == NULL)
        {
            return current;
        }
        current = current->next;

    }
    return NULL;

}
int remove_first(linked_list *list)
{
    node* node_to_be_removed = NULL;
    if(list->head == NULL) return 0;

    node_to_be_removed = list->head;

    list->head = list->head->next;

    free(node_to_be_removed);
    return 0;
}
int list_remove(linked_list *list)
{
    if(list->head == NULL) return 0;

    if(list->head->next == NULL)
    {
        free(list->head->next);
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        node* current = traverse_to_2nd_last(list);
        current->next = NULL;
        free(list->tail);
        list->tail = current;
    }
    return 0;
}

int remove_last(linked_list *list)
{
    return list_remove(list);
}

int list_remove_at_index(linked_list *list, int index)
{
    node* previous_node=NULL;
    node* node_to_be_deleted= NULL;
    if(list->head == NULL) return 0;
    else if(index == 0)
    {
        previous_node = list->head;
        list->head = list->head->next;
        free(previous_node);
        return 0;
    }

    previous_node =  traverse_to(list,index-1);
    node_to_be_deleted = previous_node->next;

    previous_node->next = previous_node->next->next;

    free(node_to_be_deleted);

    return 0;
}
node * search(linked_list * list, void * item)
{
    int current_index = 0;
    node *current = list->head;

    while(current!=NULL)
    {
        if(current->data == item)
        {
            return current;
        }
        current = current->next;
        current_index++;

    }
    return NULL;

}


void list_display(linked_list *list)
{
    node *current=list->head;
    printf("List : ");
    while(current!=NULL)
    {
        printf("%d,",current->data);
        current = current->next;
    }
    printf("\n");
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}

long parse_long(char* str) {
    char* endptr;
    long value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }
    return value;
}
int main(int argc, char * argv[])
{

    void * ptr[100];
    int block_size=0;
    linked_list *list;
    list_init(&list);

    block_size= parse_int(ltrim(rtrim(readline())));
    init_block(&block,block_size);

    int l_count = parse_int(ltrim(rtrim(readline())));
    for (int i = 0; i < l_count; i++) {
        mem_request_t mem_request = parse_mem_request(ltrim(rtrim(readline())));
        //dump_request(mem_request);
        if(mem_request.type == 0 )
        {
            void *buffer = kpalloc(mem_request.size);
            if(buffer!=NULL)
            {
                // printf("Allocated a Slot of size=%d at index=%d\n",mem_request.size,mem_request.index);
                char data = 'a' + mem_request.index;
                writeBuffer(buffer,mem_request.size,data);
                // printf("Writing to buffer sequence of '%c'\n",data);
                ptr[mem_request.index]  = buffer;
            }else
            {
                printf("Allocation failed\n");
            }

        }else
        {
            //printf("Freeing slot at index at %d\n",mem_request.index);
            kpfree(ptr[mem_request.index]);
        }

    }
    dump_block(&block);
    printf("completed\n");

    return 0;
}
