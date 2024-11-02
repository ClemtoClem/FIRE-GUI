#include "radio.h"
#include "fire_gui_main.h"
#include "../core/driver.h"
#include "../core/input.h"

void Uncheck_after(WIDGET *widget, WIDGET *after);

WIDGET *Radio_new(char *name_parent, char *name, char *afterRadioName, SDL_Rect *geometry,
	char *tilesetName, callbackUserFunc userFunc, void *userdata)
{
	WIDGET *widget = NULL;
	RADIO *radio = calloc(1, sizeof(RADIO));
	if (radio != NULL) {
        if (afterRadioName != NULL) radio->afterRadioName = strdup(afterRadioName);
		if (tilesetName != NULL) {
			radio->tileset = Driver_loadImage(tilesetName);
			radio->tileRect.w = radio->tileset->w / RADIO_TILESET_NB_TILES_WIDTH;
			radio->tileRect.h = radio->tileset->h / RADIO_TILESET_NB_TILES_HEIGHT;
		}
		
		bool allocRect;
		if (geometry == NULL) {
			geometry = AllocRect();
			allocRect = true;
		} else {
			allocRect = false;
		}
		
		widget = Widget_new(name_parent, name, RADIO_WIDGET_TYPE, geometry, NULL, userFunc, userdata,
			radio, &Radio_event, &Radio_update, &Radio_draw, &Radio_free);

		if (allocRect) free(geometry);
		SUCCESS("create radio button");
	}
	return widget;
}

void Radio_free(WIDGET *widget)
{
	if (widget != NULL && widget->type == RADIO_WIDGET_TYPE) {
		free((RADIO *) widget->private);
	}
}

void Radio_check(WIDGET *widget)
{
    if (widget != NULL && widget->type == RADIO_WIDGET_TYPE) {
		((RADIO *) widget->private)->check = true;
        widget->needUpdate = true;
	}
}

void Radio_uncheck(WIDGET *widget)
{
    if (widget != NULL && widget->type == RADIO_WIDGET_TYPE) {
		((RADIO *) widget->private)->check = false;
        widget->needUpdate = true;
	}
}

bool Radio_isCheck(WIDGET *widget)
{
    if (widget != NULL && widget->type == RADIO_WIDGET_TYPE) {
		return ((RADIO *) widget->private)->check;
	}
	return false;
}

void Uncheck_after(WIDGET *widget, WIDGET *after)
{
    if (widget != NULL && after != NULL && widget->type == RADIO_WIDGET_TYPE && after->type == RADIO_WIDGET_TYPE) {
        if (widget != after) {
            Radio_uncheck(after);
            RADIO* radio_after = after->private;
            Uncheck_after(widget, radio_after->afterRadio);
        }
    }
}

void Radio_event(WIDGET *widget)
{
	if (widget != NULL && widget->type == RADIO_WIDGET_TYPE) {
		RADIO *radio = widget->private;
		if (widget->flags.pressed) {
            if (!radio->check && radio->afterRadio != NULL) {
                Uncheck_after(widget, radio->afterRadio);
            }
            radio->check = !radio->check;
        }
	}
}

void Radio_update(WIDGET *widget)
{
	if (widget != NULL && widget->type == RADIO_WIDGET_TYPE) {
		RADIO *radio = widget->private;
        if (radio->afterRadioName != NULL && radio->afterRadio == NULL) {
            radio->afterRadio = (WIDGET *) GUI_getWidget(radio->afterRadioName);
        }
	}
}

void Radio_draw(WIDGET *widget)
{
	if (widget != NULL && widget->type == RADIO_WIDGET_TYPE) {
		RADIO *radio = widget->private;
		if (radio != NULL) {
			SDL_Rect src, dest;
			Sint32 tilesetX, tilesetY;

			/* on change l'image en fonction de l'état du bouton */
			tilesetX = radio->tileRect.w * RADIO_NB_TILES_WIDTH * radio->check;
			tilesetY = radio->tileRect.h * RADIO_NB_TILES_HEIGHT * widget->flags.focused;

			src.w = radio->tileRect.w;
			src.h = radio->tileRect.h;
			src.x = tilesetX;
			src.y = tilesetY;
			dest.x = 0;
			dest.y = 0;
            dest.w = widget->geometry.w;
            dest.h = widget->geometry.h;
			Driver_copy(radio->tileset->texture, &src, &dest);
		}
	}
}
