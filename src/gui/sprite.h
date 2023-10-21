#ifndef __SPRITE_H__
#define __SPRITE_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 *                     ( ) _ 
 *  ______ ____   _ __  _ | |__   ___
 * / ____/| __ \ | '__|| ||  __) / _ \ 
 * \____ \| \_\ || |   | || |_/\(  ___)
 * /_____/|  __/ |_|   |_|\____/ \____\
 *        |_|
 * Fixed or Animating a tileset
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "gui.h"

struct FillSurfaceWithTile {
	bool enable;
	Sint32 w, h;
};

typedef struct t_SPRITE {
	IMAGE *tileset;
	SDL_Rect tileRect, clipRect;
	Uint16 nbTilesW, nbTilesH;

	struct FillSurfaceWithTile fillSurfaceWithTile;

	Orientation orientation;
	Uint16 frame;
	Uint16 frameBetweenTile, frameAfterRepeat;
	Uint16 currentTile; // id de l'image à afficher
	bool hasBeenConfigured, animate, end, loop;
} SPRITE;

/**
 * @brief Allocation of a sprite widget
 * @param name_parent Parent widget identifier
 * @param id Widget Identifier
 * @param geometry Image size
 * @param visibleArea Part of the image to display
 * @param tilesetName Tileset name
 * @param reapeatTile Fill area to repeat sprite
 * @param userFunc User function
 * @param userdata User data
 * @return Returns a widget
 */
WIDGET *Sprite_new(char *name_parent, char *name, SDL_Rect *geometry, SDL_Rect *visibleArea, char *tilesetName,
	callbackUserFunc userFunc, void *userdata);

/**
 * @brief Free sprite private data
 * @param widget widget
 * @warning Don't use this function
 */
void Sprite_free(WIDGET *widget);

void Sprite_setFillSurfaceWithTile(WIDGET *widget, Sint32 width, Sint32 height);

/**
 * @brief Configure tileset animation
 * @param widget widget
 * @param animate true if you want to animate the sprite
 * @param resize true if we want to resize the geometry of the sprite according to the size of a tile
 * @param nbTilesW number of sprites in width
 * @param nbTilesH number of sprites in height
 * @param orientation orientation of animate sprite
 * @param frameBetweenTile number of frames between each tile change
 * @param frameAfterRepeat number of frames between each animation
 * @param loop true if we repeat the animation at the end
 */
void Sprite_configureAnimation(WIDGET *widget, bool animate, bool resize, Uint16 nbTilesW, Uint16 nbTilesH,
	Orientation orientation, Uint16 frameBetweenTile, Uint16 frameAfterRepeat, bool loop);

/**
 * @brief Start animation
 * @param widget widget
 */
void Sprite_start(WIDGET *widget);

/**
 * @brief Stop animation
 * @param widget widget
 */
void Sprite_stop(WIDGET *widget);

/**
 * @brief Choose current tile
 * @param widget widget
 * @param num tile number
 */
void Sprite_setCurrentTile(WIDGET *widget, Uint16 num);

/**
 * @brief Next animation
 * @param widget widget
 */
void Sprite_next(WIDGET *widget);

/**
 * @brief After animation
 * @param widget widget
 */
void Sprite_after(WIDGET *widget);

/**
 * @brief Manage sprite events
 * @param widget widget
 * @warning Don't use this function
 */
//void Sprite_event(WIDGET *widget);

/**
 * @brief Update sprite
 * @param widget widget
 * @warning Don't use this function
 */
void Sprite_update(WIDGET *widget);

/**
 * @brief Draw panel
 * @param widget widget
 * @warning Don't use this function
 */
void Sprite_draw(WIDGET *widget);

#endif /* __SPRITE_H__ */