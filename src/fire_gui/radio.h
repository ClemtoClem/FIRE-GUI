#ifndef __FIRE_GUI_RADIO_H__
#define __FIRE_GUI_RADIO_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 *                   _ ( )
 *  _ __   ____   __| | _   ___
 * | '__| / _` | / _  || | / _ \ 
 * | |   | (_| || (_| || |( |_| )
 * |_|    \__,_| \___ ||_| \___/
 *                   \|
 * Radio buttons are always used in groups (therefore two radio buttons at least)
 * since their objective is to allow the user to choose one, and only one, option
 * among several possible ones.
 * 
 * Example :
 * void actionRadioButton(WIDGET *widget, void *privdata)
 * {
 *     if (widget != NULL && widget->type == RADIO_WIDGET_TYPE) {
 *         RADIO *radio = widget->private;
 *         if (radio->check) printf("Radio button %d is check\n", *((Uint32 *) privdata));
 *         else printf("No radio button is check\n");
 *     }
 * }
 * void configRadioButton(WIDGET *widget, Sint32 y)
 * {
 *     Widget_setOnPressedFunc(widget, actionRadioButton);
 *     Widget_setXAlignment(widget, LEFT_ALIGNMENT, 20);
 *     Widget_setYAlignment(widget, CENTER_ALIGNMENT, y);
 *     Widget_pressedSound(widget, "click-BUTTON1.ogg");
 *     Widget_releasedSound(widget, "click-BUTTON2.ogg");
 * }
 * void newRadioButton()
 * {
 * SDL_Rect geometry = {0, 0, 24, 24};
 * configRadioButton(Radio_new("PANEL1", "RADIO1", "RADIO3", &geometry, "radio.png", NULL, ptr_uint32(1)), 16);
 * configRadioButton(Radio_new("PANEL1", "RADIO2", "RADIO1", &geometry, "radio.png", NULL, ptr_uint32(2)), 48);
 * configRadioButton(Radio_new("PANEL1", "RADIO3", "RADIO2", &geometry, "radio.png", NULL, ptr_uint32(3)), 80);
 * }
 */

#include "widget.h"

#define RADIO_TILESET_NB_TILES_WIDTH 2
#define RADIO_TILESET_NB_TILES_HEIGHT 2
#define RADIO_NB_TILES_WIDTH 1
#define RADIO_NB_TILES_HEIGHT 1

typedef struct t_RADIO {
    char *afterRadioName;
    WIDGET *afterRadio;
	IMAGE *tileset;
	SDL_Rect tileRect;
	bool check;
} RADIO;

/**
 * @brief Allocation of a radio button widget
 * @param name_parent Parent widget identifier
 * @param id Widget Identifier
 * @param afterRadioName After widget identifier for link
 * @param geometry Radio geometry
 * @param area Radio scroll area
 * @param tilesetName Background tileset name
 * @param userFunc User function
 * @param userdata User data
 * @return Returns a widget
 */
WIDGET *Radio_new(char *name_parent, char *name, char *afterRadioName, SDL_Rect *geometry,
	char *tilesetName, callbackUserFunc userFunc, void *userdata);

/**
 * @brief Free radio button private data
 * @param widget widget
 * @warning Don't use this function
 */
void Radio_free(WIDGET *widget);

void Radio_check(WIDGET *widget);

void Radio_uncheck(WIDGET *widget);

bool Radio_isCheck(WIDGET *widget);

/**
 * @brief Manage radio button events
 * @param widget widget
 * @warning Don't use this function
 */
void Radio_event(WIDGET *widget);

void Uncheck_after(WIDGET *widget, WIDGET *after);

/**
 * @brief Update radio button
 * @param widget widget
 * @warning Don't use this function
 */
void Radio_update(WIDGET *widget);

/**
 * @brief Draw radio button
 * @param widget widget
 * @warning Don't use this function
 */
void Radio_draw(WIDGET *widget);

#endif /* __FIRE_GUI_RADIO_H__ */