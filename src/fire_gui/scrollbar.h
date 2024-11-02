#ifndef __FIRE_GUI_SCROLLBAR_H__
#define __FIRE_GUI_SCROLLBAR_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 *                            _  _  _ 
 *  ______  ____  _ __  ___  | || || |___    ____  _ __
 * / ____/ / ___\| '__|/ _ \ | || ||  __ \  / _` || '__|
 * \____ \| (____| |  ( |_| )| || ||  \_\ \| (_| || |
 * /_____/ \____/|_|   \___/ |_||_|\____  / \__,_||_|
 *                                      \/
 * Horizontal and vertical scrollbar
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "fire_gui_main.h"

#define SCROLLBAR_TILESET_NB_TILES_WIDTH 6
#define SCROLLBAR_TILESET_NB_TILES_HEIGHT 3
#define SCROLLBAR_NB_TILES_WIDTH 3
#define SCROLLBAR_NB_TILES_HEIGHT 3

#define SCROLLBAR_BUTTON_TILESET_NB_TILES_WIDTH 9
#define SCROLLBAR_BUTTON_TILESET_NB_TILES_HEIGHT 3
#define SCROLLBAR_BUTTON_NB_TILES_WIDTH 3
#define SCROLLBAR_BUTTON_NB_TILES_HEIGHT 3

typedef struct t_SCROLLBAR {
	IMAGE *tileset, *buttonTileset;
	SDL_Rect tileRect, buttonTileRect;
    SDL_Rect buttonAbsolute, buttonClipRect;
	Orientation orientation;
	Sint32 min, max, step, currentValue;
} SCROLLBAR;

/**
 * @brief Allocation of a scrollbar widget
 * @param name_parent Parent widget identifier
 * @param id Widget Identifier
 * @param geometry Geometry of label (if you want the label to make the size of
 * the text set the width and or the height to zero or set NULL)
 * @param orientation horizontal or vertical
 * @param tilesetName
 * @param buttonTilesetName
 * @param orientation
 * @param min
 * @param max
 * @param step
 * @param userFunc User function
 * @param userdata User data
 * @return Returns a widget
 */
WIDGET *Scrollbar_new(char *name_parent, char *name, SDL_Rect *geometry,
	char *tilesetName, char *buttonTilesetName, Orientation orientation,
	Sint32 min, Sint32 max, Sint32 step, callbackUserFunc userFunc, void *userdata);

/**
 * @brief Free label private data
 * @param widget widget
 * @warning Don't use this function
 */
void Scrollbar_free(WIDGET *widget);

/**
 * @brief Manage label events
 * @param widget widget
 * @warning Don't use this function
 */
//void Scrollbar_event(WIDGET *widget);

/**
 * @brief Update label
 * @param widget widget
 * @warning Don't use this function
 */
void Scrollbar_update(WIDGET *widget);

/**
 * @brief Draw label
 * @param widget widget
 * @warning Don't use this function
 */
void Scrollbar_draw(WIDGET *widget);

#endif /* __FIRE_GUI_SCROLLBAR_H__ */