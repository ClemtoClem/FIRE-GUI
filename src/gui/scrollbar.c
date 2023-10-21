#include "scrollbar.h"
#include "panel.h"
#include "../core/driver.h"
#include "../core/input.h"

WIDGET *Scrollbar_new(char *name_parent, char *name, SDL_Rect *geometry,
	char *tilesetName, char *buttonTilesetName, Orientation orientation,
	Sint32 min, Sint32 max, Sint32 step, callbackUserFunc userFunc, void *userdata)
{
	if (min > max) {
		LOG_ERROR("La borne max ne doit pas être inférieur à la borne min");
		return NULL;
	}
	if (step <= 0) {
		LOG_ERROR("Le pas entre chaque graduation doit être strictement positif");
		return NULL;
	}
	WIDGET *widget = NULL;
	IMAGE *tileset = Driver_loadImage(tilesetName);
	IMAGE *buttonTileset = Driver_loadImage(buttonTilesetName);
	if (tileset != NULL && buttonTileset != NULL) {
		SCROLLBAR *scrollbar = calloc(1, sizeof(SCROLLBAR));
		if (scrollbar != NULL) {
			scrollbar->tileset = tileset;
			scrollbar->tileRect.w = tileset->w / SCROLLBAR_TILESET_NB_TILES_WIDTH;
			scrollbar->tileRect.h = tileset->h / SCROLLBAR_TILESET_NB_TILES_HEIGHT;

			scrollbar->buttonTileset = buttonTileset;
			scrollbar->buttonTileRect.w = buttonTileset->w / SCROLLBAR_BUTTON_NB_TILES_WIDTH;
			scrollbar->buttonTileRect.h = buttonTileset->h / SCROLLBAR_BUTTON_NB_TILES_HEIGHT;

			scrollbar->orientation = orientation;
			scrollbar->min = min;
			scrollbar->max = max;
			scrollbar->step = step;
			scrollbar->currentValue = min;

			bool allocRect;
			if (geometry == NULL) {
				geometry = AllocRect();
				allocRect = true;
			} else {
				allocRect = false;
			}

			widget = Widget_new(name_parent, name, SCROLLBAR_WIDGET_TYPE, geometry, NULL, userFunc, userdata,
				scrollbar, NULL /*&Scrollbar_event*/, &Scrollbar_update, &Scrollbar_draw, &Scrollbar_free);

			if (allocRect) free(geometry);

			Scrollbar_update(widget);
			SUCCESS("create scrollbar");
		}
	} else {
		LOG_ERROR("Scrollbar cannot import tileset");
	}
	return widget;
}

void Scrollbar_free(WIDGET *widget)
{
	if (widget != NULL && widget->type == SCROLLBAR_WIDGET_TYPE) {
		free((SCROLLBAR *) widget->private);
	}
}

/*
void Scrollbar_event(WIDGET *widget)
{
	if (widget != NULL && widget->type == SCROLLBAR_WIDGET_TYPE) {

	}
}
*/

void Scrollbar_update(WIDGET *widget)
{
	if (widget != NULL && widget->type == SCROLLBAR_WIDGET_TYPE) {
		SCROLLBAR *scrollbar = widget->private;
		if (widget->needUpdate) {
			Sint32 size = (scrollbar->max - scrollbar->min) / scrollbar->step;
			Sint32 current = (scrollbar->currentValue / scrollbar->step) * size;
		}
	}
}

void Scrollbar_draw(WIDGET *widget)
{
	if (widget != NULL && widget->type == SCROLLBAR_WIDGET_TYPE) {
		SCROLLBAR *scrollbar = widget->private;
		if (scrollbar->tileset != NULL) {
			Widget_drawTile(scrollbar->tileRect.w * SCROLLBAR_NB_TILES_WIDTH * widget->flags.focused, 0,
				scrollbar->tileset, &scrollbar->tileRect, &widget->geometry);
		}
	}
}