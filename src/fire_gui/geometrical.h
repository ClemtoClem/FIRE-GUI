#ifndef __FIRE_GUI_GEOMETRICAL_H__
#define __FIRE_GUI_GEOMETRICAL_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 *   ________                                  __            __                  __    
 *  /  _____/   ____   ____    _____    ____ _/  |_ _______ |__|  ____  _____   |  |   
 * /   \  ___ _/ __ \ /  _ \  /     \ _/ __ \\   __\\_  __ \ __ _/ ___\ \__  \  |  |   
 * \    \_\  \\  ___/(  <_> )|  Y Y  \\  ___/ |  |   |  | \/|  |\  \___  / __ \_|  |__ 
 *  \______  / \___  >\____/ |__|_|  / \___  >|__|   |__|   |__| \___  >(____  /|____/ 
 *         \/      \/              \/      \/                        \/      \/    
 * Geometrical widget :
 * Rectangle
 * Circle
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../core/utils.h"
#include "fire_gui_main.h"

typedef struct t_RECTANGLE_WIDGET {
    Uint32 borderColor, bgColor;
    Uint32 survolBorderColor, survolBgColor;
    Uint32 borderSize;
} RECTANGLE_WIDGET;

WIDGET *RectangleWidget_new(char *name_parent, char *name, SDL_Rect *geometry,
    Uint32 borderColor, Uint32 bgColor, Uint32 survolBorderColor, Uint32 survolBgColor, Uint32 borderSize,
    callbackUserFunc userFunc, void *userdata);

/**
 * @brief Free button private data
 * @param widget widget
 * @warning Don't use this function
 */
void RectangleWidget_free(WIDGET *widget);

/**
 * @brief Manage rectangle events
 * @param widget widget
 * @warning Don't use this function
 */
//void RectangleWidget_event(WIDGET *widget);

/**
 * @brief Update rectangle
 * @param widget widget
 * @warning Don't use this function
 */
//void RectangleWidget_update(WIDGET *widget);

/**
 * @brief Draw rectangle
 * @param widget widget
 * @warning Don't use this function
 */
void RectangleWidget_draw(WIDGET *widget);



typedef struct t_CIRCLE_WIDGET {
    Sint32 radius;
    Uint32 borderColor, bgColor;
    Uint32 survolBorderColor, survolBgColor;
} CIRCLE_WIDGET;

WIDGET *CircleWidget_new(Uint32 idRoot, char *name, CIRCLE *circle,
    Uint32 borderColor, Uint32 bgColor, Uint32 survolBorderColor, Uint32 survolBgColor);

#endif /* __FIRE_GUI_GEOMETRICAL_H__ */