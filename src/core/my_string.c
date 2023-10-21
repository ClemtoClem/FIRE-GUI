#include "my_string.h"

STR *Str_new()
{
    STR *str = calloc(1, sizeof(STR));
    str->size = INIT_STR_SIZE;
    str->string = calloc(1, INIT_STR_SIZE);
    return str;
}

void Str_free(STR *str)
{
    if (str != NULL) {
        free(str->string);
        free(str);
    }
}

void Str_append(STR *str, char c)
{
    if (str != NULL) {
        str->string[str->len] = c;
        str->len++;
        if (str->len >= str->size) {
            str->size += ADD_STR_SIZE;
            str->string = realloc(str->string, str->size);
            Uint64 i;
            for (i = str->len; i < str->size; i++) str->string[i] = '\0';
        }
    }
}

void Str_prepend(STR *str, char c)
{
    if (str != NULL) {
        Uint64 i;
        str->len++;
        if (str->len >= str->size) {
            str->size += ADD_STR_SIZE;
            str->string = realloc(str->string, str->size);
            for (i = str->len; i < str->size; i++) str->string[i] = '\0';
        }
        for(i = 1; i < str->len; i++) str->string[i] = str->string[i-1];
        str->string[0] = c;
    }
}

void Str_add(STR *str, char *cstr, Uint64 len)
{
    if (str != NULL) {
        Uint64 i;
        Uint32 lastlen = str->len;
        str->len += len;
        if (str->len >= str->size) {
            str->size += (str->len - str->size) + ADD_STR_SIZE;
            str->string = realloc(str->string, str->size);
            for (i = str->len; i < str->size; i++) str->string[i] = '\0';
        }
        for (i = 0; i < len; i++) str->string[i+lastlen] = cstr[i];
    }
}

void Str_concatenate(STR *str1, STR *str2)
{
    if (str1 != NULL && str2 != NULL) {
        Uint64 i;
        Uint32 lastlen = str1->len;
        str1->len += str2->len;
        if (str1->len >= str1->size) {
            str1->size += (str1->len - str1->size) + ADD_STR_SIZE;
            str1->string = realloc(str1->string, str1->size);
            for (i = str1->len; i < str1->size; i++) str1->string[i] = '\0';
        }
        for (i = 0; i < str2->len; i++) str1->string[i+lastlen] = str2->string[i];
    }
}

/* ---------------------------------------- */

STRL *StrList_new()
{
    STRL *strl = calloc(1, sizeof(STRL));
    return strl;
}

void StrList_free(STRL *strl)
{
    if (strl != NULL) {
        STR *str = strl->first;
        STR *strnext;
        while (str != NULL) {
            strnext = str->next;
            Str_free(str);
            str = strnext;
        }
        free(strl);
    }
}

STR *StrList_get(STRL *strl, Uint64 i)
{
    STR *getstr = NULL;
    if (strl != NULL) {
        STR *str = strl->first;
        Uint64 j = 0;
        while (str != NULL) {
            str = str->next;
            if (i == j) getstr = str;
            j++;
        }
    }
    return getstr;
}

void StrList_append(STRL *strl, STR *newstr)
{
    if (strl != NULL) {
        STR **str = &(strl->first);
        while((*str) != NULL) {
            str = &((*str)->next);
        }
        (*str) = newstr;
        strl->len++;
    }
}

void StrList_prepend(STRL *strl, STR *newstr)
{
    if (strl != NULL) {
        newstr->next = strl->first;
        strl->first = newstr;
        strl->len++;
    }
}

void StrList_appendc(STRL *strl, Uint64 i, char c)
{
    if (strl != NULL) {
        STR *str = strl->first;
        Uint64 j = 0;
        while (str != NULL) {
            str = str->next;
            if (i == j) Str_append(str, c);
            j++;
        }
    }
}

void StrList_prependc(STRL *strl, Uint64 i, char c)
{
    if (strl != NULL) {
        STR *str = strl->first;
        Uint64 j = 0;
        while (str != NULL) {
            str = str->next;
            if (i == j) Str_prepend(str, c);
            j++;
        }
    }
}

void StrList_add(STRL *strl, Uint64 i, char *cstr, Uint64 len)
{
    if (strl != NULL) {
        STR *str = strl->first;
        Uint64 j = 0;
        while (str != NULL) {
            str = str->next;
            if (i == j) {
                Str_add(str, cstr, len);
                strl->len++;
            }
            j++;
        }
    }
}