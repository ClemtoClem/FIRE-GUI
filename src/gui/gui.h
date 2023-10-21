#ifndef __GUI_H__
#define __GUI_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 *  _____       _    _       _____
 * /  ___\     | |  | |     |_   _|
 * | | ___     | |  | |       | | 
 * | |__| | _  | |__| |  _   _| |_   _
 * \_____/ |_| |______| |_| |_____| |_|
 * Graphical Unit Interface for SDL2
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "widget.h"

#define ROOT_WIDGET_NAME "ROOT"

typedef struct t_GUI {
	Uint32 idWidgetMax;
	ARRAY *allWidgets;
	WIDGET *root;
} GUI;

extern GUI *gui;

bool GUI_init();
void GUI_free();

void GUI_disable();
void GUI_enable();
bool GUI_isEnable();

Uint32 GUI_incrId();

WIDGET *GUI_getRoot();
WIDGET *GUI_getWidget(char *name);
bool GUI_widgetIsHeld(char *name);
bool GUI_widgetIsReleased(char *name);
bool GUI_widgetIsPressed(char *name);
bool GUI_widgetIsFocused(char *name);

void GUI_event();
void GUI_update();
void GUI_draw();

#endif /* __GUI_H__ */