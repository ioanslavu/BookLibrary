// Copyright 2022 Slavu Ioan
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "messages.h"

void
show_book(const char* book_name, double book_rating, int book_purchases)
{
    printf("Name:%s Rating:%.3f Purchases:%d\n", book_name,
            book_rating, book_purchases);
}
void
show_user(const char* user_name, int points)
{
    printf("Name:%s Points:%d\n", user_name, points);
}
void
book_not_found_message()
{
    printf("The book is not in the library.\n");
}
void
definition_not_found_message()
{
    printf("The definition is not in the book.\n");
}
void
user_not_found_message()
{
    printf("You are not registered yet.\n");
}
void
user_already_registered_message()
{
    printf("User is already registered.\n");
}
void
user_banned_message()
{
    printf("You are banned from this library.\n");
}
void
book_is_borrowed_message()
{
    printf("The book is borrowed.\n");
}
void
user_already_borrowed_book_message()
{
    printf("You have already borrowed a book.\n");
}
void
ban_user_message(const char* user_name)
{
    printf("The user %s has been banned from this library.\n", user_name);
}
void
user_not_borrowed_book_message()
{
    printf("You didn't borrow this book.\n");
}
void
invalid_command_message()
{
    printf("Invalid command. Please try again.\n");
}
