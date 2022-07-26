// Copyright 2022 Slavu Ioan
#ifndef BOOK_H_
#define BOOK_H_

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "hashtable.h"
#include "utils.h"

typedef struct book book;
struct book{
    hashtable_t *book_ht;
    char name[MAX_INPUT_SIZE];
    double rating;
    int purchases;
    int borrowed;
};

void
add_def(hashtable_t *ht_book, char* def_key, char* def_val);

void
get_def(hashtable_t *ht_book, char* def_key);

void
delete_def(hashtable_t *ht_book, char* def_key);

void
free_book(hashtable_t *ht_book);

#endif  // BOOK_H_
