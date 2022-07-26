// Copyright 2022 Slavu Ioan
#ifndef LIBRARY_H_
#define LIBRARY_H_

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "hashtable.h"
#include "book.h"

void
add_book(hashtable_t *ht_library, char *book_name, int number_of_def);

void
get_book(hashtable_t *ht_library, char *book_name);

void
delete_book(hashtable_t *ht_library, char* book_name);

void
add_def_to_book(hashtable_t *ht_library,
                char *book_name, char *def_key, char *def_value);

void
get_def_from_book(hashtable_t *ht_library, char *book_name, char *def_key);

void
delete_def_from_book(hashtable_t *ht_library, char *book_name, char *def_key);

int
borrow_book(hashtable_t *ht_library, char *book_name);

void
return_book(hashtable_t *ht_library, char *book_name);

void
add_rating_to_book(hashtable_t *ht_library, char *book_name, double rating);

void
top_books(hashtable_t *ht_library);

int
compare_books_value(void* book1, void* book2);

double
get_book_ranking(book *book);

void
free_library(hashtable_t *ht_library);

#endif  // LIBRARY_H_
