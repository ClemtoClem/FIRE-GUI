#include "button.h"
#include "label.h"
#include "sprite.h"
#include "../core/driver.h"
#include "../core/input.h"

WIDGET *Button_new(char *name_parent, char *name, SDL_Rect *geometry, SDL_Rect *area, char *tilesetName,
	Sint32 offsetPressedX, Sint32 offsetPressedY, callbackUserFunc userFunc, void *userdata)
{
	WIDGET *widget = NULL;
	IMAGE *tileset = Driver_loadImage(tilesetName);
	if (tileset != NULL) {
		BUTTON *button = calloc(1, sizeof(BUTTON));
		if (button != NULL) {
			button->tileset = tileset;
			button->tileRect.w = button->tileset->w / BUTTON_TILESET_NB_TILES_WIDTH;
			button->tileRect.h = button->tileset->h / BUTTON_TILESET_NB_TILES_HEIGHT;
			
			button->offsetPressedX = offsetPressedX;
			button->offsetPressedY = offsetPressedY;

			bool allocRect;
			if (geometry == NULL) {
				geometry = AllocRect();
				allocRect = true;
			} else {
				allocRect = false;
			}

			widget = Widget_new(name_parent, name, BUTTON_WIDGET_TYPE, geometry, area, userFunc, userdata,
				button, NULL /*&Button_event*/, &Button_update, &Button_draw, &Button_free);
			
			if (allocRect) free(geometry);
			SUCCESS("create button");
		}
	} else {
		LOG_ERROR("Button cannot import tileset");
	}
	return widget;
}

WIDGET *ButtonWithLabel_new(char *name_parent, char *name, SDL_Rect *geometry, SDL_Rect *area, char *tilesetName,
	char *message, char *fontName, Uint32 sizeFont, Uint32 colorFont, //Uint32 focusedColorFont, Uint32 pressedColorFont,
	Sint32 offsetPressedX, Sint32 offsetPressedY, callbackUserFunc userFunc, void *userdata)
{
	WIDGET *widget = NULL;
	BUTTON *button = calloc(1, sizeof(BUTTON));
	if (button != NULL) {
		if (tilesetName != NULL) {
			button->tileset = Driver_loadImage(tilesetName);
			button->tileRect.w = button->tileset->w / BUTTON_TILESET_NB_TILES_WIDTH;
			button->tileRect.h = button->tileset->h / BUTTON_TILESET_NB_TILES_HEIGHT;
		}
		button->offsetPressedX = offsetPressedX;
		button->offsetPressedY = offsetPressedY;

		bool allocRect;
		if (geometry == NULL) {
			geometry = AllocRect();
			allocRect = true;
		} else {
			allocRect = false;
		}
		
		widget = Widget_new(name_parent, name, BUTTON_WIDGET_TYPE, geometry, area, userFunc, userdata,
			button, NULL /*&Button_event*/, &Button_update, &Button_draw, &Button_free);
		
		char *label_name = malloc(strlen(name) + 7);
		sprintf(label_name, "%s:LABEL", name);
		button->label = Label_new(name, label_name, NULL, message, fontName, sizeFont, colorFont, NULL, NULL);
		Widget_resetParentEvent(button->label, false);
		Widget_placeAlignment(button->label, CENTER_ALIGNMENT, 0, true, CENTER_ALIGNMENT, 0, true);
		free(label_name);
		
		if (allocRect) free(geometry);
		SUCCESS("create button with label");
	}
	return widget;
}

WIDGET *ButtonWithSprite_new(char *name_parent, char *name, SDL_Rect *geometry, SDL_Rect *area, char *tilesetName,
	SDL_Rect *spriteGeometry, SDL_Rect *spriteVisibleArea, char *tilesetSpriteName, bool changeSpriteByEvent,
	Sint32 offsetPressedX, Sint32 offsetPressedY, callbackUserFunc userFunc, void *userdata)
{
	WIDGET *widget = NULL;
	BUTTON *button = calloc(1, sizeof(BUTTON));
	if (button != NULL) {
		if (tilesetName != NULL) {
			button->tileset = Driver_loadImage(tilesetName);
			button->tileRect.w = button->tileset->w / BUTTON_TILESET_NB_TILES_WIDTH;
			button->tileRect.h = button->tileset->h / BUTTON_TILESET_NB_TILES_HEIGHT;
		}
		button->offsetPressedX = offsetPressedX;
		button->offsetPressedY = offsetPressedY;
		button->changeSpriteByEvent = changeSpriteByEvent;

		bool allocRect;
		if (geometry == NULL) {
			geometry = AllocRect();
			allocRect = true;
		} else {
			allocRect = false;
		}
		
		widget = Widget_new(name_parent, name, BUTTON_WIDGET_TYPE, geometry, area, userFunc, userdata,
			button, NULL /*&Button_event*/, &Button_update, &Button_draw, &Button_free);
		
		char *sprite_name = malloc(strlen(name) + 8);
		sprintf(sprite_name, "%s:SPRITE", name);
		button->sprite = Sprite_new(name, sprite_name, spriteGeometry, spriteVisibleArea, tilesetSpriteName, NULL, NULL);
		if (changeSpriteByEvent) {
			Sprite_configureAnimation(button->sprite, false, true, 3, 1, HORIZONTAL, 0, 0, false);
		}
		Widget_placeAlignment(button->sprite, CENTER_ALIGNMENT, 0, true, CENTER_ALIGNMENT, 0, true);
		Widget_resetParentEvent(button->sprite, false);
		free(sprite_name);

		if (allocRect) free(geometry);
		SUCCESS("create button with sprite");
	}
	return widget;
}

void Button_free(WIDGET *widget)
{
	if (widget != NULL && widget->type == BUTTON_WIDGET_TYPE) {
		free((BUTTON *) widget->private);
	}
}

/*
void Button_event(WIDGET *widget)
{
	if (widget != NULL && widget->type == BUTTON_WIDGET_TYPE) {

	}
}
*/

void Button_update(WIDGET *widget)
{
	if (widget != NULL && widget->type == BUTTON_WIDGET_TYPE) {
		BUTTON *button = widget->private;
		if (button->sprite != NULL && button->changeSpriteByEvent) {
			if (widget->flags.held) Sprite_setCurrentTile(button->sprite, 2);
			else if (widget->flags.focused) Sprite_setCurrentTile(button->sprite, 1);
			else Sprite_setCurrentTile(button->sprite, 0);
		}
		if (button->offsetPressedX != 0 || button->offsetPressedY != 0) {
			if (widget->flags.held && !button->moveOffset) {
				button->moveOffset = true;

				DATA *page = Array_getFromIndex(widget->allPages, widget->currentActivatePage);
				if (page->type == ARRAY_TYPE) {
					DATA *data = ((ARRAY *) page->buffer)->first;
					while (data != NULL) {
						Widget_move((WIDGET *) data->buffer, button->offsetPressedX, button->offsetPressedY);
						data = data->next;
					}
				}
			}
			if (!widget->flags.held && button->moveOffset) {
				button->moveOffset = false;

				DATA *page = Array_getFromIndex(widget->allPages, widget->currentActivatePage);
				if (page->type == ARRAY_TYPE) {
					DATA *data = ((ARRAY *) page->buffer)->first;
					while (data != NULL) {
						Widget_move((WIDGET *) data->buffer, -button->offsetPressedX, -button->offsetPressedY);
						data = data->next;
					}
				}
			}
		}
	}
}

void Button_draw(WIDGET *widget)
{
	if (widget != NULL && widget->type == BUTTON_WIDGET_TYPE) {
		BUTTON *button = widget->private;
		if (button != NULL) {
			Widget_drawTile(button->tileRect.w * BUTTON_NB_TILES_WIDTH * (widget->flags.focused + widget->flags.held), 0,
				button->tileset, &button->tileRect, &widget->geometry);
		}
	}
}
