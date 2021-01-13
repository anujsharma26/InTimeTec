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

void *hash[101];

typedef struct node_t
{
    char *key;
    char *value;
    struct node_t *next;
} node;

typedef struct linked_list_t
{
    node *head;
    node *tail;
} linked_list;

void list_init(linked_list **list)
{
    *list = malloc(sizeof(linked_list));
    (*list)->head = NULL;
    (*list)->tail = NULL;
}

int get_hash_code(char *key)
{
    int index = key - '0';
    index %= 100;
    return index;
}

void put(void *hashtable[], char *key, char *value)
{
    int rec_hash = get_hash_code(key);
    linked_list *hash_val;
    list_init(&hash_val);
    node *incoming_node = (struct node_t *)malloc(sizeof(node));
    incoming_node->key = key;
    incoming_node->value = value;
    incoming_node->next = NULL;

    if (hashtable[rec_hash] == NULL)
    {
        hashtable[rec_hash] = hash_val;
        hash_val->head = incoming_node;
        hash_val->tail = incoming_node;
        return;
    }
    hash_val = ((linked_list *)hashtable[rec_hash]);
    node *curr_node = hash_val->head;
    while (curr_node->next != NULL && curr_node->key != key)
    {
        curr_node = curr_node->next;
    }
    if (curr_node->key == key)
    {
        curr_node->value = value;
    }
    else
    {
        curr_node->next = incoming_node;
        hash_val->tail = incoming_node;
    }
}

char *get(void *hashtable[], char *key)
{
    int rec_hash = get_hash_code(key);
    if (hashtable[rec_hash] == NULL)
    {
        return NULL;
    }
    linked_list *hash_val;
    list_init(&hash_val);
    hash_val = ((linked_list *)hashtable[rec_hash]);
    node *curr_node = hash_val->head;
    while (curr_node->next != NULL && curr_node->key != key)
    {
        curr_node = curr_node->next;
    }
    if (curr_node == NULL)
    {
        return NULL;
    }
    return curr_node->value;
}

char *readline()
{
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char *data = malloc(alloc_length);

    while (true)
    {
        char *cursor = data + data_length;
        char *line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line)
        {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n')
        {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data)
        {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n')
    {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data)
        {
            data = '\0';
        }
    }
    else
    {
        data = realloc(data, data_length + 1);

        if (!data)
        {
            data = '\0';
        }
        else
        {
            data[data_length] = '\0';
        }
    }

    return data;
}

char *ltrim(char *str)
{
    if (!str)
    {
        return '\0';
    }

    if (!*str)
    {
        return str;
    }

    while (*str != '\0' && isspace(*str))
    {
        str++;
    }

    return str;
}

char *rtrim(char *str)
{
    if (!str)
    {
        return '\0';
    }

    if (!*str)
    {
        return str;
    }

    char *end = str + strlen(str) - 1;

    while (end >= str && isspace(*end))
    {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

int parse_int(char *str)
{
    char *endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0')
    {
        exit(EXIT_FAILURE);
    }

    return value;
}
typedef struct record_type
{
    char *phone;
    char *name;
} record_t;

record_t parse_record(char *line)
{
    record_t record;
    record.phone = strtok(line, " ");
    record.name = strtok(NULL, " ");

    return record;
}
long parse_long(char *str)
{
    char *endptr;
    long value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0')
    {
        exit(EXIT_FAILURE);
    }
    return value;
}

int main(int argc, char *argv[])
{
    record_t records[100];
    int count = 0;
    count = parse_int(ltrim(rtrim(readline())));
    for (int i = 0; i < count; i++)
    {
        records[i] = parse_record(ltrim(rtrim(readline())));
        put(hash, records[i].phone, records[i].name);
    }
    for (int i = 0; i < count; i++)
    {
        char *name = get(hash, records[i].phone);
        if (strcmp(name, records[i].name) != 0)
        {
            printf("Incorrect data retrieved for key=%s - value=%s\n", records[i].phone, name);
        }
        else
        {
            printf("key=%s-value=%s\n", records[i].phone, name);
        }
    }

    return 0;
}
