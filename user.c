// Copyright 2022 Slavu Ioan
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "messages.h"
#include "LinkedList.h"
#include "hashtable.h"
#include "library.h"
#include "user.h"

// add new user
void
add_user(hashtable_t *ht_user, char *user_name)
{
    if (!ht_user || !user_name)
        return;

    // verify if user already exists
    if (ht_has_key(ht_user, user_name)) {
        user_already_registered_message();
        return;
    }

    // create new user
    user new_user;
    strcpy(new_user.user_name, user_name);
    new_user.points = 100;
    new_user.is_banned = 0;
    new_user.is_borrowed = 0;
    new_user.borrowed_days = 0;
    strcpy(new_user.borrowed_book_name, "");

    // add user in hashtable
    ht_put(ht_user, user_name, strlen(user_name) + 1,
            &new_user, sizeof(new_user));
}

void
user_borrow_book(hashtable_t *ht_user, hashtable_t *ht_library,
                        char *user_name, char *book_name, int days)
{
    if (!ht_user || !ht_library || !user_name || !book_name)
        return;

    // verify if user exists
    if (!ht_has_key(ht_user, user_name)) {
        user_not_found_message();
        return;
    }

    user *user = ht_get(ht_user, user_name);
    if (!user)
        return;

    // verify if user is banned
    if (user->is_banned) {
        user_banned_message();
        return;
    }

    // verify if user already borrowed a book
    if (user->is_borrowed) {
        user_already_borrowed_book_message();
        return;
    }

    // borrow book
    if (!borrow_book(ht_library, book_name))
        return;

    user->is_borrowed = 1;
    user->borrowed_days = days;
    strcpy(user->borrowed_book_name, book_name);
}

void
user_return_book(hashtable_t *ht_user, hashtable_t *ht_library,
                    char *user_name, char *book_name, int days, double rating)
{
    if (!ht_user || !ht_library || !user_name || !book_name)
        return;

    // verify if user exists
    if (!ht_has_key(ht_user, user_name)) {
        user_not_found_message();
        return;
    }

    user *user = ht_get(ht_user, user_name);
    if (!user)
        return;

    // verify if user is banned
    if (user->is_banned) {
        user_banned_message();
        return;
    }

    // verify if user borrowed a book
    if (!user->is_borrowed) {
        user_not_borrowed_book_message();
        return;
    }

    // verify if user returned the same book
    if (strcmp(user->borrowed_book_name, book_name) != 0) {
        user_not_borrowed_book_message();
        return;
    }

    // penalty for delay return
    if (days > user->borrowed_days) {
        user->points -= (days - user->borrowed_days) * 2;
    }
    // bonus for early return
    if (days < user->borrowed_days) {
        user->points += (user->borrowed_days - days) * 1;
    }
    // ban user if he has less than 0 points
    if (user->points < 0) {
        user->is_banned = 1;
        ban_user_message(user_name);
    }

    // add rating to book
    add_rating_to_book(ht_library, book_name, rating);

    // return book
    return_book(ht_library, book_name);

    user->is_borrowed = 0;
    user->borrowed_days = 0;
    strcpy(user->borrowed_book_name, "");
}

void
user_lost_book(hashtable_t *ht_user, hashtable_t *ht_library,
                    char *user_name, char *book_name)
{
    if (!ht_user || !ht_library || !user_name || !book_name)
        return;

    // verify if user exists
    if (!ht_has_key(ht_user, user_name)) {
        user_not_found_message();
        return;
    }

    user *user = ht_get(ht_user, user_name);
    if (!user)
        return;

    // verify if user is banned
    if (user->is_banned) {
        user_banned_message();
        return;
    }

    // penalty for losing a book
    user->points -= 50;

    // ban user if he has less than 0 points
    if (user->points < 0) {
        user->is_banned = 1;
        ban_user_message(user_name);
    }

    // delete book
    delete_book(ht_library, book_name);

    user->is_borrowed = 0;
    user->borrowed_days = 0;
    strcpy(user->borrowed_book_name, "");
}

void
top_users(hashtable_t *ht_user)
{
    if (!ht_user)
        return;

    // create a linked list where will be stored the users
    linked_list_t *users_list = NULL;
    users_list = ll_create(sizeof(user));

    // transform hashtable into a linked list
    ht_to_ll(ht_user, users_list);

    // sort the linked list
    ll_sort(users_list, compare_users_value);

    ll_node_t *curr_user = users_list->head;

    printf("Users ranking:\n");

    int i = 1;
    while (curr_user != NULL) {
        user *user = curr_user->data;
        if (!user->is_banned) {
            printf("%d. ", i);
            show_user(user->user_name, user->points);
            i++;
        }
        curr_user = curr_user->next;
    }

    ll_free(&users_list);
    free(users_list);
}

// compare 2 user value
int
compare_users_value(void* user1, void* user2)
{
    user *user1_data = (user *)((ll_node_t*)user1)->data;
    user *user2_data = (user *)((ll_node_t*)user2)->data;
    if (user1_data->points == user2_data->points)
        return strcmp(user1_data->user_name, user2_data->user_name) < 0 ? 0 : 1;
    return user1_data->points > user2_data->points ? 0 : 1;
}

void
free_user(hashtable_t *ht_user)
{
    if (!ht_user)
        return;
    ht_free(ht_user);
}
