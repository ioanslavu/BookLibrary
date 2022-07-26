// Copyright 2022 Slavu Ioan
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "messages.h"
#include "hashtable.h"
#include "book.h"
#include "library.h"

// add new book to library
void
add_book(hashtable_t *ht_library, char *book_name, int number_of_def)
{
    if (!ht_library || !book_name)
        return;

    // check if book exists
    if (ht_has_key(ht_library, book_name))
        delete_book(ht_library, book_name);

    // create book struct
    hashtable_t *new_book_ht = NULL;

    // create book hashtabke
    new_book_ht = ht_create(MAX_BUCKET_SIZE, hash_function_string,
                    compare_function_strings);

    if (!new_book_ht)
        return;

    // read pair of key-value from stdin and add to book hashtable
    char args[MAX_INPUT_SIZE], key[MAX_INPUT_SIZE], value[MAX_INPUT_SIZE];
    while (number_of_def > 0)
    {
        fgets(args, MAX_INPUT_SIZE, stdin);
        sscanf(args, "%s %s", key, value);
        // add to book hashtable
        add_def(new_book_ht, key, value);

        number_of_def--;
    }

    book new_book;
    new_book.rating = 0;
    new_book.purchases = 0;
    new_book.borrowed = 0;
    strcpy(new_book.name, book_name);
    new_book.book_ht = new_book_ht;

    // add new book to library
    ht_put(ht_library, book_name, strlen(book_name) + 1,
            &new_book, sizeof(new_book));
}

// get_book
void
get_book(hashtable_t *ht_library, char *book_name)
{
    if (!ht_library || !book_name)
        return;

    // find in library hashtable
    if (!ht_has_key(ht_library, book_name)) {
        book_not_found_message();
        return;
    }

    book *book = ht_get(ht_library, book_name);
    if (!book)
        return;

    double book_rating = get_book_ranking(book);
    show_book(book->name, book_rating, book->purchases);
}

void
delete_book(hashtable_t *ht_library, char* book_name)
{
    if (!ht_library || !book_name)
        return;

    // find in library hashtable
    if (!ht_has_key(ht_library, book_name)) {
        book_not_found_message();
        return;
    }

    book *book = ht_get(ht_library, book_name);
    if (!book)
        return;

    // delete book hashtable
    free_book(book->book_ht);
    book->borrowed = 0;
    book->purchases = 0;
    book->rating = 0;

    // delete from library hashtable
    ht_remove_entry(ht_library, book_name);
}

void
add_def_to_book(hashtable_t *ht_library, char *book_name,
                    char *def_key, char *def_value)
{
    if (!ht_library || !book_name || !def_key || !def_value)
        return;

    // find in library hashtable
    if (!ht_has_key(ht_library, book_name)) {
        book_not_found_message();
        return;
    }

    book *book = ht_get(ht_library, book_name);
    if (!book)
        return;

    // add new def to book hashtable
    add_def(book->book_ht, def_key, def_value);
}


void
get_def_from_book(hashtable_t *ht_library, char *book_name, char *def_key)
{
    if (!ht_library || !book_name || !def_key)
        return;

    // find in library hashtable
    if (!ht_has_key(ht_library, book_name)) {
        book_not_found_message();
        return;
    }
    book *book = ht_get(ht_library, book_name);
    if (!book)
        return;

    // find in book hashtable
    get_def(book->book_ht, def_key);
}

void
delete_def_from_book(hashtable_t *ht_library, char *book_name, char *def_key)
{
    if (!ht_library || !book_name || !def_key)
        return;

    // find in library hashtable
    if (!ht_has_key(ht_library, book_name)) {
        book_not_found_message();
        return;
    }
    book *book = ht_get(ht_library, book_name);
    if (!book)
        return;

    // delete def from book hashtable
    delete_def(book->book_ht, def_key);
}

int
borrow_book(hashtable_t *ht_library, char *book_name)
{
    if (!ht_library || !book_name)
        return 0;

    // find in library hashtable
    if (!ht_has_key(ht_library, book_name)) {
        book_not_found_message();
        return 0;
    }
    book *book = ht_get(ht_library, book_name);
    if (!book)
        return 0;

    // verify if book is not borrowed
    if (book->borrowed) {
        book_is_borrowed_message();
        return 0;
    }

    // borrow book
    book->borrowed++;

    return 1;
}

void
return_book(hashtable_t *ht_library, char *book_name)
{
    if (!ht_library || !book_name)
        return;

    // find in library hashtable
    if (!ht_has_key(ht_library, book_name)) {
        book_not_found_message();
        return;
    }

    book *book = ht_get(ht_library, book_name);
    if (!book)
        return;

    // verify if book is borrowed
    if (!book->borrowed) {
        return;
    }

    // return book
    book->borrowed--;

    // add a new purchase
    book->purchases++;
}

void
add_rating_to_book(hashtable_t *ht_library, char *book_name, double rating)
{
    if (!ht_library || !book_name)
        return;

    // find in library hashtable
    if (!ht_has_key(ht_library, book_name)) {
        book_not_found_message();
        return;
    }

    book *book = ht_get(ht_library, book_name);
    if (!book)
        return;

    // add rating to book
    book->rating += rating;
}

void
top_books(hashtable_t *ht_library)
{
    if (!ht_library)
        return;

    // create a linked list where will be stored the books
    linked_list_t *books_list = NULL;
    books_list = ll_create(sizeof(book));

    // transform hashtable into a linked list
    ht_to_ll(ht_library, books_list);

    // sort the linked list
    ll_sort(books_list, compare_books_value);

    ll_node_t *curr_book = books_list->head;

    printf("Books ranking:\n");

    int i = 1;
    while (curr_book != NULL) {
        book *book = curr_book->data;
        double book_rating = get_book_ranking(book);
        printf("%d. ", i);
        show_book(book->name, book_rating, book->purchases);
        curr_book = curr_book->next;
        i++;
    }

    ll_free(&books_list);
    free(books_list);
}

// compare 2 book value
int
compare_books_value(void* book1, void* book2)
{
    if (!book1 || !book2)
        return -1;

    book *book1_data = (book *)((ll_node_t*)book1)->data;
    book *book2_data = (book *)((ll_node_t*)book2)->data;

    double book1_rating = get_book_ranking(book1_data);
    double book2_rating = get_book_ranking(book2_data);

    if (book1_rating == book2_rating) {
        if (book1_data->purchases == book2_data->purchases)
            return strcmp(book1_data->name, book2_data->name) < 0 ? 0 : 1;
        return book1_data->purchases > book2_data->purchases ? 0 : 1;
    }

    return book1_rating > book2_rating ? 0 : 1;
}


double
get_book_ranking(book *book)
{
    return book->purchases > 0 ? book->rating / book->purchases : 0;
}

void
free_library(hashtable_t *ht_library)
{
    if (!ht_library)
        return;

    for (unsigned int i = 0; i < ht_library->hmax; ++i) {
        ll_node_t* node = ht_library->buckets[i]->head;
        while (node != NULL) {
            struct info* data_info = (struct info *)node->data;
            book *book = data_info->value;
            free_book(book->book_ht);
            node = node->next;
        }
    }
    ht_free(ht_library);
}
