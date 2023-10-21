#ifndef __INI_FILE_H__
#define __INI_FILE_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 * Parsage rapide d'un fichier ini personnalisé avec reconnaissance de type 
 * convertit sous forme de liste ARRAY pour pouvoir être utilisé lors
 * de l'execution du programme
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "array.h"
#include "logger.h"
#include "my_string.h"

void *StringToAllocValue(TypeData type, char *str);
DATA *convertStringtoData(char *nomVariable, char *str, int line);
bool readInstruction(FILE *file, char *instruction, ARRAY *array, int *line);
bool readBlock(FILE *file, ARRAY *array, int *line);

/* Load ini file to string value with key, return true if success */
bool Array_loadINIFile(ARRAY *array, char *INI_File);

#endif /* __INI_FILE_H__ */