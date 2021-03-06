/*
 * ============================================================================
 *
 *       Filename:  lexical.c
 *
 *    Description:  This file is responsible of identify tokens of the json
 *                  object model
 *
 *        Version:  1.0
 *        Created:  09/15/2015 07:52:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gustavo Pantuza,
 *   Organization:  Software Community
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "tokens.h"
#include "colors.h"
#include "lexical.h"
#include "parsing.h"
#include "colors.h"



/*
 * Tries to open the input json file. Exits with failure in case of any error
 */
FILE*
open_json_file (char filename[])
{
    FILE *file;

    if (strcmp(filename, "-") == 0) {
        file = stdin;

    } else {
        file = fopen(filename, "r");
    }

    if (!file) {
        fprintf(stderr, RED "Error on openning file '%s': %s\n" NO_COLOR,
                filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return file;
}



/*
 * Check if we found a number
 */
void
check_number(char *character)
{
    if(isdigit(*character)) {
        *character = (char) NUMBER;
    }
}



/*
 * Function to identify if the token represents any valid Json symbol.
 * If it matches a symbol, the symbol parser is called. Otherwise
 * the program exits with errors
 */
void
match_symbol(char character, struct token *token, FILE *file,
             int *n_tabs, bool *in_array, options_t *options)
{
    char char_cpy = character;
    check_number(&character);

    switch (character) {

        case OBJECT_OPEN:
        {
            *in_array = false;
            parse_object(token, n_tabs, options->tab_stop);
            add_token(token, options);

            char next_char = getc(file);
            match_symbol(next_char, token, file, n_tabs, in_array, options);
            break;
        }

        case PAIR_SEPARATOR:
        {
            parse_pair_separator(token);
            add_token(token, options);

            char next_char = getc(file);
            match_symbol(next_char, token, file, n_tabs, in_array, options);
            break;
        }

        case OBJECT_CLOSE:
        {
            parse_object_close(token, n_tabs, options->tab_stop);
            add_token(token, options);

            char next_char = getc(file);
            match_symbol(next_char, token, file, n_tabs, in_array, options);
            break;
        }

        case ARRAY_OPEN:
        {
            *in_array = true;
            parse_array_open(token, n_tabs, options->tab_stop);
            add_token(token, options);

            char next_char = getc(file);
            match_symbol(next_char, token, file, n_tabs, in_array, options);
            break;
        }

        case VALUE_SEPARATOR:
        {
            parse_value_separator(token,*n_tabs, 
                                  *in_array, options->tab_stop);
            add_token(token, options);

            char next_char = getc(file);
            match_symbol(next_char, token, file, n_tabs, in_array, options);
            break;
        }

        case ARRAY_CLOSE:
        {
            *in_array = false;
            parse_array_close(token, n_tabs, options->tab_stop);
            add_token(token, options);

            char next_char = getc(file);
            match_symbol(next_char, token, file, n_tabs, in_array, options);
            break;
        }

        case STRING_0:
        {
            fprintf(stderr, RED "Json format do not allows single quotes: '\n"
                            NO_COLOR);
            exit(EXIT_FAILURE);
            break;
        }
        case STRING_1:
        {
            parse_string(token, character, file);
            add_token(token, options);

            char next_char = getc(file);
            match_symbol(next_char, token, file, n_tabs, in_array, options);
            break;
        }

        case '-':
        case ((char) NUMBER):
        {
            parse_number(token, char_cpy, file);
            add_token(token, options);

            char next_char = getc(file);
            match_symbol(next_char, token, file, n_tabs, in_array, options);
            break;
        }

        case 't':
        {
            parse_true_token(token, file);
            add_token(token, options);

            char next_char = getc(file);
            match_symbol(next_char, token, file, n_tabs, in_array, options);
            break;
        }

        case 'f':
        {
            parse_false_token(token, file);
            add_token(token, options);

            char next_char = getc(file);
            match_symbol(next_char, token, file, n_tabs, in_array, options);
            break;
        }

        case 'n':
        {
            parse_null_token(token, file);
            add_token(token, options);

            char next_char = getc(file);
            match_symbol(next_char, token, file, n_tabs, in_array, options);
            break;
        }
    }
}


/*
 * Search for tokens on the opened json file
 */
void
find_token (FILE *file, struct token *token, options_t *options)
{
    int n_tabs = 0;
    bool in_array = false;

    /* Gets the first character */
    char character = getc(file);

    /* Validates if the json inicialization is correct */
    if(character != OBJECT_OPEN && character != ARRAY_OPEN)
    {
        fprintf(stderr, RED "Json must start with "
                        "'" GRAY "%c" RED
                        "' or '" GRAY "%c" RED "'."
                        " Character '" GRAY "%c" RED
                        "' is not valid.\n" NO_COLOR,
                        OBJECT_OPEN, ARRAY_OPEN, character);
        exit(EXIT_FAILURE);
    }

    /* Recusively parses json stream */
    do {

        /* matches the token with an Json symbol */
        match_symbol(character, token, file, &n_tabs, &in_array, options);

        /* Gets the next char */
        character = getc(file);

    } while (character != EOF);
}



/*
 * Starts the parsing by doing the lexical analysis
 */
void
start_parsing (options_t* options)
{

    FILE *file = open_json_file(options->file_name);

    static struct token token;
    token.line = 0;
    token.column = 0;
    find_token(file, &token, options);
}
