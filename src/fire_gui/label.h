#ifndef __FIRE_GUI_LABEL_H__
#define __FIRE_GUI_LABEL_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 *  _         _              _
 * | |  ____ | |___    ___  | |
 * | | / _  ||  __ \  / _ \ | |
 * | |( ( | ||  \_\ \(  ___)| |
 * |_| \_||_|\____  / \____\|_|
 *                \/ 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "fire_gui_main.h"

typedef struct t_LABEL {
	char *message;
	TTF_Font *font;
	SDL_Texture *texture;
	SDL_Rect surfaceRect;
	Uint32 colorFont;
} LABEL;

/**
 * @brief Allocation of a label widget
 * @param name_parent Parent widget identifier
 * @param id Widget Identifier
 * @param geometry Geometry of label (if you want the label to make the size of
 * the text set the width and or the height to zero or set NULL)
 * @param message Text to display
 * @param fontName Font used
 * @param sizeFont Font size
 * @param colorFont Font color
 * @param userFunc User function
 * @param userdata User data
 * @return Returns a widget
 */
WIDGET *Label_new(char *name_parent, char *name, SDL_Rect *geometry,
	char *message, char *fontName, Uint32 sizeFont, Uint32 colorFont,
	callbackUserFunc userFunc, void *userdata);

/**
 * @brief Free label private data
 * @param widget widget
 * @warning Don't use this function
 */
void Label_free(WIDGET *widget);

/**
 * @brief Change message in label
 * @param widget widget
 * @param message char*
 * @param resize Adapte label geometry with new message
 */
void Label_setMessage(WIDGET *widget, char *message, bool resize);

/**
 * @brief Manage label events
 * @param widget widget
 * @warning Don't use this function
 */
//void Label_event(WIDGET *widget);

/**
 * @brief Update label
 * @param widget widget
 * @warning Don't use this function
 */
//void Label_update(WIDGET *widget);

/**
 * @brief Draw label
 * @param widget widget
 * @warning Don't use this function
 */
void Label_draw(WIDGET *widget);

#endif /* __FIRE_GUI_LABEL_H__ */