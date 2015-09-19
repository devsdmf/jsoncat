/*
 * ============================================================================
 *
 *       Filename:  tokens.h
 *
 *    Description:  Json object model token representation
 *
 *        Version:  1.0
 *        Created:  09/17/2015 08:46:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gustavo Pantuza
 *   Organization:  Software Community
 *
 * ============================================================================
 */

#ifndef TOKENS_H
#define TOKENS_H


/*
 * Tokens are based on the JSON object model definition: http://json.org
 */
#define OBJECT_OPEN     '{'
#define OBJECT_CLOSE    '}'

#define ARRAY_OPEN      '['
#define ARRAY_CLOSE     ']'

#define VALUE_SEPARATOR ','
#define PAIR_SEPARATOR  ':'

#define STRING_0        '\''
#define STRING_1        '"'
#define STRING_SCAPE    '\\'

#define TRUE_VALUE      900
#define FALSE_VALUE     901
#define NULL_VALUE      902

#define NUMBER          903


#endif // TOKENS_H
