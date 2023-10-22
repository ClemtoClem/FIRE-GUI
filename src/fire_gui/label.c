#include "label.h"
#include "../core/driver.h"
#include "../core/input.h"

WIDGET *Label_new(char *name_parent, char *name, SDL_Rect *geometry,
	char *message, char *fontName, Uint32 sizeFont, Uint32 colorFont,
	callbackUserFunc userFunc, void *userdata)
{
	WIDGET *widget = NULL;
	LABEL *label = calloc(1, sizeof(LABEL));
	if (label != NULL) {
		label->font = (TTF_Font *) Driver_loadFont(fontName, sizeFont);
		if (label->font != NULL) {
			label->message = strdup(message);
			label->colorFont = colorFont;
			label->texture = NULL;
			
			SDL_Color col = hexColorToRGBA(label->colorFont);
			SDL_Surface *surface = TTF_RenderText_Blended(label->font, label->message, col);
			RectSet(&label->surfaceRect, 0, 0, surface->w, surface->h);
			label->texture = Driver_createTextureFromSurface(surface);

			bool allocRect;
			if (geometry == NULL) {
				geometry = (SDL_Rect *) calloc(1, sizeof(SDL_Rect));
				allocRect = true;
			} else {
				allocRect = false;
			}

			if (geometry->w == 0) geometry->w = surface->w;
			if (geometry->h == 0) geometry->h = surface->h;
			
			widget = Widget_new(name_parent, name, LABEL_WIDGET_TYPE, geometry, NULL, userFunc, userdata,
				label, NULL /*&Label_event*/, NULL /*&Label_update*/, &Label_draw, &Label_free);
			Widget_resetParentEvent(widget, false);

			if (allocRect) free(geometry);
			SUCCESS("create label");
		}
	}
	return widget;
}

void Label_free(WIDGET *widget)
{
	if (widget != NULL && widget->type == LABEL_WIDGET_TYPE) {
		LABEL *label = widget->private;
		if (label != NULL) {
			if (label->message != NULL) free(label->message);
			if (label->texture != NULL) SDL_DestroyTexture(label->texture);
			free(label);
			widget->private = NULL;
		}
	}
}

void Label_setMessage(WIDGET *widget, char *message, bool resize)
{
	if (widget != NULL && widget->type == LABEL_WIDGET_TYPE) {
		LABEL *label = widget->private;
		if (label->message != NULL) free(label->message);
		if (label->texture != NULL) SDL_DestroyTexture(label->texture);

		label->message = strdup(message);

		SDL_Color col = hexColorToRGBA(label->colorFont);
		SDL_Surface *surface = TTF_RenderText_Blended(label->font, label->message, col);
		RectSet(&label->surfaceRect, 0, 0, surface->w, surface->h);
		label->texture = Driver_createTextureFromSurface(surface);
		if (resize) {
			widget->geometry.w = surface->w;
			widget->geometry.h = surface->h;
		}
		widget->needUpdate = true;
	}
}

/*
void Label_event(WIDGET *widget)
{
	if (widget != NULL && widget->type == LABEL_WIDGET_TYPE) {
		//LABEL *label = widget->private;
	}
}

void Label_update(WIDGET *widget)
{
	if (widget != NULL && widget->type == LABEL_WIDGET_TYPE) {
		//LABEL *label = widget->private;
	}
}
*/

void Label_draw(WIDGET *widget)
{
	if (widget != NULL && widget->type == LABEL_WIDGET_TYPE) {
		LABEL *label = widget->private;
		if (label->texture != NULL && widget->visible) {
			SDL_Rect dest = {0, 0, widget->geometry.w, widget->geometry.h};
			Driver_copy(label->texture, &dest, &label->surfaceRect);
		}
	}
}
