#include "sprite.h"
#include "../core/driver.h"
#include "../core/input.h"

WIDGET *Sprite_new(char *name_parent, char *name, SDL_Rect *geometry, SDL_Rect *visibleArea, char *tilesetName,
	callbackUserFunc userFunc, void *userdata)
{
	WIDGET *widget = NULL;
	IMAGE *tileset = Driver_loadImage(tilesetName);
	if (tileset != NULL) {
		SPRITE *sprite = calloc(1, sizeof(SPRITE));
		if (sprite != NULL && tilesetName != NULL) {
			sprite->tileset = tileset;
			if (visibleArea == NULL) {
				RectSet(&sprite->tileRect, 0, 0, tileset->w, tileset->h);
			} else {
				RectCopy(&sprite->tileRect, visibleArea);
				if (visibleArea->w == 0) sprite->tileRect.w = tileset->w;
				if (visibleArea->h == 0) sprite->tileRect.h = tileset->h;
			}

			bool allocRect;
			if (geometry == NULL) {
				geometry = AllocRect();
				allocRect = true;
			} else {
				allocRect = false;
			}

			if (geometry->w == 0) geometry->w = tileset->w;
			if (geometry->h == 0) geometry->h = tileset->h;

			widget = Widget_new(name_parent, name, SPRITE_WIDGET_TYPE, geometry, NULL, userFunc, userdata,
				sprite, NULL /*&Sprite_event*/, &Sprite_update, &Sprite_draw, &Sprite_free);
			Widget_resetParentEvent(widget, false);

			if (allocRect) free(geometry);
			SUCCESS("create sprite");
		}
	} else {
		LOG_ERROR("Sprite cannot create without tileset");
	}
	return widget;
}

void Sprite_free(WIDGET *widget)
{
	if (widget != NULL && widget->type == SPRITE_WIDGET_TYPE) {
		free((SPRITE *) widget->private);
	}
}

void Sprite_setFillSurfaceWithTile(WIDGET *widget, Sint32 width, Sint32 height)
{
	if (widget != NULL && widget->type == SPRITE_WIDGET_TYPE) {
		SPRITE *sprite = widget->private;
		sprite->fillSurfaceWithTile.enable = true;
		if (width == 0) width = widget->geometry.w;
		if (height == 0) height = widget->geometry.h;
		sprite->fillSurfaceWithTile.w = width;
		sprite->fillSurfaceWithTile.h = height;
	}
}

void Sprite_configureAnimation(WIDGET *widget, bool animate, bool resize, Uint16 nbTilesW, Uint16 nbTilesH,
	Orientation orientation, Uint16 frameBetweenTile, Uint16 frameAfterRepeat, bool loop)
{
	if (widget != NULL && widget->type == SPRITE_WIDGET_TYPE) {
		SPRITE *sprite = widget->private;

		sprite->orientation = orientation;
		sprite->frameBetweenTile = frameBetweenTile;
		sprite->frameAfterRepeat = frameAfterRepeat;
		sprite->frame = 0;
		sprite->currentTile = 0;
		sprite->end = false;

		sprite->nbTilesW = nbTilesW;
		sprite->nbTilesH = nbTilesH;
		sprite->tileRect.w = sprite->tileset->w / nbTilesW;
		sprite->tileRect.h = sprite->tileset->h / nbTilesH;
		
		if (resize == true) {
			Widget_resize(widget, sprite->tileRect.w, sprite->tileRect.h);
		}

		sprite->hasBeenConfigured = true;
		sprite->animate = animate;
		sprite->loop = loop;
	}
}

void Sprite_start(WIDGET *widget)
{
	if (widget != NULL && widget->type == SPRITE_WIDGET_TYPE) {
		SPRITE *sprite = widget->private;
		if (sprite->hasBeenConfigured) {
			sprite->animate = true;
			sprite->end = false;
			sprite->currentTile = 0;
		} else {
			WARNING("You have to configure the sprite animation before activating it.");
		}
	}
}

void Sprite_stop(WIDGET *widget)
{
	if (widget != NULL && widget->type == SPRITE_WIDGET_TYPE) {
		((SPRITE *) widget->private)->animate = false;
	}
}

void Sprite_setCurrentTile(WIDGET *widget, Uint16 num)
{
	if (widget != NULL ) {
		SPRITE *sprite = widget->private;
		if (sprite->hasBeenConfigured) {
			if (sprite->currentTile != num) {
				if (sprite->orientation == HORIZONTAL)
					sprite->currentTile = num % sprite->nbTilesW;
				else if (sprite->orientation == VERTICAL)
					sprite->currentTile = num % sprite->nbTilesH;
			}
		} else {
			WARNING("You have to configure the sprite animation before modifing it.");
		}
	}
}

void Sprite_next(WIDGET *widget)
{
	if (widget != NULL ) {
		SPRITE *sprite = widget->private;
		if (sprite->hasBeenConfigured) {
			if (sprite->orientation == HORIZONTAL)
				sprite->currentTile = (sprite->currentTile+1) % sprite->nbTilesW;
			else if (sprite->orientation == VERTICAL)
				sprite->currentTile = (sprite->currentTile+1) % sprite->nbTilesH;
		} else {
			WARNING("You have to configure the sprite animation before modifing it.");
		}
	}
}

void Sprite_after(WIDGET *widget)
{
	if (widget != NULL ) {
		SPRITE *sprite = widget->private;
		if (sprite->hasBeenConfigured) {
			if (sprite->currentTile > 0) sprite->currentTile--;
			else {
				if (sprite->orientation == HORIZONTAL)
					sprite->currentTile = sprite->nbTilesW-1;
				else if (sprite->orientation == VERTICAL)
					sprite->currentTile = sprite->nbTilesH-1;
			}
		} else {
			WARNING("You have to configure the sprite animation before modifing it.");
		}
	}
}

/*
void Sprite_event(WIDGET *widget)
{
	if (widget != NULL && widget->type == SPRITE_WIDGET_TYPE) {

	}
}
*/

void Sprite_update(WIDGET *widget)
{
	if (widget != NULL && widget->type == SPRITE_WIDGET_TYPE) {
		SPRITE *sprite = widget->private;
		if (sprite->animate == true) {
			if (!sprite->end) {
				sprite->frame++;
				if (sprite->frame >= sprite->frameBetweenTile) {
					sprite->currentTile++;
					if (sprite->orientation == HORIZONTAL
						&& sprite->currentTile == sprite->nbTilesW-1)
						sprite->end = true;
					else if (sprite->orientation == VERTICAL
						&& sprite->currentTile == sprite->nbTilesH-1)
						sprite->end = true;
					sprite->frame = 0;
				}
			} else if (sprite->loop) {
				sprite->frame++;
				if (sprite->frame >= sprite->frameAfterRepeat) {
					sprite->end = false;
					sprite->frame = 0;
					sprite->currentTile = 0;
				}
			}
		} else {
			sprite->tileRect.x = 0;
			sprite->tileRect.y = 0;
		}
	}
}

void Sprite_draw(WIDGET *widget)
{
	if (widget != NULL && widget->type == SPRITE_WIDGET_TYPE) {
		SPRITE *sprite = widget->private;
		if (sprite != NULL) {
			if (sprite->hasBeenConfigured) {
				if (sprite->orientation == HORIZONTAL) {
					sprite->tileRect.x = sprite->currentTile * sprite->tileRect.w;
					sprite->tileRect.y = 0;
				} else {
					sprite->tileRect.x = 0;
					sprite->tileRect.y = sprite->currentTile * sprite->tileRect.h;
				}
			}
			if (sprite->fillSurfaceWithTile.enable) {
				SDL_Rect dest = {0, 0, sprite->fillSurfaceWithTile.w, sprite->fillSurfaceWithTile.h};
				Sint32 i, j, i1, j1;
				i1 = widget->geometry.w / dest.w;
				j1 = widget->geometry.h / dest.h;
				for (j = 0; j <= j1; j++) {
					dest.y = j*dest.h;
					for (i = 0; i <= i1; i++) {
						dest.x = i*dest.w;
						Driver_copy(sprite->tileset->texture, &sprite->tileRect, &dest);
					}
				}
			} else {
				SDL_Rect dest = {0, 0, widget->geometry.w, widget->geometry.h};
				Driver_copy(sprite->tileset->texture, &sprite->tileRect, &dest);
			}
		}
	}
}
