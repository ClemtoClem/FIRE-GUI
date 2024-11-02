#ifndef __FIRE_GUI_WIDGET_H__
#define __FIRE_GUI_WIDGET_H__

/*
 ********************************************
 *              SOURCE CODE                 *
 ********************************************
 *               ( )     _                _
 * __    __    __ _   __| |  ___    ___  | |__
 * \ \  /  \  / /| | / _  | / _ \  / _ \ |  __)
 *  \ \/ /\ \/ / | || (_| || |_| |(  ___)| |_/\
 *   \__/  \__/  |_| \___ || ___/  \____\\____/
 *                       \|/ ___ \
 *                         \_____/
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "../core/array.h"
#include "../core/utils.h"
#include "../core/image.h"

typedef struct t_WIDGET WIDGET;

typedef void (*callbackFunc) (WIDGET *widget);
typedef void (*callbackUserFunc) (WIDGET *widget, void *userdata);
typedef void (*callbackUserDragFunc) (WIDGET *widget, Sint32 moveX, Sint32 moveY, void *userdata);

#define LEFT_ALIGNMENT 0.0
#define TOP_ALIGNMENT 0.0
#define QUARTER_ALIGNMENT 25.0
#define CENTER_ALIGNMENT 50.0
#define THIRD_QUARTER_ALIGNMENT 75.0
#define RIGHT_ALIGNMENT 100.0
#define BOTTOM_ALIGNMENT 100.0
#define DOUBLE_ALIGNMENT 200.0

typedef enum {
	HORIZONTAL,
	VERTICAL,
} Orientation;

struct BorderSize {
	Sint32 left, top, right, bottom;
};

struct Flags {
	bool gain, lose, focused;
	bool pressed, released, held;
};

struct UserFunc {
	callbackUserFunc user;
	callbackUserFunc gainFocused;
	callbackUserFunc loseFocused;
	callbackUserFunc onFocused;
	callbackUserFunc onPressed;
	callbackUserFunc onReleased;
	callbackUserFunc onHeld;
	callbackUserDragFunc onDrag;
};

struct t_WIDGET {
	TypeData type;
	Uint32 id;
	char *name;

	SDL_Point position;
	SDL_Rect geometry;
	SDL_Rect geometryWithoutBorder;
	SDL_Rect area;
	SDL_Rect absolute;
	SDL_Rect composite;
	SDL_Rect clipRect;
	SDL_Texture *texture;
	Uint32 bgColor;

	Sint32 row, col;
	Sint32 nb_row, nb_col;

	SDL_FRect align_pourcent;
	SDL_Rect align_offset;
	bool x_centered, y_centered;
	bool resetParentEvent;

	struct BorderSize borderSize;

	WIDGET *parent;

	Uint32 nbPages, currentWorkPages, currentActivatePage;
	ARRAY *allPages; /* allPages est une liste de listes de widgets */

	void *userdata;
	void *private;

	struct UserFunc userFunc;

	callbackFunc eventFunc;
	callbackFunc updateFunc;
	callbackFunc drawFunc;
	callbackFunc freeFunc;

	char *pressedSoundName;
	char *releasedSoundName;

	SDL_Point dragLastPos;
	Sint32 dragMoveX, dragMoveY;

	bool visible, enable, usedArea;
	struct Flags flags;
	bool dragable, dragEnable, drag;
	bool needUpdate;
};

WIDGET *Widget_alloc();
WIDGET *Widget_new(char *name_parent, char *name, TypeData type, SDL_Rect *geometry, SDL_Rect *area,
	callbackUserFunc userFunc, void *userdata,
	void *private, callbackFunc eventFunc, callbackFunc updateFunc, callbackFunc drawFunc, callbackFunc freeFunc);
void Widget_free(WIDGET *widget);
void Widget_Data_free(void *buffer, TypeData type);

/** Remove widget to parent */
void Widget_remove(WIDGET *parent, WIDGET *widget);
/** Add widget to parent */
void Widget_add(WIDGET *parent, WIDGET *widget);
void Widget_placeAxis(WIDGET *widget, Sint32 x, Sint32 y);
void Widget_placeGrid(WIDGET *widget, Sint32 row, Sint32 col);
void Widget_placeAlignment(WIDGET *widget, float x_pourcent, Sint32 x_offset, bool x_centered, float y_pourcent, Sint32 y_offset, bool y_centered);
void Widget_setXAlignment(WIDGET *widget, float x_pourcent, Sint32 x_offset, bool x_centered);
void Widget_setYAlignment(WIDGET *widget, float y_pourcent, Sint32 y_offset, bool y_centered);
void Widget_move(WIDGET *widget, Sint32 dx, Sint32 dy);
void Widget_resize(WIDGET *widget, Sint32 w, Sint32 h);
void Widget_resizeAlignment(WIDGET *widget, float w_pourcent, Sint32 w_offset, float h_pourcent, Sint32 h_offset);
void Widget_setWidthAlignment(WIDGET *widget, float w_pourcent, Sint32 w_offset);
void Widget_setHeightAlignment(WIDGET *widget, float h_pourcent, Sint32 h_offset);

void Widget_areaEnable(WIDGET *widget);
void Widget_areaDisable(WIDGET *widget);
bool Widget_areaIsEnable(WIDGET *widget);
/* Configure move area */
void Widget_setArea(WIDGET *widget, SDL_Rect *area);
/* Move area, return true if overflow */
bool Widget_moveArea(WIDGET *widget, Sint32 dx, Sint32 dy);
/* Configure border size */
void Widget_setBorderSize(WIDGET *widget, Sint32 left, Sint32 top, Sint32 right, Sint32 bottom);

void Widget_enable(WIDGET *widget);
void Widget_disable(WIDGET *widget);
bool Widget_isEnable(WIDGET *widget);
bool Widget_isVisible(WIDGET *widget);

void Widget_resetParentEvent(WIDGET *widget, bool active);

/**
 * @brief Ajouter des pages aux widget
 * @param widget widget
 * @param nbPages nombre de pages à ajouter
 */
void Widget_addPage(WIDGET *widget, Uint32 nbPages);

/**
 * @brief Chosir une page de travail dans lequel ont pourra ajouter des widget enfants,
 * 0 si on veut que les widgets enfants qui seront ajoutés soient présent sur toutes les pages
 * @param widget widget
 * @param page id de la page
 */
void Widget_setWorkPage(WIDGET *widget, Uint32 page);

/**
 * @brief Retourner l'id de la pages de travil actuelle
 * @param widget widget
 */
Uint32 Widget_getWorkPage(WIDGET *widget);

/**
 * @brief Aller à la page de travail suivante
 * @param widget widget
 */
void Widget_nextWorkPage(WIDGET *widget);

/**
 * @brief Aller à la page de travail précédente
 * @param widget widget
 */
void Widget_afterWorkPage(WIDGET *widget);

/**
 * @brief Chosir une page active qui sera affichée à l'écran,
 * 0 si on veut qu'aucune page ne soient visible
 * @param widget widget
 * @param page id de la page
 */
void Widget_setActivePage(WIDGET *widget, Uint32 page);

/**
 * @brief Retourner l'id de la pages visible actuelle
 * @param widget widget
 */
Uint32 Widget_getActivePage(WIDGET *widget);

/**
 * @brief Afficher la page suivante
 * @param widget widget
 */
void Widget_nextActivePage(WIDGET *widget);

/**
 * @brief Afficher la page précédente
 * @param widget widget
 */
void Widget_afterActivePage(WIDGET *widget);

/* Choisir le son qui sera joué losrque le widget est pressé */
void Widget_pressedSound(WIDGET *widget, char *soundName);
/* Choisir le son qui sera joué losrque le widget est relâché */
void Widget_releasedSound(WIDGET *widget, char *soundName);

void Widget_setFunc(WIDGET *widget, callbackUserFunc func);
void Widget_setGainFocusedFunc(WIDGET *widget, callbackUserFunc func);
void Widget_setLoseFocusedFunc(WIDGET *widget, callbackUserFunc func);
void Widget_setOnFocusedFunc(WIDGET *widget, callbackUserFunc func);
void Widget_setOnPressedFunc(WIDGET *widget, callbackUserFunc func);
void Widget_setOnReleasedFunc(WIDGET *widget, callbackUserFunc func);
void Widget_setOnHeldFunc(WIDGET *widget, callbackUserFunc func);
void Widget_setOnDragFunc(WIDGET *widget, callbackUserDragFunc func);

void Widget_dragable(WIDGET *widget, bool dragable);
bool Widget_isDragable(WIDGET *widget);

bool Widget_gainFocused(WIDGET *widget);
bool Widget_loseFocused(WIDGET *widget);
bool Widget_onFocused(WIDGET *widget);
bool Widget_onPressed(WIDGET *widget);
bool Widget_onReleased(WIDGET *widget);
bool Widget_onHeld(WIDGET *widget);

WIDGET *Widget_findChild(WIDGET *widget, char *name);

void Widget_callbackEvent(WIDGET *widget, bool callback);
void Widget_eventChildren(ARRAY *children);
void Widget_resetEvent(WIDGET *widget);
void Widget_event(WIDGET *widget);
void Widget_updateChildren(ARRAY *children, bool needUpdate);
void Widget_update(WIDGET *widget);
void Widget_drawChildren(ARRAY *children);
void Widget_draw(WIDGET *widget);

void Widget_drawTile(Sint32 tilesetX, Sint32 tilesetY, IMAGE *tileset, SDL_Rect *tileRect, SDL_Rect *geometry);

#endif /* __FIRE_GUI_WIDGET_H__ */
