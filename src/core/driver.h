#ifndef __DRIVER_H__
#define __DRIVER_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 * Graphics permet de regrouper tous les elements les plus important de la SDL.
 * Deplus les ressources sont enregistre dans une liste ce qui evite de les reimportes à chaque fois
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "logger.h"
#include "array.h"
#include "image.h"
#include "colors.h"

typedef struct t_DRIVER {
	Uint32 width, height, bgColor;
	SDL_Window *window;
	SDL_Renderer *renderer;
	ARRAY *ressources;
	Mix_Music *currentMusic;
	bool audioInited, pauseMusic;
	Sint32 volume;
	int loopMusic;
} DRIVER;

extern DRIVER *driver;

/* ----- GRAPHICS ----- */

/* Convertir une couleur RGBA 32 bits en structure SDL_Color */
SDL_Color hexColorToRGBA(Uint32 hex);

/**
 * @brief Initialisation de la structure Driver ("driver" est une variable globale ne pas initialiser)
*/
bool Driver_init(char *title, Uint32 width, Uint32 height, Uint32 bgColor, Uint32 windowflags);

/**
 * @brief Detruire la structure Driver
*/
void Driver_free();

DRIVER *Driver_get();

bool Drvier_makeWindowTransparent();

char *Driver_getTitle();
void Driver_setTitle(char *title);
Sint32 Driver_getWidth();
Sint32 Driver_getHeight();
Uint32 Driver_getBgColor();
void Driver_setBgColor(Uint32 bgColor);

SDL_Texture *Driver_createTextureFromSurface(SDL_Surface *surface);

/**
 * @brief Importer une image (.png)
 * @attention Ne pas free l'image, cela se fera automatiquement à la destruction de la structure Driver
 * @param imzgeName nom de l'image à importer dans le dossier ./res/images/
 */
IMAGE *Driver_loadImage(char *imageName);
void freeImage(void *buffer, TypeData type);

/**
 * @brief Importer une police (.ttf)
 * @attention Ne pas free la police, cela se fera automatiquement à la destruction de la structure Driver
 * @param fontName nom de la police à importer dans le dossier ./res/fonts/
 * @param fontSize taille de la police
 */
TTF_Font *Driver_loadFont(char *fontName, Uint16 fontSize);
void freeFont(void *buffer, TypeData type);

/**
 * @brief Effacer le renderer
 */
void Driver_clear();

SDL_Renderer *Driver_getRenderer();

/**
 * @brief Sélectionner une couleur de dessin
 */
void Driver_drawColor(Uint32 color);

/**
 * @brief Dessiner un rectangle dans le renderer
 */
void Driver_drawRect(SDL_Rect *rect, bool fill);

/**
 * @brief Envoyer le renderer au processeur graphique
 */
void Driver_present();

/**
 * @brief Insérer une texture dans le renderer
 */
void Driver_copy(SDL_Texture *texture, SDL_Rect *src, SDL_Rect *dest);

/**
 * @brief Créer une texture
*/
SDL_Texture *Driver_createTexture(Sint32 w, Sint32 h);

/**
 * @brief Set a texture as the current rendering target
 */
void Driver_setTarget(SDL_Texture *texture);

/**
 * @brief Insérer du text dans le renderer
 * @param text text à afficher
 * @param font police
 * @param textColor couleur du texte
 * @param rect rectangle pour restrindre l'affichage du texte (w = 0 pour ne pas restrindre la longeur, h = 0 pour ne pas restrindre la largeur)
 * @param centerX centrer le texte dans le rectangle
 * @param centerY centrer le texte dans le rectangle
*/
void Driver_copyText(char* text, TTF_Font* font, Uint32 textColor, SDL_Rect *rect, bool centerX, bool centerY);


/**
 * @brief Adaptation du printf pour afficher du texte formatté dans l'interface graphique
*/
void Driver_printf(Sint32 x, Sint32 y, TTF_Font *font, Uint32 textColor, char *text, ... );

/* ----- AUDIO ----- */

bool Driver_openAudio();
void Driver_closeAudio();

void Driver_setVolume(Sint32 volume);
Sint32 Driver_getVolume();

Mix_Chunk* Driver_loadSound(char *soundName);
void freeSound(void *buffer, TypeData type);

void Driver_playSound(char *soundName, int loops);
void Driver_pauseSound(char *soundName);
void Driver_resumeSound(char *soundName);

Mix_Music* Driver_loadMusic(char *musicName);
void freeMusic(void *buffer, TypeData type);

void Driver_playMusic(char *musicName, int loops);
void Driver_stopMusic();
void Driver_pauseMusic();
void Driver_resumeMusic();

#endif  /* __DRIVER_H__ */