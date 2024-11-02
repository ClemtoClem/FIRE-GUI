#include "iniFile.h"

bool estUnEspace(char c);
bool estUnChiffre(char c);
bool estUnCaractereSyntaxique(char c);
bool stringCompare(char *str1, char *str2, size_t len);

bool estUnEspace(char c) {
	if (c == ' ' || c == '\t') return true;
	return false;
}

bool estUnChiffre(char c) {
	if (c >= '0' && c <= '9') return true;
	return false;
}

bool estUnCaractereSyntaxique(char c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || estUnChiffre(c)) return true;
	return false;
}

bool stringCompare(char *str1, char *str2, size_t len)
{
	size_t j;
	for (j = 0; j < len; j++) {
		if (str1[j] != str2[j]) return false;
	}
	return true;
}

void *StringToAllocValue(TypeData type, char *str)
{
	void *buffer = NULL;
	switch (type) {
		case UINT8_TYPE: {
			Uint8 val;
			sscanf(str, "%hhu", &val);
			buffer = (void *) ptr_uint8(val);
		} break;
		case SINT8_TYPE: {
			Sint8 val;
			sscanf(str, "%hhd", &val);
			buffer = (void *) ptr_int8(val);
		} break;
		case UINT16_TYPE: {
			Uint16 val;
			sscanf(str, "%hu", &val);
			buffer = (void *) ptr_uint16(val);
		} break;
		case SINT16_TYPE: {
			Sint16 val;
			sscanf(str, "%hd", &val);
			buffer = (void *) ptr_int16(val);
		} break;
		case UINT32_TYPE: {
			Uint32 val;
			sscanf(str, "%u", &val);
			buffer = (void *) ptr_uint32(val);
		} break;
		case SINT32_TYPE: {
			Sint32 val;
			sscanf(str, "%d", &val);
			buffer = (void *) ptr_int32(val);
		} break;
		case UINT64_TYPE: {
			Uint64 val;
			sscanf(str, "%llu", &val);
			buffer = (void *) ptr_uint64(val);
		} break;
		case SINT64_TYPE: {
			Sint64 val;
			sscanf(str, "%lld", &val);
			buffer = (void *) ptr_int64(val);
		} break;
		case FLOAT_TYPE: {
			float val;
			sscanf(str, "%f", &val);
			buffer = (void *) ptr_float(val);
		} break;
		case DOUBLE_TYPE: {
			double val;
			sscanf(str, "%lf", &val);
			buffer = (void *) ptr_double(val);
		} break;
		default: break;
	}
	return buffer;
}

bool readInstruction(FILE *file, char *instruction, ARRAY *array, int *line)
{
	char c;
	STRL *listVariables = StrList_new();
	STR *valeurVariable = NULL;

	bool definirVariable = false;
	bool definirValeur = false;
	size_t i = 0;

	bool success = false;
	
	/* si c'est un commentaire on ignore tout jusqu'à la fin de la ligne */
	if (instruction[0] == '#') { success = true; goto endReadInstruction; }

	for (i = 0; i < strlen(instruction); i++) {
		c = instruction[i];
		
		/* si c'est un caractère syntaxique on commence à recherche le nom de la variable */
		if (!definirValeur && !definirVariable) {
			if (c == '%' && StrList_get(listVariables, listVariables->len-1)->type != ID_STR) StrList_get(listVariables, listVariables->len-1)->type = ID_STR;
			else if (estUnCaractereSyntaxique(c)) {
				if (StrList_get(listVariables, listVariables->len-1)->type != NAME_STR) StrList_get(listVariables, listVariables->len-1)->type = NAME_STR;
				StrList_append(listVariables, Str_new());
				StrList_appendc(listVariables, listVariables->len-1, c);
				definirVariable = true;
			} else {
				LOG_ERROR("ligne %d, Mauvaise syntaxe, utilisation de caractere interdit pour definir une variable (%s)", *line, instruction);
				goto endReadInstruction;
			}
		} else if (definirVariable) {
			/* si c'est un caractère syntaxique on continue à recherche le nom de la variable */
			if (estUnCaractereSyntaxique(c)) {
				StrList_appendc(listVariables, listVariables->len-1, c);
			}
			else if (c == ',') {
				StrList_append(listVariables, Str_new());
			}
			/* on fini de rechercher le nom de la variable si on arrive à un = */
			else if (c == '=') {
				if (c != '\n' && c != '\0') {
					valeurVariable = Str_new();
					Str_append(valeurVariable, c);
				}

				definirVariable = false;
				definirValeur = true;
			} else {
				LOG_ERROR("ligne %d, Mauvaise syntaxe, utilisation de caractere interdit pour definir une variable (%s)", *line, instruction);
				goto endReadInstruction;
			}
		} else if (definirValeur) {
			if (c == '\n' || c == '\0') goto nextReadInstruction;
			Str_append(valeurVariable, c);
		}
	}

nextReadInstruction:
	/* On recherche la valeur et le type de la variable */
	if (valeurVariable != NULL) {
		/* si c'est un bloc on créer une nouvelle liste */
		if (valeurVariable->string[0] == '{') {
			ARRAY *array2 = Array_new(NO_LIMITED);
			if (!readBlock(file, array2, line)) goto endReadInstruction;
			
			STR *str = listVariables->first;
			while(str != NULL) {
				if (str->type == ID_STR) {
					Uint32 id;
					if (sscanf(str->string, "%u", &id) == 1)
						Array_append(array, Data_new(id, NULL, ARRAY_TYPE, array2, NULL));
					else {
						LOG_ERROR("ligne %d, Identifiant incorrect (%s)", *line, instruction);
						goto endReadInstruction;
					}
				} else if (str->type == NAME_STR) {
					Array_append(array, Data_new(ID_DEFAULT, str->string, ARRAY_TYPE, array2, NULL));
				}
			}
			success = true;
			goto endReadInstruction;
		} else {
			STR *str = listVariables->first;
			while(str != NULL) {
				DATA *data = convertStringtoData(str->string, valeurVariable->string, *line);
				if (data != NULL) {
					Array_append(array, data);
					success = true;
					goto endReadInstruction;
				}
			}
		}
	}
endReadInstruction:
	StrList_free(listVariables);
	Str_free(valeurVariable);
	return success;
}

DATA *convertStringtoData(char *nomVariable, char *str, int line)
{	
	bool estUnNombre = false;
	bool decimal = false, negatif = false;
	char c, valeurVariable[500] = "\0";
	size_t i, j = 0;
	TypeData type = NIL_TYPE;

	/* sinon on recherche le type et la valeur de la variable et on l'ajoute dans la liste */
	if (stringCompare(str, "true", 4) || stringCompare(str, "yes", 3)) {
		type = BOOL_TYPE;
		return Data_new(ID_DEFAULT, nomVariable, BOOL_TYPE, ptr_bool(true), NULL);
	} else if (stringCompare(str, "false", 5) || stringCompare(str, "no", 2)) {
		type = BOOL_TYPE;
		return Data_new(ID_DEFAULT, nomVariable, BOOL_TYPE, ptr_bool(false), NULL);
	} else if (stringCompare(str, "circle(", 7)) {
		CIRCLE *circle = malloc(sizeof(CIRCLE));
		if (sscanf(str, "circle(%d,%d,%d)", &circle->x, &circle->y, &circle->radius) != 3) {
			LOG_ERROR("ligne %d, Mauvais parametre pour definir le cercle", line);
			return false;
		}
		return Data_new(ID_DEFAULT, nomVariable, CIRCLE_TYPE, circle, NULL);
	} else if (stringCompare(str, "rect(", 5)) {
		SDL_Rect *rect = malloc(sizeof(SDL_Rect));
		if (sscanf(str, "rect(%d,%d,%d,%d)", &rect->x, &rect->y, &rect->w, &rect->h) != 4) {
			LOG_ERROR("ligne %d, Mauvais parametre pour definir le rectangle", line);
			return false;
		}
		return Data_new(ID_DEFAULT, nomVariable, RECT_TYPE, rect, NULL);
	} else if (stringCompare(str, "fcircle(", 8)) {
		FCIRCLE *circle = malloc(sizeof(FCIRCLE));
		if (sscanf(str, "fcircle(%f,%f,%f)", &circle->x, &circle->y, &circle->radius) != 3) {
			LOG_ERROR("ligne %d, Mauvais parametre pour definir le cercle", line);
			return false;
		}
		return Data_new(ID_DEFAULT, nomVariable, FLOAT_CIRCLE_TYPE, circle, NULL);
	} else if (stringCompare(str, "frect(", 6)) {
		SDL_FRect *rect = malloc(sizeof(SDL_FRect));
		if (sscanf(str, "frect(%f,%f,%f,%f)", &rect->x, &rect->y, &rect->w, &rect->h) != 4) {
			LOG_ERROR("ligne %d, Mauvais parametre pour definir le cercle", line);
			return false;
		}
		return Data_new(ID_DEFAULT, nomVariable, FLOAT_RECT_TYPE, rect, NULL);
	} else if (str[0] == '"') {
		j = 0;
		while(str[j+1] != '\0' && str[j+1] != '\n') {
			if (str[j+1] == '"' && str[j] != '\\') break;
			valeurVariable[j] = str[j+1];
			j++;
		}
		valeurVariable[j] = '\0';
		return Data_new(ID_DEFAULT, nomVariable, STRING_TYPE, strdup(valeurVariable), NULL);
	} else {
		j = 0;
		for (i = 0; i < strlen(str); i++) {
			c = str[i];
			if (c == 'u') {
				if (type == NIL_TYPE) type = UINT32_TYPE;
				else if (type == SINT16_TYPE) type = UINT16_TYPE;
				else if (type == SINT8_TYPE) type = UINT8_TYPE;
				else if (type == UINT64_TYPE) type = UINT64_TYPE;
				else {
					LOG_ERROR("ligne %d, Le typage doit etre defini avant la valeur", line);
					return NULL;
				}
			} else if (c == 'h') {
				if (type == NIL_TYPE) type = SINT16_TYPE;
				else if (type == SINT16_TYPE) type = SINT8_TYPE;
				else {
					LOG_ERROR("ligne %d, Le typage doit etre defini avant la valeur", line);
					return NULL;
				}
			} else if (c == 'l') {
				if (type == NIL_TYPE) type = SINT64_TYPE;
			} else if (c == 'f') {
				if (type == NIL_TYPE) type = FLOAT_TYPE;
				else if (type == SINT64_TYPE) type = DOUBLE_TYPE;
				else {
					LOG_ERROR("ligne %d, Le typage doit etre defini avant la valeur", line);
					return NULL;
				}
			} else if (c == '.') {
				if (type == NIL_TYPE) {
					type = FLOAT_TYPE;
					estUnNombre = true;
					valeurVariable[j] = '0';
					valeurVariable[j+1] = '.';
					decimal = true;
					j+=2;
				} else if (type == SINT32_TYPE || (type == FLOAT_TYPE && decimal == false)) {
					type = FLOAT_TYPE;
					valeurVariable[j] = '.';
					j++;
				} else {
					LOG_ERROR("ligne %d, Un flottant ne comporte qu'un point", line);
					return NULL;
				}
			} else if (c == '-') {
				if (type == NIL_TYPE && j == 0 && negatif == false) {
					type = SINT32_TYPE;
					estUnNombre = true;
					negatif = true;
					valeurVariable[j] = '-';
					j++;
				} else {
					LOG_ERROR("ligne %d, Mauvaise position du signe", line);
					return NULL;
				}	
			} else if (estUnChiffre(c)) {
				if (type == NIL_TYPE) { type = SINT32_TYPE; estUnNombre = true; }
				else if (!estUnNombre && type >= UINT8_TYPE && type <= DOUBLE_TYPE) estUnNombre = true;
				if (estUnNombre) {
					valeurVariable[j] = c;
					j++;
				}
			}
		}

		if (estUnNombre) {
			valeurVariable[j] = '\0';
			return Data_new(ID_DEFAULT, nomVariable, type, StringToAllocValue(type, valeurVariable), NULL);
		}	
	}
	return NULL;
}

bool readBlock(FILE *file, ARRAY *array, int *line)
{
	int i;
	char c, instruction[600];
	while (feof(file) == 0) {
		instruction[0] = '\0';
		for (i = 0 ;; i++) {
			c = fgetc(file);
			if (c != '\n' && c != '\0' && c != EOF) {
				if (!estUnEspace(c)) instruction[i] = c;
				else i--;
			} else {
				instruction[i] = '\0';
				break;
			}
		}

		if (instruction[0] == '}') return true;
		if (instruction[0] != '\n' && instruction[0] != '\0')
			if (!readInstruction(file, instruction, array, line))
				return false;
		
		(*line)++;
	}
	return true;
}

bool Array_loadINIFile(ARRAY *array, char *INI_File)
{
	bool success = false;

	char filePath[100];
	FILE *file = NULL;

	sprintf(filePath, "./%s", INI_File);
	file = fopen(filePath, "r");
	if (file == NULL) {
		sprintf(filePath, "../%s", INI_File);
		file = fopen(filePath, "r");
	}
	if (file == NULL) {
		LOG_ERROR("Fichier INI \"%s\" introuvable", INI_File);
		return false;
	}

	int line = 0;
	
	success = readBlock(file, array, &line);

	fclose(file);
	return success;

}