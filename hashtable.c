// Copyright 2022 Slavu Ioan
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "hashtable.h"
#include "LinkedList.h"

int
compare_function_ints(void *a, void *b)
{
    int int_a = *((int *)a);
    int int_b = *((int *)b);

    if (int_a == int_b) {
        return 0;
    } else if (int_a < int_b) {
        return -1;
    } else {
        return 1;
    }
}

int
compare_function_strings(void *a, void *b)
{
    char *str_a = (char *)a;
    char *str_b = (char *)b;

    return strcmp(str_a, str_b);
}

unsigned int
hash_function_int(void *a)
{
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}

unsigned int
hash_function_string(void *a)
{
    unsigned char *puchar_a = (unsigned char*) a;
    unsigned int hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c;

    return hash;
}

hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
        int (*compare_function)(void*, void*))
{
    hashtable_t *ht = malloc(sizeof(hashtable_t));
    DIE(!ht, "ht malloc");

    ht->buckets = malloc(hmax * sizeof(linked_list_t *));
    DIE(!ht->buckets, "ht->buckets malloc");
    for (int i = 0; i < (int) hmax; ++i) {
        ht->buckets[i] = ll_create(sizeof(struct info));
    }

    ht->size = 0;
    ht->hmax = hmax;
    ht->hash_function = hash_function;
    ht->compare_function = compare_function;

    return ht;
}

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
    void *value, unsigned int value_size)
{
    if (!ht || !key || !value) {
        return;
    }

        unsigned int hash = ht->hash_function(key) % ht->hmax;
        ll_node_t *node = ht->buckets[hash]->head;

    for (int i = 0; i < (int) ht->buckets[hash]->size; ++i) {
        struct info *node_info = (struct info *)node->data;

        if (!ht->compare_function(node_info->key, key)) {
            free(node_info->value);
            node_info->value = malloc(value_size);
            DIE(!node_info->value, "node_info->value malloc");
            memcpy(node_info->value, value, value_size);
            return;
        }

        node = node->next;
    }

    struct info *data_info = malloc(sizeof(struct info));
    DIE(!data_info, "data_info malloc");

    data_info->key = malloc(key_size);
    DIE(!data_info->key, "data_info key malloc");
    data_info->value = malloc(value_size);
    DIE(!data_info->value, "data_info value malloc");

    memcpy(data_info->key, key, key_size);
    memcpy(data_info->value, value, value_size);

    ll_add_nth_node(ht->buckets[hash], 0, data_info);
    ht->size++;

    free(data_info);
    ht->buckets = ht_resize(ht);
}

linked_list_t **
ht_resize(hashtable_t *ht)
{
    float loadfactor = (float)ht->size / (float)ht->hmax;
    if (loadfactor < 1) {
        return ht->buckets;
    }

    int new_hmax = ht->hmax * 2;
    linked_list_t **temp = malloc(new_hmax * sizeof(linked_list_t *));
    DIE(!ht->buckets, "ht->buckets malloc");
    for (int i = 0; i < (int) new_hmax; ++i) {
        temp[i] = ll_create(sizeof(struct info));
    }

    // iterate over all buckets and keys
    for (int i = 0; i < (int) ht->hmax; ++i) {
        ll_node_t *node = ht->buckets[i]->head;
        while (node) {
            struct info *node_info = (struct info *)node->data;
            unsigned int new_hash = ht->hash_function(node_info->key)
                                    % new_hmax;
            ll_add_nth_node(temp[new_hash], 0, node_info);
            node = node->next;
        }
    }

    for (int i = 0; i < (int) ht->hmax; ++i) {
        ll_free(&ht->buckets[i]);
    }
    free(ht->buckets);

    ht->hmax = new_hmax;
    return temp;
}

void *
ht_get(hashtable_t *ht, void *key)
{
    if (!ht || !key || ht_has_key(ht, key) != 1)
        return NULL;

    unsigned int hash = ht->hash_function(key) % ht->hmax;
    ll_node_t *node = ht->buckets[hash]->head;

    for (int i = 0; i < (int) ht->buckets[hash]->size; ++i) {
        struct info *node_info = (struct info *)node->data;

        if (!ht->compare_function(node_info->key, key)) {
            return node_info->value;
        }

        node = node->next;
    }
    return NULL;
}

int
ht_has_key(hashtable_t *ht, void *key)
{
    if (!ht || !key)
        return -1;

    unsigned int hash = ht->hash_function(key) % ht->hmax;
    ll_node_t *node = ht->buckets[hash]->head;

    for (int i = 0; i < (int) ht->buckets[hash]->size; ++i) {
        struct info *node_info = (struct info *)node->data;

        if (!ht->compare_function(node_info->key, key)) {
            return 1;
        }

        node = node->next;
    }
    return 0;
}

void
ht_to_ll(hashtable_t *ht, linked_list_t *ll)
{
    if (!ht || !ll)
        return;

    // iterate over all buckets and keys
    for (int i = 0; i < (int) ht->hmax; ++i) {
        ll_node_t *node = ht->buckets[i]->head;
        while (node) {
            struct info *node_info = (struct info *)node->data;
            // add first node, no need to make this complex in time
            ll_add_nth_node(ll, 0, node_info->value);
            node = node->next;
        }
    }
}

void
ht_remove_entry(hashtable_t *ht, void *key)
{
    if (!ht || !key || ht_has_key(ht, key) != 1)
        return;

    unsigned int hash = ht->hash_function(key) % ht->hmax;
    ll_node_t *node = ht->buckets[hash]->head;

    for (int i = 0; i < (int) ht->buckets[hash]->size; ++i) {
        struct info *node_info = (struct info *)node->data;

        if (!ht->compare_function(node_info->key, key)) {
            ll_node_t *deleted_node = ll_remove_nth_node(ht->buckets[hash], i);
            free(node_info->key);
            free(node_info->value);
            free(node->data);
            free(deleted_node);
            ht->size--;
            return;
        }
        node = node->next;
    }
}

void
ht_free(hashtable_t *ht)
{
    if (!ht) {
        return;
    }

    for (int i = 0; i < (int) ht->hmax; ++i) {
        ll_node_t *curr = ht->buckets[i]->head;
        while (curr) {
            struct info *node_info = (struct info *)curr->data;
            free(node_info->key);
            free(node_info->value);

            curr = curr->next;
        }

        ll_free(&ht->buckets[i]);
    }
    free(ht->buckets);
    free(ht);
}

unsigned int
ht_get_size(hashtable_t *ht)
{
    if (ht == NULL)
        return 0;

    return ht->size;
}

unsigned int
ht_get_hmax(hashtable_t *ht)
{
    if (ht == NULL)
        return 0;

    return ht->hmax;
}
