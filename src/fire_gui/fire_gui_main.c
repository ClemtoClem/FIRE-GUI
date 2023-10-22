#include "fire_gui_main.h"
#include "../core/driver.h"
#include "../core/input.h"

GUI *gui = NULL;

bool GUI_init()
{
	bool success = false;
	if (gui == NULL) {
		gui = (GUI *) calloc(1, sizeof(GUI));
		if (gui != NULL) {
			SDL_Rect win = {0, 0, Driver_getWidth(), Driver_getHeight()};
			gui->root = Widget_new(NULL, ROOT_WIDGET_NAME, ROOT_WIDGET_TYPE, &win, NULL,
				NULL, NULL, NULL, NULL, NULL, NULL, NULL);

			success = true;
		}
	}
	return success;
}

void GUI_free()
{
	if (gui != NULL) {
		Widget_free(gui->root);
		free(gui);
	}
}


void GUI_enable()
{
	if (gui != NULL && !gui->root->enable) {
		gui->root->enable = true;
	}
}

void GUI_disable()
{
	if (gui != NULL && gui->root->enable) {
		gui->root->enable = false;
	}
}

bool GUI_isEnable()
{
	if (gui != NULL) {
		return gui->root->enable;
	}
	return false;
}

Uint32 GUI_incrId()
{
	if (gui != NULL) {
		gui->idWidgetMax++;
		return gui->idWidgetMax;
	}
	return 0;
}

WIDGET *GUI_getRoot()
{
	if (gui != NULL) {
		return gui->root;
	} else {
		return NULL;
	}
}

WIDGET *GUI_getWidget(char *name)
{
	WIDGET *widget = NULL;
	if (gui != NULL) {
		if (strcmp(name, ROOT_WIDGET_NAME) == 0) widget = GUI_getRoot();
		else widget = Widget_findChild(gui->root, name);
	}
	return widget;
}

bool GUI_widgetIsFocused(char *name)
{
	bool focused = false;
	if (gui != NULL) {
		focused = Widget_onFocused(Widget_findChild(gui->root, name));
	}
	return focused;
}

bool GUI_widgetIsPressed(char *name)
{
	bool pressed = false;
	if (gui != NULL) {
		pressed = Widget_onPressed(Widget_findChild(gui->root, name));
	}
	return pressed;
}

bool GUI_widgetIsReleased(char *name)
{
	bool released = false;
	if (gui != NULL) {
		released = Widget_onReleased(Widget_findChild(gui->root, name));
	}
	return released;
}

bool GUI_widgetIsHeld(char *name)
{
	bool held = false;
	if (gui != NULL) {
		held = Widget_onHeld(Widget_findChild(gui->root, name));
	}
	return held;
}



void GUI_event()
{
	if (gui != NULL && gui->root->enable) {
		Widget_event(gui->root);
	}
}

void GUI_update()
{
	if (gui != NULL && gui->root->enable) {
		Widget_update(gui->root);
	}
}

void GUI_draw()
{
	if (gui != NULL && gui->root->enable) {
		Widget_draw(gui->root);
		Driver_setTarget(NULL);
	}
}
