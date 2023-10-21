#ifndef __ARRAY_H__
#define __ARRAY_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 * Array est une liste dans lequel on peut enregistrer different type de donnee lors de l'execution du programme
 * Le type de chaques donnees de la liste est renseigne par le type "TypeData"
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "logger.h"
#include "utils.h"

#define IGNORED NULL
#define NO_LIMITED (Uint32) 0
#define ID_DEFAULT (Uint32) (~0)

typedef enum t_TypeData {
	NIL_TYPE = 0,
	BOOL_TYPE = 1,
	UINT8_TYPE, SINT8_TYPE,
	UINT16_TYPE, SINT16_TYPE,
	UINT32_TYPE, SINT32_TYPE,
	UINT64_TYPE, SINT64_TYPE,
	FLOAT_TYPE, DOUBLE_TYPE,
	STRING_TYPE,
	
	BUFFER_TYPE, // type void*
	ARRAY_TYPE,
	RECT_TYPE, FLOAT_RECT_TYPE,
	CIRCLE_TYPE, FLOAT_CIRCLE_TYPE,
	FONT_TYPE,
	SURFACE_TYPE,
	TEXTURE_TYPE,
	IMAGE_TYPE,
	SOUND_TYPE,
	MUSIC_TYPE,

	ROOT_WIDGET_TYPE,
	RECTANGLE_WIDGET_TYPE,
	CIRCLE_WIDGET_TYPE,
	PANEL_WIDGET_TYPE,
	SPRITE_WIDGET_TYPE,
	LABEL_WIDGET_TYPE,
	BUTTON_WIDGET_TYPE,
	RADIO_WIDGET_TYPE,
	TOGGLE_WIDGET_TYPE,
	SCROLLBAR_WIDGET_TYPE,
	LEVELBAR_WIDGET_TYPE,
	COMBOBOX_WIDGET_TYPE
} TypeData;

typedef struct t_DATA {
	Uint32 id;
	char *key;
	TypeData type;

	void *buffer;
	void (*freeFunc) (void *, TypeData);
	
	struct t_DATA *next, *after;
} DATA;

typedef struct t_ARRAY {
	Uint32 limitedLength;
	Uint32 length;
	DATA *first, *last;
} ARRAY;

/* Create data */
DATA *Data_new(Uint32 id, char *key, TypeData type, void *buffer, void (*freeFunc) (void *, TypeData));

/* Delete data */
void Data_free(DATA *data, bool freeBuffer);

/* Create list */
ARRAY *Array_new(Uint32 limitedLength);

/* Delete list */
void Array_free(ARRAY *array, bool freeBuffer);

/* Clear array */
void Array_clear(ARRAY *array, bool freeBuffer);

/* Return the length of the list */
Uint32 Array_length(ARRAY *array);

/* Return true if array is full */
Uint32 Array_full(ARRAY *array);

/* Add data to the end of the list. */
void Array_append(ARRAY *array, DATA *data);

/* Add data to the begin of the array. */
void Array_prepend(ARRAY *array, DATA *data);

/* Del object to index in the list. */
void Array_delFromIndex(ARRAY *array, Uint32 index, bool freeBuffer);

/* Del first object with this id in the list. */
void Array_delFromId(ARRAY *array, Uint32 id, bool freeBuffer);

/* Del first  object with this key in the list. */
void Array_delFromKey(ARRAY *array, char *key, bool freeBuffer);

/* Del first object with this type in the list. */
void Array_delFromType(ARRAY *array, TypeData type, bool freeBuffer);

/* Insert object to index in the list. */
void Array_insert(ARRAY *array, Uint32 index, DATA *new_data);

/* Permute object to old index to index. */
void Array_permute(ARRAY *array, Uint32 index1, Uint32 index2);

/* Get last object the list. */
DATA *Array_getLast(ARRAY *array);

/* Get object to index in the list. */
DATA *Array_getFromIndex(ARRAY *array, Uint32 index);

/* Get first object with this id in the list. */
DATA *Array_getFromId(ARRAY *array, Uint32 id);

/* Get first  object with this key in the list. */
DATA *Array_getFromKey(ARRAY *array, char *key);

/* Get first object with this type in the list. */
DATA *Array_getFromType(ARRAY *array, TypeData type);

/* Get type of object to index in the list. */
TypeData Array_getTypeFromIndex(ARRAY *array, Uint32 index);

/* Get first type of object with this id in the list. */
TypeData Array_getTypeFromId(ARRAY *array, Uint32 id);

/* Get first type of object with this key in the list. */
TypeData Array_getTypeFromKey(ARRAY *array, char *key);

/* Type to string */
void ConvertTypeToString(TypeData type, char str[40]);

void Data_BufferToString(DATA *data, char *dest, size_t len);

/* convert list to string */
void Array_BufferToString(ARRAY *array, char *dest, size_t len);

/* create pointer on the constant */
bool	*ptr_bool(bool val);
Uint8	*ptr_uint8(Uint8 val);
Sint8	*ptr_int8(Sint8 val);
Uint16	*ptr_uint16(Uint16 val);
Sint16	*ptr_int16(Sint16 val);
Uint32	*ptr_uint32(Uint32 val);
Sint32	*ptr_int32(Sint32 val);
Uint64	*ptr_uint64(Uint64 val);
Sint64	*ptr_int64(Sint64 val);
float	*ptr_float(float val);
double	*ptr_double(double val);
char	*ptr_string(char *val);

/* ajouter un bool à la fin */
void Array_appendBool(ARRAY *array, bool value);
/* ajouter un Uint8 à la fin */
void Array_appendUint8(ARRAY *array, Uint8 value);
/* ajouter un Sint8 à la fin */
void Array_appendSint8(ARRAY *array, Sint8 value);
/* ajouter un Uint16 à la fin */
void Array_appendUint16(ARRAY *array, Uint16 value);
/* ajouter un Sint16 à la fin */
void Array_appendSint16(ARRAY *array, Sint16 value);
/* ajouter un Uint32 à la fin */
void Array_appendUint32(ARRAY *array, Uint32 value);
/* ajouter un Sint32 à la fin */
void Array_appendSint32(ARRAY *array, Sint32 value);
/* ajouter un Uint64 à la fin */
void Array_appendUint64(ARRAY *array, Uint64 value);
/* ajouter un Sint64 à la fin */
void Array_appendSint64(ARRAY *array, Sint64 value);
/* ajouter un float à la fin */
void Array_appendFloat(ARRAY *array, float value);
/* ajouter un double à la fin */
void Array_appendDouble(ARRAY *array, double value);
/* ajouter une chaine de caractères à la fin */
void Array_appendString(ARRAY *array, char *str);
/* ajouter un rectangle à la fin */
void Array_appendRect(ARRAY *array, SDL_Rect *rect);
/* ajouter un rectangle flottant à la fin */
void Array_appendFloatRect(ARRAY *array, SDL_FRect *rect);
/* ajouter un cercle à la fin */
void Array_appendCircle(ARRAY *array, CIRCLE *circle);
/* ajouter un cercle flottant à la fin */
void Array_appendFloatCircle(ARRAY *array, FCIRCLE *circle);

/* ajouter un bool au début */
void Array_prependBool(ARRAY *array, bool value);
void Array_prependUint8(ARRAY *array, Uint8 value);
void Array_prependSint8(ARRAY *array, Sint8 value);
void Array_prependUint16(ARRAY *array, Uint16 value);
void Array_prependSint16(ARRAY *array, Sint16 value);
void Array_prependUint32(ARRAY *array, Uint32 value);
void Array_prependSint32(ARRAY *array, Sint32 value);
void Array_prependUint64(ARRAY *array, Uint64 value);
void Array_prependSint64(ARRAY *array, Sint64 value);
void Array_prependFloat(ARRAY *array, float value);
void Array_prependDouble(ARRAY *array, double value);
void Array_prependString(ARRAY *array, char *str);
void Array_prependRect(ARRAY *array, SDL_Rect *rect);
void Array_prependFloatRect(ARRAY *array, SDL_FRect *rect);
void Array_prependCircle(ARRAY *array, CIRCLE *circle);
void Array_prependFloatCircle(ARRAY *array, FCIRCLE *circle);

bool Array_getBool(ARRAY *array, char *key, bool defaultValue);
Uint8 Array_getUint8(ARRAY *array, char *key, Uint8 defaultValue);
Sint8 Array_getSint8(ARRAY *array, char *key, Sint8 defaultValue);
Uint16 Array_getUint16(ARRAY *array, char *key, Uint16 defaultValue);
Sint16 Array_getSint16(ARRAY *array, char *key, Sint16 defaultValue);
Uint32 Array_getUint32(ARRAY *array, char *key, Uint32 defaultValue);
Sint32 Array_getSint32(ARRAY *array, char *key, Sint32 defaultValue);
Uint64 Array_getUint64(ARRAY *array, char *key, Uint64 defaultValue);
Sint64 Array_getSint64(ARRAY *array, char *key, Sint64 defaultValue);
float Array_getFloat(ARRAY *array, char *key, float defaultValue);
double Array_getDouble(ARRAY *array, char *key, double defaultValue);
void Array_getString(ARRAY *array, char *key, char *dest, char *defaultValue, size_t len);
void Array_getRect(ARRAY *array, char *key, SDL_Rect *destRect, SDL_Rect *defaultRect);
void Array_getFloatRect(ARRAY *array, char *key, SDL_FRect *destRect, SDL_FRect *defaultRect);
void Array_getFloatCircle(ARRAY *array, char *key, FCIRCLE *destCircle, FCIRCLE *defaultCircle);
void Array_getCircle(ARRAY *array, char *key, CIRCLE *destCircle, CIRCLE *defaultCircle);
ARRAY *Array_getArray(ARRAY *array, char *key);


void Array_print(ARRAY *array, size_t printLen);

void Array_test();

#endif /* __ARRAY_H__ */