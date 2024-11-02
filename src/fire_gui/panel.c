#include "panel.h"
#include "../core/driver.h"
#include "../core/input.h"

WIDGET *Panel_new(char *name_parent, char *name, SDL_Rect *geometry, SDL_Rect *area, char *tilesetName,
	callbackUserFunc userFunc, void *userdata)
{
	WIDGET *widget = NULL;
	IMAGE *tileset = Driver_loadImage(tilesetName);
	if (tileset != NULL) {
		PANEL *panel = calloc(1, sizeof(PANEL));
		if (panel != NULL) {
			panel->tileset = tileset;
			panel->tileRect.w = panel->tileset->w / PANEL_TILESET_NB_TILES_WIDTH;
			panel->tileRect.h = panel->tileset->h / PANEL_TILESET_NB_TILES_HEIGHT;

			bool allocRect;
			if (geometry == NULL) {
				geometry = AllocRect();
				allocRect = true;
			} else {
				allocRect = false;
			}

			widget = Widget_new(name_parent, name, PANEL_WIDGET_TYPE, geometry, area, userFunc, userdata,
				panel, &Panel_event, NULL /*&Panel_update*/, &Panel_draw, &Panel_free);

			if (allocRect) free(geometry);
			SUCCESS("create panel");
		}
	} else {
		LOG_ERROR("Panel cannot import tileset");
	}
	return widget;
}

void Panel_free(WIDGET *widget)
{
	if (widget != NULL && widget->type == PANEL_WIDGET_TYPE) {
		PANEL *panel = widget->private;
		if (panel != NULL) {
			free(panel);
			widget->private = NULL;
		}
	}
}


void Panel_event(WIDGET *widget)
{
	if (widget != NULL && widget->type == PANEL_WIDGET_TYPE) {
		Sint32 x, y;
		if (widget->flags.focused && Input_wasWheelEvent(&x, &y)) {
			Widget_moveArea(widget, WHEEL_DISTANCE*x, -WHEEL_DISTANCE*y);
		}
	}
}

/*
void Panel_update(WIDGET *widget)
{
	if (widget != NULL && widget->type == PANEL_WIDGET_TYPE) {
		
	}
}
*/

void Panel_draw(WIDGET *widget)
{
	if (widget != NULL && widget->type == PANEL_WIDGET_TYPE) {
		PANEL *panel = widget->private;
		if (panel->tileset != NULL) {
			Widget_drawTile(panel->tileRect.w * PANEL_NB_TILES_WIDTH * widget->flags.focused, 0,
				panel->tileset, &panel->tileRect, &widget->geometry);
		}
	}
}
