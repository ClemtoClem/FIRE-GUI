#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "core/array.h"
#include "core/input.h"
#include "core/driver.h"
#include "core/logger.h"
#include "core/timer.h"
#include "core/colors.h"

#include "fire_gui/fire_gui.h"

#define OFFSET_PRESSED 2

bool running = true;

bool Init()
{
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		LOG_ERROR("Echec de l'initialisation de SDL : %s\n", SDL_GetError());
		return false;
	}
	if (IMG_Init(IMG_INIT_PNG) < 0) {
		LOG_ERROR("Echec de l'initialisation de IMG : %s\n", IMG_GetError());
		return false;
	}
	if (TTF_Init() < 0) {
		LOG_ERROR("Echec de l'initialisation de TTF : %s\n", TTF_GetError());
		return false;
	}
	if (Mix_Init(MIX_INIT_OGG) < 0) {
		LOG_ERROR("Echec de l'initialisation de Mix : %s\n", Mix_GetError());
		return false;
	}
	return true;
}

void Quit()
{
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void actionButton(WIDGET *widget, void *privdata)
{
	if (widget != NULL && widget->type == BUTTON_WIDGET_TYPE) {
		uint32_t id = *((int *) privdata);
		if (widget->flags.gain) DEBUG("La souris vient de passer sur la zone du bouton %d", id);
		if (widget->flags.lose) DEBUG("La souris vient de quitter la zone du bouton %d", id);

		if (widget->flags.pressed) {
			DEBUG("bouton %d press\x82", id);
			if (id == 1) {
				Sprite_start(GUI_getWidget("SPRITE2"));
			}
		}
		
		if (widget->flags.released) DEBUG("bouton %d relach\x82", id);
	}
}

void actionQuit(WIDGET *widget, void *private)
{
	if (widget != NULL && widget->type == BUTTON_WIDGET_TYPE) {
		//uint32_t id = *((int *) privdata);
		running = false;
		
		//if (widget->flags.released)
	}
}

void actionToggle(WIDGET *widget, void *privdata)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {
		uint32_t id = *((int *) privdata);
		if (widget->flags.gain) DEBUG("La souris vient de passer sur la zone du toggle bouton %d", id);
		if (widget->flags.lose) DEBUG("La souris vient de quitter la zone du toggle bouton %d", id);
		if (widget->flags.pressed) {
			DEBUG("toggle bouton %d press\x82", id);

			bool state = Toggle_getState(widget);
			WIDGET *toggle;
			if (strcmp(widget->name, "TOGGLE1") == 0) {
				if (state) Driver_playMusic("club.ogg", -1);
				else Driver_pauseMusic();
			} else if (strcmp(widget->name, "TOGGLE2") == 0) {
				toggle = GUI_getWidget("TOGGLE3");
				Toggle_setState(toggle, state);

				WIDGET *btn = GUI_getWidget("BUTTON1");
				if (state) Widget_enable(btn);
				else Widget_disable(btn);
			} else if (strcmp(widget->name, "TOGGLE3") == 0) {
				toggle = GUI_getWidget("TOGGLE2");
				Toggle_setState(toggle, state);

				WIDGET *btn = GUI_getWidget("BUTTON1");
				if (state) Widget_enable(btn);
				else Widget_disable(btn);
			}
		}
		if (widget->flags.released) DEBUG("toggle bouton %d relach\x82", id);
	}
}

void actionNextPage(WIDGET *widget, void *privdata)
{
	if (widget != NULL && widget->type == BUTTON_WIDGET_TYPE) {
		WIDGET *panel1 = privdata;
		Widget_nextActivePage(panel1);
	}
}

void actionAfterPage(WIDGET *widget, void *privdata)
{
	if (widget != NULL && widget->type == BUTTON_WIDGET_TYPE) {
		WIDGET *panel1 = privdata;
		Widget_afterActivePage(panel1);
	}
}

void actionRadioButton(WIDGET *widget, void *privdata)
{
	if (widget != NULL && widget->type == RADIO_WIDGET_TYPE) {
		RADIO *radio = widget->private;
		char msg[100];
		if (radio->check)
			sprintf(msg, "Radio button %d is check", *((Sint32 *) privdata));
		else
			strcpy(msg, "No radio button is check");

		WIDGET *label = GUI_getWidget("LABEL7");
		if (label != NULL) {
			Label_setMessage(label, msg, true);
		}
	}
}

int main(int argv, char *argc[])
{
	if (argv == 2) {
		if (strcmp(argc[1], "debug") == 0) {
			debugLog(true);
			//openLogFile();
		} else {
			printf("Use \"debug\" argument for display message\n");
		}
	} else {
		debugLog(false);
	}

	if (!Init()) goto EndMain;
	int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
    int desktopHeight = GetSystemMetrics(SM_CYSCREEN);
	if (!Driver_init("GUI Test", desktopWidth, desktopHeight, SDL_ALPHA_TRANSPARENT, SDL_WINDOW_BORDERLESS)) goto EndMain;
	if (!Input_init(false)) goto EndMain;
	if (!GUI_init()) goto EndMain;

	/* -------------- TEST -------------- */
	
	SDL_Rect geometry, area;
	WIDGET *widget;
	
	/* Ajout d'une image en arrière plan */
	//Widget_resizeAlignment(Sprite_new(ROOT_WIDGET_NAME, "SPRITE1", NULL, NULL, "underwater-light-beams.png", NULL, NULL), 100, 0, 100, 0);

	/* Création d'un panel */
	WIDGET *panel_top = Panel_new(ROOT_WIDGET_NAME, "PANEL_TOP", NULL, NULL, "panel_grey.png", NULL, NULL);
	Widget_setBorderSize(panel_top, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_placeAlignment(panel_top, LEFT_ALIGNMENT, 0, true, TOP_ALIGNMENT, 0, true);
	Widget_resize(panel_top, 300, 80);
	Widget_dragable(panel_top, true);

	/* Ajout d'un titre dans le panel */
	RectSet(&geometry, 0, 20, 0, 0);
	widget = Label_new("PANEL_TOP", "LABEL_TITLE1", &geometry, "FIRE - GUI", "Fire_Storm.ttf", 50, ORANGE_JUICE, NULL, NULL);
	Widget_setXAlignment(widget, CENTER_ALIGNMENT, 0, true);
	Widget_resetParentEvent(widget, false);

	/* Création d'un panel avec 3 pages */
	WIDGET *panel1 = Panel_new(ROOT_WIDGET_NAME, "PANEL1", NULL, NULL, "panel_orange.png", NULL, NULL);
	Widget_setBorderSize(panel1, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_placeAlignment(panel1, CENTER_ALIGNMENT, 0, true, CENTER_ALIGNMENT, 0, true);
	Widget_resizeAlignment(panel1, 50, 0, 50, 0);
	Widget_addPage(panel1, 3);
	Widget_setActivePage(panel1, 1);
	Widget_dragable(panel1, true);

	/* Page 0, les widgets ajouter visible sur toutes les pages du panel 1 */
	Widget_setWorkPage(panel1, 0);

	/* Ajout d'un titre dans le panel */
	RectSet(&geometry, 0, 20, 0, 0);
	widget = Label_new("PANEL1", "LABEL_TITLE2", &geometry, "FIRE GRAPHIC UNITY INTERFACE", "Fire_Storm.ttf", 50, WHITE, NULL, NULL);
	Widget_setXAlignment(widget, CENTER_ALIGNMENT, 0, true);

	/* Ajout de deux animations de flamme */
	WIDGET *sprite = Sprite_new("PANEL1", "SPRITE_FIRE1", NULL, NULL, "fire.png", NULL, NULL);
	Sprite_configureAnimation(sprite, true, true, 20, 1, HORIZONTAL, 5, 5, true);
	Widget_placeAlignment(sprite, LEFT_ALIGNMENT, 20, true, TOP_ALIGNMENT, 20, true);
	Widget_resetParentEvent(sprite, false);
	sprite = Sprite_new("PANEL1", "SPRITE_FIRE2", NULL, NULL, "fire.png", NULL, NULL);
	Sprite_configureAnimation(sprite, true, true, 20, 1, HORIZONTAL, 5, 5, true);
	Widget_placeAlignment(sprite, RIGHT_ALIGNMENT, -sprite->geometry.w + 20, true, TOP_ALIGNMENT, 20, true);
	Widget_resetParentEvent(sprite, false);

	/* Ajout d'un bouton avec un sprite dans le panel 1 */
	RectSet(&geometry, 0, 0, 40, 40);
	widget = ButtonWithSprite_new("PANEL1", "BUTTON_CLOSE_WINDOW", &geometry, NULL, "button_red.png", NULL, NULL,
		"cross.png", true, OFFSET_PRESSED, OFFSET_PRESSED, NULL, NULL);
	Widget_setOnReleasedFunc(widget, actionQuit);
	Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");
	Widget_placeAlignment(widget, RIGHT_ALIGNMENT, -28, true, TOP_ALIGNMENT, 6, true);

	/* Ajout d'un sprite animé dans le panel */
	sprite = Sprite_new("PANEL1", "SPRITE2", NULL, NULL, "explosion.png", NULL, NULL);
	Sprite_configureAnimation(sprite, true, true, 9, 1, HORIZONTAL, 6, 0, false);
	Widget_placeAlignment(sprite, RIGHT_ALIGNMENT, -80, true, BOTTOM_ALIGNMENT, -80, true);
	Sprite_setCurrentTile(sprite, 7);

	/* Page 1, les widgets jouter sur cette page seront visible seulement sur la page 1 du panel 1 */
	Widget_setWorkPage(panel1, 1);

	RectSet(&geometry, 8, 0, 0, 0);
	widget = Label_new("PANEL1", "LABEL2", &geometry, "Page 1/3", "arial.ttf", 15, WHITE, NULL, NULL);
	Widget_setYAlignment(widget, BOTTOM_ALIGNMENT, -23, true);
	
	/* Ajout d'un bouton avec un sprite dans le panel 1 */
	RectSet(&geometry, 0, 0, 40, 40);
	widget = ButtonWithSprite_new("PANEL1", "BUTTON_NEXT1", &geometry, NULL, "button_blue.png", NULL, NULL,
		"arrow_right.png", true, OFFSET_PRESSED, OFFSET_PRESSED, NULL, panel1);
	Widget_setOnReleasedFunc(widget, actionNextPage);
	Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");
	Widget_placeAlignment(widget, RIGHT_ALIGNMENT, -28, true, BOTTOM_ALIGNMENT, -28, true);

	/* --- Ajout de trois boutons --- */
	RectSet(&geometry, 20, 0, 100, 40);

	widget = ButtonWithLabel_new("PANEL1", "BUTTON1", &geometry, NULL, "button_green.png",
		"Don't touch", "arial.ttf", 18, WHITE, OFFSET_PRESSED, OFFSET_PRESSED, actionButton, ptr_int32(1));
	Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_setYAlignment(widget, CENTER_ALIGNMENT, 0, true);
	Widget_pressedSound(widget, "explode.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");
	Widget_disable(widget);
	
	widget = Toggle_new("PANEL1", "TOGGLE1", &geometry, NULL, "toggle.png",
		OFFSET_PRESSED, OFFSET_PRESSED, false, actionToggle, ptr_int32(1));
	Widget_setYAlignment(widget, CENTER_ALIGNMENT, 48, true);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");
	Toggle_configLeftLabel(widget, "PLAY", "arial.ttf", 20, WHITE);
	Toggle_configRightLabel(widget, "STOP", "arial.ttf", 20, WHITE);

	widget = Toggle_new("PANEL1", "TOGGLE2", &geometry, NULL, "toggle.png",
		OFFSET_PRESSED, OFFSET_PRESSED, false, actionToggle, ptr_int32(2));
	Widget_setYAlignment(widget, CENTER_ALIGNMENT, 96, true);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");
	Toggle_configLeftLabel(widget, "ON", "arial.ttf", 20, WHITE);
	Toggle_configRightLabel(widget, "OFF", "arial.ttf", 20, WHITE);
	/* --- */

	GUI_update();

	/* Ajout d'un deuxième panel avec une scroll area dans le premier panel */
	RectSet(&geometry, 0, 0, panel1->geometry.w/3, panel1->geometry.h/2);
	RectSet(&area, 0, 0, panel1->geometry.w, 340);
	WIDGET *panel2 = Panel_new("PANEL1", "PANEL2", &geometry, &area, "panel_grey_inner.png", NULL, NULL);
	Widget_setBorderSize(panel2, 4, 4, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_placeAlignment(panel2, CENTER_ALIGNMENT, 0, true, CENTER_ALIGNMENT, 0, true);

	RectSet(&geometry, 0, 0, panel1->geometry.w/3, 20);
	widget = Scrollbar_new("PANEL1", "SCROLLBAR1", &geometry, "panel_grey_inner.png", "button_grey.png", HORIZONTAL, 0, panel2->area.w, 4, NULL, NULL);
	Widget_setBorderSize(widget, 4, 4, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_placeAlignment(widget, CENTER_ALIGNMENT, 0, true, CENTER_ALIGNMENT, panel2->geometry.h/2 + 12, true);

	/* Ajout d'un label dans le panel 2 */
	RectSet(&geometry, 10, 10, 0, 0);
	widget = Label_new("PANEL2", "LABEL3", &geometry, "This is a text. Press button 1 :", "arial.ttf", 20, WHITE, NULL, NULL);
	
	/* Ajout d'un bouton dans le panel 2 */
	RectSet(&geometry, 10, 40, 200, 40);
	widget = ButtonWithLabel_new("PANEL2", "BUTTON2", &geometry, NULL, "button_blue.png",
		"Button 1, Clic me !", "arial.ttf", 20, WHITE,
		OFFSET_PRESSED, OFFSET_PRESSED, actionButton, ptr_int32(2));
	Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");

	/* Ajout d'un bouton dans le panel 2 */
	RectSet(&geometry, 10, 90, 0, 0);
	widget = Label_new("PANEL2", "LABEL4", &geometry, "Press button 2 :", "arial.ttf", 20, WHITE, NULL, NULL);

	/* Ajout d'un bouton dans le panel 2 */
	RectSet(&geometry, 10, 120, 100, 40);
	widget = ButtonWithLabel_new("PANEL2", "BUTTON3", &geometry, NULL, "button_blue.png",
		"Button 2", "arial.ttf", 20, WHITE,
		OFFSET_PRESSED, OFFSET_PRESSED, actionButton, ptr_int32(3));
	Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");

	/* Ajout d'un bouton avec un sprite dans le panel 2 */
	RectSet(&geometry, 112, 120, 40, 40);
	widget = ButtonWithSprite_new("PANEL2", "BUTTON4", &geometry, NULL, "button_orange.png", NULL, NULL,
		"config.png", true, OFFSET_PRESSED, OFFSET_PRESSED, actionButton, ptr_int32(4));
	Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");

	/* Ajout d'un label dans le panel 2 */
	RectSet(&geometry, 10, 170, 0, 0);
	widget = Label_new("PANEL2", "LABEL5", &geometry, "Press button 3 :", "arial.ttf", 20, WHITE, NULL, NULL);

	/* Ajout d'un bouton dans le panel 2 */
	RectSet(&geometry, 10, 200, 100, 40);
	widget = ButtonWithLabel_new("PANEL2", "BUTTON5", &geometry, NULL, "button_blue.png",
		"Button 3", "arial.ttf", 20, WHITE,
		OFFSET_PRESSED, OFFSET_PRESSED, actionButton, ptr_int32(5));
	Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");

	/* Ajout d'un bouton avec un sprite dans le panel 2 */
	RectSet(&geometry, 112, 200, 40, 40);
	widget = ButtonWithSprite_new("PANEL2", "BUTTON6", &geometry, NULL, "button_orange.png", NULL, NULL,
		"sound.png", true, OFFSET_PRESSED, OFFSET_PRESSED, actionButton, ptr_int32(4));
	Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_pressedSound(widget, "win.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");

	/* Ajout d'un label dans le panel 2 */
	RectSet(&geometry, 10, 250, 0, 0);
	widget = Label_new("PANEL2", "LABEL6", &geometry, "Press toggle button :", "arial.ttf", 20, WHITE, NULL, NULL);

	/* Ajout d'un bouton à basculement dans le panel 2 */
	RectSet(&geometry, 10, 280, 100, 40);
	widget = Toggle_new("PANEL2", "TOGGLE3", &geometry, NULL, "toggle.png",
		OFFSET_PRESSED, OFFSET_PRESSED, false, actionToggle, ptr_int32(3));
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");
	Toggle_configLeftLabel(widget, "YES", "arial.ttf", 20, WHITE);
	Toggle_configRightLabel(widget, "NO", "arial.ttf", 20, WHITE);


	/* Page 2, les widgets jouter sur cette page seront visible seulement sur la page 2 du panel 1 */
	Widget_setWorkPage(panel1, 2);

	RectSet(&geometry, 8, 0, 0, 0);
	widget = Label_new("PANEL1", "LABEL2", &geometry, "Page 2/3", "arial.ttf", 15, WHITE, NULL, NULL);
	Widget_setYAlignment(widget, BOTTOM_ALIGNMENT, -23, true);
	
	/* Ajout d'un bouton avec un sprite dans le panel 1 */
	RectSet(&geometry, 0, 0, 40, 40);
	widget = ButtonWithSprite_new("PANEL1", "BUTTON_AFTER2", &geometry, NULL, "button_blue.png", NULL, NULL,
		"arrow_left.png", true, OFFSET_PRESSED, OFFSET_PRESSED, NULL, panel1);
	Widget_setOnReleasedFunc(widget, actionAfterPage);
	Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");
	Widget_placeAlignment(widget, RIGHT_ALIGNMENT, -74, true, BOTTOM_ALIGNMENT, -28, true);
	
	/* Ajout d'un bouton avec un sprite dans le panel 1 */
	RectSet(&geometry, 0, 0, 40, 40);
	widget = ButtonWithSprite_new("PANEL1", "BUTTON_NEXT2", &geometry, NULL, "button_blue.png", NULL, NULL,
		"arrow_right.png", true, OFFSET_PRESSED, OFFSET_PRESSED, NULL, panel1);
	Widget_setOnReleasedFunc(widget, actionNextPage);
	Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");
	Widget_placeAlignment(widget, RIGHT_ALIGNMENT, -28, true, BOTTOM_ALIGNMENT, -28, true);

	/* Ajout et liaison de 4 radios boutons dans le panel 1 */

	RectSet(&geometry, 20, 0, 24, 24);
	widget = Radio_new("PANEL1", "RADIO1", "RADIO6", &geometry, "radio.png", NULL, ptr_int32(1));
	Widget_setOnPressedFunc(widget, actionRadioButton);
	Widget_setYAlignment(widget, CENTER_ALIGNMENT, -80, true);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");

	RectSet(&geometry, 20, 0, 24, 24);
	widget = Radio_new("PANEL1", "RADIO2", "RADIO1", &geometry, "radio.png", NULL, ptr_int32(2));
	Widget_setOnPressedFunc(widget, actionRadioButton);
	Widget_setYAlignment(widget, CENTER_ALIGNMENT, -48, true);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");

	RectSet(&geometry, 20, 0, 24, 24);
	widget = Radio_new("PANEL1", "RADIO3", "RADIO2", &geometry, "radio.png", NULL, ptr_int32(3));
	Widget_setOnPressedFunc(widget, actionRadioButton);
	Widget_setYAlignment(widget, CENTER_ALIGNMENT, -16, true);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");

	RectSet(&geometry, 20, 0, 24, 24);
	widget = Radio_new("PANEL1", "RADIO4", "RADIO3", &geometry, "radio.png", NULL, ptr_int32(4));
	Widget_setOnPressedFunc(widget, actionRadioButton);
	Widget_setYAlignment(widget, CENTER_ALIGNMENT, 16, true);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");

	RectSet(&geometry, 20, 0, 24, 24);
	widget = Radio_new("PANEL1", "RADIO5", "RADIO4", &geometry, "radio.png", NULL, ptr_int32(5));
	Widget_setOnPressedFunc(widget, actionRadioButton);
	Widget_setYAlignment(widget, CENTER_ALIGNMENT, 48, true);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");

	RectSet(&geometry, 20, 0, 24, 24);
	widget = Radio_new("PANEL1", "RADIO6", "RADIO5", &geometry, "radio.png", NULL, ptr_int32(6));
	Widget_setOnPressedFunc(widget, actionRadioButton);
	Widget_setYAlignment(widget, CENTER_ALIGNMENT, 80, true);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");
	
	/* Ajout d'un label dans le panel 2 */
	RectSet(&geometry, 60, 0, 0, 0);
	widget = Label_new("PANEL1", "LABEL7", &geometry, "No radio button is check", "arial.ttf", 20, WHITE, NULL, NULL);
	Widget_setYAlignment(widget, CENTER_ALIGNMENT, 0, true);




	/* Page 3 du panel 1 */
	Widget_setWorkPage(panel1, 3);

	RectSet(&geometry, 8, 0, 0, 0);
	widget = Label_new("PANEL1", "LABEL2", &geometry, "Page 3/3", "arial.ttf", 15, WHITE, NULL, NULL);
	Widget_setYAlignment(widget, BOTTOM_ALIGNMENT, -23, true);
	
	/* Ajout d'un bouton avec un sprite dans le panel 1 */
	RectSet(&geometry, 0, 0, 40, 40);
	widget = ButtonWithSprite_new("PANEL1", "BUTTON_AFTER3", &geometry, NULL, "button_blue.png", NULL, NULL,
		"arrow_left.png", true, OFFSET_PRESSED, OFFSET_PRESSED, NULL, panel1);
	Widget_setOnReleasedFunc(widget, actionAfterPage);
	Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");
	Widget_placeAlignment(widget, RIGHT_ALIGNMENT, -74, true, BOTTOM_ALIGNMENT, -28, true);

	/* Ajout d'un deuxième panel avec une scroll area dans le premier panel */
	RectSet(&geometry, 0, 0, 400, 220);
	RectSet(&area, 0, 0, Driver_getWidth(), Driver_getHeight());
	WIDGET *panel3 = Panel_new("PANEL1", "PANEL3", &geometry, &area, "panel_grey_inner.png", NULL, NULL);
	Widget_setBorderSize(panel3, 4, 4, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_placeAlignment(panel3, CENTER_ALIGNMENT, 0, true, CENTER_ALIGNMENT, 0, true);

	char name[20];
	Uint32 i = 0;

	sprintf(name, "DRAG_BUTTON%u", i);
	RectSet(&geometry, 0, 5 + 45*i, 200, 40);
	widget = Button_new("PANEL3", name, &geometry, NULL, "button_red.png", OFFSET_PRESSED, OFFSET_PRESSED, NULL, NULL);
	Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
	Widget_placeGrid(widget, 0, 0);
	Widget_placeAlignment(widget, CENTER_ALIGNMENT, 0, true, CENTER_ALIGNMENT, 0, true);
	Widget_pressedSound(widget, "click-button1.ogg");
	Widget_releasedSound(widget, "click-button2.ogg");
	Widget_dragable(widget, true);

	RectSet(&geometry, 0, 0, 0, 0);
	Sprite_new(name, "SPRITE4", &geometry, NULL, "arrow_down.png", NULL, NULL);
	widget = Sprite_new(name, "SPRITE5", &geometry, NULL, "arrow_down.png", NULL, NULL);
	Widget_setXAlignment(widget, RIGHT_ALIGNMENT, -90, false);

	for (i++; i <= 10; i++) {
		sprintf(name, "DRAG_BUTTON%u", i);
		RectSet(&geometry, 0, 5 + 45*i, 200, 40);
		widget = ButtonWithLabel_new("PANEL3", name, &geometry, NULL, "button_white.png",
			"Clic me and drag me !", "arial.ttf", 18, BLACK, OFFSET_PRESSED, OFFSET_PRESSED, NULL, NULL);
		Widget_setBorderSize(widget, 0, 0, OFFSET_PRESSED, OFFSET_PRESSED);
		Widget_placeGrid(widget, i, 0);
		Widget_placeAlignment(widget, CENTER_ALIGNMENT, (i%2) ? -60 : 60, true, CENTER_ALIGNMENT, 0, true);
		Widget_pressedSound(widget, "click-button1.ogg");
		Widget_releasedSound(widget, "click-button2.ogg");
		Widget_dragable(widget, true);
	}

	
	/* ---------------------------- */
	
	SUCCESS("INIT");

	/* UPDATE */
	GUI_update();
	while(running) {
		/* EVENT */
		Input_event();
		GUI_event();

		if (Input_quitEvent() || Input_wasKeyPressed(SDLK_ESCAPE)) running = false;
		
		/* UPDATE */
		GUI_update();

		/* DRAW */
		Driver_clear();

		GUI_draw();

		Driver_present();

		SDL_Delay(10);
	}
EndMain:
	GUI_free();
	SUCCESS("GUI IS FREE");
	Input_free();
	Driver_free();
	SUCCESS("Driver IS FREE");

	Quit();

	SUCCESS("QUIT");

	//closeLogFile();
	return 0;
}