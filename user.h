// Copyright 2022 Slavu Ioan
#ifndef USER_H_
#define USER_H_

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "hashtable.h"
#include "utils.h"

typedef struct user user;

struct user{
    char user_name[MAX_INPUT_SIZE];
    int points;
    int is_banned;
    int is_borrowed;
    int borrowed_days;
    char borrowed_book_name[MAX_INPUT_SIZE];
};

void
add_user(hashtable_t *ht_user, char *user_name);

void
user_borrow_book(hashtable_t *ht_user, hashtable_t *ht_library,
                    char *user_name, char *book_name, int days);

void
user_return_book(hashtable_t *ht_user, hashtable_t *ht_library,
                    char *user_name, char *book_name, int days, double rating);

void
user_lost_book(hashtable_t *ht_user, hashtable_t *ht_library,
                    char *user_name, char *book_name);

void
top_users(hashtable_t *ht_user);

int
compare_users_value(void* user1, void* user2);

void
free_user(hashtable_t *ht_user);

#endif  // USER_H_
