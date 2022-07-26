// Copyright 2022 Slavu Ioan
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "utils.h"
#include "library.h"
#include "user.h"

int main(){
    // create hashtables for library and users
    hashtable_t *ht_library = NULL;
    ht_library = ht_create(MAX_BUCKET_SIZE,
                        hash_function_string, compare_function_strings);
    hashtable_t *ht_user = NULL;
    ht_user = ht_create(MAX_BUCKET_SIZE,
                        hash_function_string, compare_function_strings);
    while (1) {
        char args[200], command[MAX_INPUT_SIZE], arg1[MAX_INPUT_SIZE];
        char arg2[MAX_INPUT_SIZE], arg3[MAX_INPUT_SIZE], arg4[MAX_INPUT_SIZE];
        fgets(args, 200, stdin);

        int arg_nr = sscanf(args, "%s \"%[^\"]\" %s %s %s\n",
                                command, arg1, arg2, arg3, arg4);

        // ADD_BOOK <book_name> <def_number>
        if (strncmp(command, "ADD_BOOK", 8) == 0 && arg_nr == 3)
            add_book(ht_library, arg1, atoi(arg2));
        // GET_BOOK <book_name>
        else if (strncmp(command, "GET_BOOK", 8) == 0 && arg_nr == 2)
            get_book(ht_library, arg1);
        // RMV_BOOK <book_name>
        else if (strncmp(command, "RMV_BOOK", 8) == 0 && arg_nr == 2)
            delete_book(ht_library, arg1);
        // ADD_DEF <book_name> <def_key> <def_val>
        else if (strncmp(command, "ADD_DEF", 7) == 0 && arg_nr == 4)
            add_def_to_book(ht_library, arg1, arg2, arg3);
        // GET_DEF <book_name> <def_key>
        else if (strncmp(command, "GET_DEF", 7) == 0 && arg_nr == 3)
            get_def_from_book(ht_library, arg1, arg2);
        // RMV_DEF <book_name> <def_key>
        else if (strncmp(command, "RMV_DEF", 8) == 0 && arg_nr == 3)
            delete_def_from_book(ht_library, arg1, arg2);
        // ADD_USER <user_name>
        else if (strncmp(command, "ADD_USER", 8) == 0)
        {
            sscanf(args, "%s %s\n", command, arg1);
            add_user(ht_user, arg1);
        }
        // BORROW <user_name> <book_name> <days_available>
        else if (strncmp(command, "BORROW", 6) == 0)
        {
            sscanf(args, "%s %s \"%[^\"]\" %s\n", command, arg1, arg2, arg3);
            user_borrow_book(ht_user, ht_library, arg1, arg2, atoi(arg3));
        }
        // RETURN <user_name> <book_name> <days_since_borrow> <rating>
        else if (strncmp(command, "RETURN", 6) == 0)
        {
            sscanf(args, "%s %s \"%[^\"]\" %s %s\n",
                            command, arg1, arg2, arg3, arg4);
            user_return_book(ht_user, ht_library, arg1, arg2,
                                atoi(arg3), atof(arg4));
        }
        // LOST <user_name> <book_name>
        else if (strncmp(command, "LOST", 4) == 0)
        {
            sscanf(args, "%s %s \"%[^\"]\"\n", command, arg1, arg2);
            user_lost_book(ht_user, ht_library, arg1, arg2);
        }
        // EXIT
        else if (strncmp(command, "EXIT", 4) == 0)
        {
            top_books(ht_library);
            top_users(ht_user);
            free_library(ht_library);
            free_user(ht_user);
            break;
        }
        else
            continue;
    }
    return 0;
}
