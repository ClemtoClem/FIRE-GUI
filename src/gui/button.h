#ifndef __BUTTON_H__
#define __BUTTON_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 *  _              _     _           
 * | |___   _   _ | |__ | |__   ___   ____
 * |  __ \ | | | ||  __)|  __) / _ \ |    \
 * |  \_\ \| |_| || |_/\| |_/\( |_| )|  |  \
 * \____  / \__,_|\____/\____/ \___/ |__|__/
 *      \/         
 */

#include "widget.h"

#define BUTTON_TILESET_NB_TILES_WIDTH 9
#define BUTTON_TILESET_NB_TILES_HEIGHT 3
#define BUTTON_NB_TILES_WIDTH 3
#define BUTTON_NB_TILES_HEIGHT 3

#define BUTTON_LABEL_WIDGET_ID ID_DEFAULT-1
#define BUTTON_SPRITE_WIDGET_ID ID_DEFAULT-2

typedef struct t_BUTTON {
	WIDGET *label, *sprite;
	bool changeSpriteByEvent;
	IMAGE *tileset;
	SDL_Rect tileRect;
	Sint32 offsetPressedX, offsetPressedY;
	bool moveOffset;
} BUTTON;

/**
 * @brief Allocation of a button widget
 * @param name_parent Parent widget identifier
 * @param id Widget Identifier
 * @param geometry Button geometry
 * @param area Button scroll area
 * @param tilesetName Background tileset name
 * @param offsetPressedX Offset child widgets in width to give a sense of depth when selecting buttons
 * @param offsetPressedY  Offset child widgets in height to give a sense of depth when selecting buttons
 * @param userFunc User function
 * @param userdata User data
 * @return Returns a widget
 */
WIDGET *Button_new(char *name_parent, char *name, SDL_Rect *geometry, SDL_Rect *area, char *tilesetName,
	Sint32 offsetPressedX, Sint32 offsetPressedY, callbackUserFunc userFunc, void *userdata);

/**
 * @brief Allocation of a button widget with label child widget
 * @param name_parent Parent widget identifier
 * @param id Widget Identifier
 * @param geometry Button geometry
 * @param area Button scroll area
 * @param tilesetName Background tileset name
 * @param message Text of label
 * @param fontName Label font used
 * @param sizeFont Label font size
 * @param colorFont Label font color
 * @param offsetPressedX Offset child widgets in width to give a sense of depth when selecting buttons
 * @param offsetPressedY  Offset child widgets in height to give a sense of depth when selecting buttons
 * @param userFunc User function
 * @param userdata User data
 * @return Returns a widget
 */
WIDGET *ButtonWithLabel_new(char *name_parent, char *name, SDL_Rect *geometry, SDL_Rect *area, char *tilesetName,
	char *message, char *fontName, Uint32 sizeFont, Uint32 colorFont,
	Sint32 offsetPressedX, Sint32 offsetPressedY, callbackUserFunc userFunc, void *userdata);

/**
 * si changeSpriteByEvent vaut true, le tileset devra mesurer 7 tiles en longueur et 1 tile et hauteur
 * tile 0 : no fucused tile
 * tile 1 : focused tile=
 * tile 2 : held tile=
*/
WIDGET *ButtonWithSprite_new(char *name_parent, char *name, SDL_Rect *geometry, SDL_Rect *area, char *tilesetName,
	SDL_Rect *spriteGeometry, SDL_Rect *spriteVisibleArea, char *tilesetSpriteName, bool changeSpriteByEvent,
	Sint32 offsetPressedX, Sint32 offsetPressedY, callbackUserFunc userFunc, void *userdata);

/**
 * @brief Free button private data
 * @param widget widget
 * @warning Don't use this function
 */
void Button_free(WIDGET *widget);

/**
 * @brief Manage button events
 * @param widget widget
 * @warning Don't use this function
 */
//void Button_event(WIDGET *widget);

/**
 * @brief Update button
 * @param widget widget
 * @warning Don't use this function
 */
void Button_update(WIDGET *widget);

/**
 * @brief Draw button
 * @param widget widget
 * @warning Don't use this function
 */
void Button_draw(WIDGET *widget);

#endif /* __BUTTON_H__ */