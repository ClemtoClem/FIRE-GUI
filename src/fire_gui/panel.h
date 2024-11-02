#ifndef __FIRE_GUI_PANEL_H__
#define __FIRE_GUI_PANEL_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 *                              _
 *  ____    __ _  ____    ___  | |
 * | __ \  / _` ||    \  / _ \ | |
 * | \_\ || (_| ||  |  \(  ___)| |
 * |  __/  \__,_||__|__/ \____\|_|
 * |_|
 */

#include "widget.h"

#define PANEL_TILESET_NB_TILES_WIDTH 6
#define PANEL_TILESET_NB_TILES_HEIGHT 3
#define PANEL_NB_TILES_WIDTH 3
#define PANEL_NB_TILES_HEIGHT 3

#define WHEEL_DISTANCE 5

typedef struct t_PANEL {
	IMAGE *tileset;
	SDL_Rect tileRect;
} PANEL;

/**
 * @brief Allocation of a panel widget
 * @param name_parent Parent widget identifier
 * @param id Widget Identifier
 * @param geometry Panel geometry
 * @param area Panel scroll area
 * @param tilesetName Background tileset name
 * @param userFunc User function
 * @param userdata User data
 * @return Returns a widget
 */
WIDGET *Panel_new(char *name_parent, char *name, SDL_Rect *geometry, SDL_Rect *area, char *tilesetName,
	callbackUserFunc userFunc, void *userdata);

/**
 * @brief Free panel private data
 * @param widget widget
 * @warning Don't use this function
 */
void Panel_free(WIDGET *widget);

/**
 * @brief Manage panel events
 * @param widget widget
 * @warning Don't use this function
 */
void Panel_event(WIDGET *widget);

/**
 * @brief Update panel
 * @param widget widget
 * @warning Don't use this function
 */
//void Panel_update(WIDGET *widget);

/**
 * @brief Draw panel
 * @param widget widget
 * @warning Don't use this function
 */
void Panel_draw(WIDGET *widget);

#endif /* __FIRE_GUI_PANEL_H__ */