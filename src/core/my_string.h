#ifndef __MY_STRING_H__
#define __MY_STRING_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>

#define INIT_STR_SIZE 50
#define ADD_STR_SIZE 10

typedef enum StringType {
    STRING, ID_STR, NAME_STR, COLOR_STR
} StringType;

typedef struct t_STR STR;
struct t_STR {
    StringType type;
    char *string;
    Uint64 size, len;

    STR *next;
};

typedef struct t_STRL {
    STR *first;
    Uint64 len;
} STRL;

STR *Str_new();
void Str_free(STR *str);
void Str_append(STR *str, char c);
void Str_prepend(STR *str, char c);
void Str_add(STR *str, char *cstr, Uint64 len);
void Str_concatenate(STR *str1, STR *str2);

STRL *StrList_new();
void StrList_free(STRL *strl);
STR *StrList_get(STRL *strl, Uint64 i);
void StrList_append(STRL *strl, STR *newstr);
void StrList_prepend(STRL *strl, STR *newstr);
void StrList_appendc(STRL *strl, Uint64 i, char c);
void StrList_prependc(STRL *strl, Uint64 i, char c);
void StrList_add(STRL *strl, Uint64 i, char *cstr, Uint64 len);

#endif /* __MY_STRING_H__ */