#ifndef __TOGGLE_H__
#define __TOGGLE_H__

/*
 ********************************************
 *				SOURCE CODE				 	*
 ********************************************
 *  _                          _
 * | |__   ___    ___    ___  | |  ___  
 * |  __) / _ \  / _ \  / _ \ | | / _ \ 
 * | |_/\( |_| )| |_| || |_| || |(  ___)
 * \____/ \___/ | ___/ | ___/ |_| \____\
 *              / ___ \/ ___ \
 *              \_____/\_____/
 */

#include "widget.h"

#define TOGGLE_TILESET_NB_TILES_WIDTH 9
#define TOGGLE_TILESET_NB_TILES_HEIGHT 6
#define TOGGLE_NB_TILES_WIDTH 3
#define TOGGLE_NB_TILES_HEIGHT 3

typedef struct t_TOGGLE {
	WIDGET *left_label, *right_label, *button_label;
	WIDGET *left_sprite, *right_sprite, *button_sprite;
	bool changeSpriteByEvent;
	IMAGE *tileset;
	SDL_Rect tileRect;
	SDL_Texture *texture;
	Sint32 offsetPressedX, offsetPressedY;
	bool state;
} TOGGLE;

/**
 * @brief Allocation of a toggle button widget with label child widget
 * @param name_parent Parent widget identifier
 * @param id Widget Identifier
 * @param geometry Toggle button geometry
 * @param area Toggle button scroll area
 * @param tilesetName Background tileset name
 * @param offsetPressedX Offset child widgets in width to give a sense of depth when selecting buttons
 * @param offsetPressedY  Offset child widgets in height to give a sense of depth when selecting buttons
 * @param state Toggle button state
 * @param userFunc User function
 * @param userdata User data
 * @return Returns a widget
 */
WIDGET *Toggle_new(char *name_parent, char *name, SDL_Rect *geometry, SDL_Rect *area, char *tilesetName,
	Sint32 offsetPressedX, Sint32 offsetPressedY, bool state, callbackUserFunc userFunc, void *userdata);

/**
 * @brief Free toggle button private data
 * @param widget widget
 * @warning Don't use this function
 */
void Toggle_free(WIDGET *widget);

/**
 * @brief Fix state of toggle button
 * @param widget widget
 * @param state state
 */
void Toggle_setState(WIDGET *widget, bool state);

/**
 * @brief Return state of toggle button
 * @param widget widget
 */
bool Toggle_getState(WIDGET *widget);

/**
 * @brief Change state of toggle button
 * @param widget widget
 * @param state
 */
void Toggle_changeState(WIDGET *widget);

void Toggle_configLeftLabel(WIDGET *widget, char *message, char *fontName, Uint32 sizeFont, Uint32 colorFont);

void Toggle_configRightLabel(WIDGET *widget, char *message, char *fontName, Uint32 sizeFont, Uint32 colorFont);

void Toggle_configButtonLabel(WIDGET *widget, char *message, char *fontName, Uint32 sizeFont, Uint32 colorFont);

void Toggle_configLeftSprite(WIDGET *widget, SDL_Rect *spriteGeometry, char *tilesetSpriteName, bool changeSpriteByEvent);

void Toggle_configRightSprite(WIDGET *widget, SDL_Rect *spriteGeometry, char *tilesetSpriteName, bool changeSpriteByEvent);

void Toggle_configButtonSprite(WIDGET *widget, SDL_Rect *spriteGeometry, char *tilesetSpriteName, bool changeSpriteByEvent);

/**
 * @brief Manage toggle button events
 * @param widget widget
 * @warning Don't use this function
 */
void Toggle_event(WIDGET *widget);

/**
 * @brief Update toggle button
 * @param widget widget
 * @warning Don't use this function
 */
void Toggle_update(WIDGET *widget);

/**
 * @brief Draw toggle button
 * @param widget widget
 * @warning Don't use this function
 */
void Toggle_draw(WIDGET *widget);

#endif /* __TOGGLE_H__ */