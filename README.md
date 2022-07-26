# Implement a book library using HashTable
The program reads input from the keyboard 

- The command ADD_BOOK <book_name> <def_number>, adds to the hashtable of the library
the book, creates a hashtable for the book and reads the number of pairs of
words. For this command, the program waits for input from the keyboard
with the pairs of words in the form
    <def_key1> <def_val1>
    <def_key2> <def_val2>
- The command GET_BOOK <book_name> searches in the hashtable of the library if it exists
book <book_name> and if yes, display it.
- The command RMV_BOOK <book_name> searches in the hashtable of the library if it exists
book <book_name> and if so, delete it.
- The command ADD_DEF <book_name> <def_key> <def_val> searches in the hashtable
the bookstore if there is the book <book_name> and if so, add the pair
of words <def_key> <def_val> in the hashtable of the book
- The command GET_DEF <book_name> <def_key> searches in the hashtable
the bookstore if the book <book_name> exists and if so, search in the hashtable
books if the key exists, if so, display its value.
- The command RMV_DEF <book_name> <def_key> searches in the hashtable
the bookstore if the book <book_name> exists and if so, search in the hashtable
books if the key exists, if so, delete the definition
- The command ADD_USER <user_name> adds a user to the hashtable of users.
-The command BORROW <user_name> <book_name> <days_available> borrows a book
a user for a number of days.
-RETURN command <user_name> <book_name> <days_since_borrow> <rating> a user
returns a book, he can be penalized for delays, or maybe
to receive bonus points if he brings it earlier.
-The command LOST <user_name> <book_name> deletes the book from the library hashtable,
and apply a penalty to the user.
- The EXIT command stops the program, the top is calculated based on the book rating
and points to users and frees memory.