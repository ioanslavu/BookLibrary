// Copyright 2022 Slavu Ioan
#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// messages
void
show_book(const char* book_name, double book_rating, int book_purchases);

void
show_user(const char* user_name, int points);

void
book_not_found_message();

void
definition_not_found_message();

void
user_not_found_message();

void
user_already_registered_message();

void
user_banned_message();

void
book_is_borrowed_message();

void
user_already_borrowed_book_message();

void
ban_user_message(const char* user_name);

void
user_not_borrowed_book_message();

void
invalid_command_message();

#endif  // MESSAGES_H_
