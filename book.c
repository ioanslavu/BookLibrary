// Copyright 2022 Slavu Ioan
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "messages.h"
#include "book.h"

void
add_def(hashtable_t *ht_book, char *def_key, char *def_val)
{
    if (!ht_book || !def_key || !def_val)
        return;

    // add def in book hashtable
    ht_put(ht_book, def_key, strlen(def_key) + 1, def_val,
            strlen(def_val) + 1);
}

void
get_def(hashtable_t *ht_book, char* def_key)
{
    if (!ht_book || !def_key)
        return;

    // find in book hashtable
    if (!ht_has_key(ht_book, def_key)) {
        definition_not_found_message();
        return;
    }

    // show def from book hashtable
    char* def_val = (char*)ht_get(ht_book, def_key);
    printf("%s\n", def_val);
}

void
delete_def(hashtable_t *ht_book, char* def_key)
{
    if (!ht_book || !def_key)
        return;

    // find in book hashtable
    if (!ht_has_key(ht_book, def_key)) {
        definition_not_found_message();
        return;
    }

    ht_remove_entry(ht_book, def_key);
}

void
free_book(hashtable_t *ht_book)
{
    if (!ht_book)
        return;

    ht_free(ht_book);
}
