#include "toggle.h"
#include "label.h"
#include "sprite.h"
#include "../core/driver.h"
#include "../core/input.h"

WIDGET *Toggle_new(char *name_parent, char *name, SDL_Rect *geometry, SDL_Rect *area, char *tilesetName,
	Sint32 offsetPressedX, Sint32 offsetPressedY, bool state, callbackUserFunc userFunc, void *userdata)
{
	WIDGET *widget = NULL;
	IMAGE *tileset = Driver_loadImage(tilesetName);
	if (tileset != NULL) {
		TOGGLE *toggle = calloc(1, sizeof(TOGGLE));
		if (toggle != NULL) {
			toggle->tileset = tileset;
			toggle->tileRect.w = toggle->tileset->w / TOGGLE_TILESET_NB_TILES_WIDTH;
			toggle->tileRect.h = toggle->tileset->h / TOGGLE_TILESET_NB_TILES_HEIGHT;
			
			toggle->offsetPressedX = offsetPressedX;
			toggle->offsetPressedY = offsetPressedY;
			toggle->state = state;

			bool allocRect;
			if (geometry == NULL) {
				geometry = AllocRect();
				allocRect = true;
			} else {
				allocRect = false;
			}
			
			widget = Widget_new(name_parent, name, TOGGLE_WIDGET_TYPE, geometry, area, userFunc, userdata,
				toggle, &Toggle_event, &Toggle_update, &Toggle_draw, &Toggle_free);

			if (allocRect) free(geometry);
			SUCCESS("create toggle button");
		}
	} else {
		LOG_ERROR("Toggle button cannot import tileset");
	}
	return widget;
}

void Toggle_free(WIDGET *widget)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {
		free((TOGGLE *) widget->private);
	}
}

void Toggle_configLeftLabel(WIDGET *widget, char *message, char *fontName, Uint32 sizeFont, Uint32 colorFont)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {
		TOGGLE *toggle = widget->private;
		SDL_Rect geometry = {0, 0, 0, 0};

		char *label_name = malloc(strlen(widget->name) + 12);
		sprintf(label_name, "%s:LABEL_LEFT", widget->name);

		toggle->left_label = Label_new(widget->name, label_name, &geometry, message, fontName, sizeFont, colorFont, NULL, NULL);
		Widget_placeAlignment(toggle->left_label, CENTER_ALIGNMENT, -widget->geometry.h/2, true, CENTER_ALIGNMENT, 0, true);
		widget->needUpdate = true;
		free(label_name);
	}
}

void Toggle_configRightLabel(WIDGET *widget, char *message, char *fontName, Uint32 sizeFont, Uint32 colorFont)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {
		TOGGLE *toggle = widget->private;
		SDL_Rect geometry = {0, 0, 0, 0};

		char *label_name = malloc(strlen(widget->name) + 13);
		sprintf(label_name, "%s:LABEL_RIGHT", widget->name);

		toggle->right_label = Label_new(widget->name, label_name, &geometry, message, fontName, sizeFont, colorFont, NULL, NULL);
		Widget_placeAlignment(toggle->right_label, CENTER_ALIGNMENT, widget->geometry.h/2, true, CENTER_ALIGNMENT, 0, true);
		widget->needUpdate = true;
		free(label_name);
	}
}

void Toggle_configButtonLabel(WIDGET *widget, char *message, char *fontName, Uint32 sizeFont, Uint32 colorFont)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {

	}
}

void Toggle_configLeftSprite(WIDGET *widget, SDL_Rect *spriteGeometry, char *tilesetSpriteName, bool changeSpriteByEvent)
{
	
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {
		TOGGLE *toggle = widget->private;
		SDL_Rect geometry = {0, 0, spriteGeometry->w, spriteGeometry->h};

		char *sprite_name = malloc(strlen(widget->name) + 13);
		sprintf(sprite_name, "%s:SPRITE_LEFT", widget->name);
		
		toggle->left_sprite = Sprite_new(widget->name, sprite_name, &geometry, NULL, tilesetSpriteName, NULL, NULL);
		Widget_placeAlignment(toggle->left_sprite, CENTER_ALIGNMENT, -widget->geometry.h/2, true, CENTER_ALIGNMENT, 0, true);
		widget->needUpdate = true;
		free(sprite_name);
	}
}

void Toggle_configRightSprite(WIDGET *widget, SDL_Rect *spriteGeometry, char *tilesetSpriteName, bool changeSpriteByEvent)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {
		TOGGLE *toggle = widget->private;
		SDL_Rect geometry = {0, 0, spriteGeometry->w, spriteGeometry->h};

		char *sprite_name = malloc(strlen(widget->name) + 14);
		sprintf(sprite_name, "%s:SPRITE_RIGHT", widget->name);
		
		toggle->right_sprite = Sprite_new(widget->name, sprite_name, &geometry, NULL, tilesetSpriteName, NULL, NULL);
		Widget_placeAlignment(toggle->right_sprite, CENTER_ALIGNMENT, widget->geometry.h/2, true, CENTER_ALIGNMENT, 0, true);
		widget->needUpdate = true;
		free(sprite_name);
	}
}

void Toggle_configButtonSprite(WIDGET *widget, SDL_Rect *spriteGeometry, char *tilesetSpriteName, bool changeSpriteByEvent)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {

	}
}

void Toggle_setState(WIDGET *widget, bool state)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {
		((TOGGLE *) widget->private)->state = state;
		widget->needUpdate = true;
	}
}

bool Toggle_getState(WIDGET *widget)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {
		return ((TOGGLE *) widget->private)->state;
	}
	return false;
}

void Toggle_changeState(WIDGET *widget)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {
		((TOGGLE *) widget->private)->state ^= 1;
		widget->needUpdate = true;
	}
}

void Toggle_event(WIDGET *widget)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {
		TOGGLE *toggle = widget->private;
		if (widget->flags.pressed) {
			toggle->state = !toggle->state;
			widget->needUpdate = true;
		}
	}
}

void Toggle_update(WIDGET *widget)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {
		TOGGLE *toggle = widget->private;

		if (widget->needUpdate) {
			if (toggle->state) {
				Widget_disable(toggle->right_label);
				Widget_disable(toggle->right_sprite);

				Widget_enable(toggle->left_label);
				Widget_enable(toggle->left_sprite);
			} else {
				Widget_enable(toggle->right_label);
				Widget_enable(toggle->right_sprite);

				Widget_disable(toggle->left_label);
				Widget_disable(toggle->left_sprite);
			}

			/* Si les dimensions du widget ont augmenté, la texture est détruite
				* et est recréée avec les bonnes dimensions */
			if (toggle->texture != NULL) {
				Sint32 w, h;
				SDL_QueryTexture(toggle->texture, NULL, NULL, &w, &h);
				if (w < widget->geometry.h || h < widget->geometry.h) {
					SDL_DestroyTexture(toggle->texture);
					toggle->texture = NULL;
				}
			}
			
			if (toggle->texture == NULL) {
				toggle->texture = Driver_createTexture(widget->geometry.h, widget->geometry.h);
			}
		}
	}
}

void Toggle_draw(WIDGET *widget)
{
	if (widget != NULL && widget->type == TOGGLE_WIDGET_TYPE) {
		TOGGLE *toggle = widget->private;
		if (toggle != NULL) {
			SDL_Rect src, dest, dest2;
			Sint32 tilesetX, tilesetY, posX, posY, dim;
			
			// on affiche l'arrière plan du bouton
			// on change l'image en fonction de l'état du toggle
			tilesetX = toggle->tileRect.w * TOGGLE_NB_TILES_WIDTH * toggle->state;
			tilesetY = toggle->tileRect.h * TOGGLE_NB_TILES_HEIGHT *  widget->flags.focused;

			src.w = dest2.w = toggle->tileRect.w;
			src.h = dest2.h = toggle->tileRect.h;

			if (widget->geometry.w > toggle->tileRect.w*2) {
				src.x  = tilesetX + toggle->tileRect.w;
				dest.x = toggle->tileRect.w;
				dest.w = widget->geometry.w - toggle->tileRect.w*2;
				dest.h = toggle->tileRect.h;

				/* côté haut */
				src.y  = tilesetY;
				dest.y = 0;
				Driver_copy(toggle->tileset->texture, &src, &dest);
					
				/* côté bas */
				src.y  = tilesetY + toggle->tileRect.h*2;
				dest.y = widget->geometry.h - toggle->tileRect.h;
				Driver_copy(toggle->tileset->texture, &src, &dest);
			}
			
			if (widget->geometry.h > toggle->tileRect.h*2) {
				src.y  = tilesetY + toggle->tileRect.h;
				dest.y = toggle->tileRect.h;
				dest.w = toggle->tileRect.w;
				dest.h = widget->geometry.h - toggle->tileRect.h*2;

				/* côté gauche */
				src.x  = tilesetX;
				dest.x = 0;
				Driver_copy(toggle->tileset->texture, &src, &dest);
					
				/* côté droite */
				src.x  = tilesetX + toggle->tileRect.w*2;
				dest.x = widget->geometry.w - toggle->tileRect.w;
				Driver_copy(toggle->tileset->texture, &src, &dest);
			}
			
			if ((widget->geometry.w > toggle->tileRect.w*2) && (widget->geometry.h > toggle->tileRect.h*2)) {
				/* centre */
				src.x  = tilesetX + toggle->tileRect.w;
				src.y  = tilesetY + toggle->tileRect.h;
				dest.x = toggle->tileRect.w;
				dest.y = toggle->tileRect.h;
				dest.w = widget->geometry.w - toggle->tileRect.w*2;
				dest.h = widget->geometry.h - toggle->tileRect.h*2;
				Driver_copy(toggle->tileset->texture, &src, &dest);
			}

			/* coin haut gauche */
			src.x = tilesetX;
			src.y = tilesetY;
			toggle->tileRect.x = 0;
			toggle->tileRect.y = 0;
			Driver_copy(toggle->tileset->texture, &src, &toggle->tileRect);

			/* coin haut droit */
			src.x = tilesetX + toggle->tileRect.w*2;
			src.y = tilesetY;
			toggle->tileRect.x = widget->geometry.w - toggle->tileRect.w;
			toggle->tileRect.y = 0;
			Driver_copy(toggle->tileset->texture, &src, &toggle->tileRect);
			
			/* coin bas gauche */
			src.x = tilesetX;
			src.y = tilesetY + toggle->tileRect.h*2;
			toggle->tileRect.x = 0;
			toggle->tileRect.y = widget->geometry.h - toggle->tileRect.h;
			Driver_copy(toggle->tileset->texture, &src, &toggle->tileRect);
			
			/* coin bas droit */
			src.x = tilesetX + toggle->tileRect.w*2;
			src.y = tilesetY + toggle->tileRect.h*2;
			toggle->tileRect.x = widget->geometry.w - toggle->tileRect.w;
			toggle->tileRect.y = widget->geometry.h - toggle->tileRect.h;
			Driver_copy(toggle->tileset->texture, &src, &toggle->tileRect);


			// on affiche l'interieur du bouton
			Driver_setTarget(toggle->texture);
			tilesetX = toggle->tileRect.w * TOGGLE_NB_TILES_WIDTH * 2;
			posX = (toggle->state) ? widget->geometry.w - widget->geometry.h + widget->borderSize.top + toggle->offsetPressedX*2 : toggle->offsetPressedX*2 ;
			posY = widget->borderSize.top + toggle->offsetPressedY*2;
			dim = widget->geometry.h - widget->borderSize.top - widget->borderSize.bottom - toggle->offsetPressedX*2-1;
			
			if (dim > toggle->tileRect.w*2) {
				src.x  = tilesetX + toggle->tileRect.w;
				dest.x = toggle->tileRect.w;
				dest.w = dim - toggle->tileRect.w*2;
				dest.h = toggle->tileRect.h;

				/* côté haut */
				src.y  = tilesetY;
				dest.y = 0;
				Driver_copy(toggle->tileset->texture, &src, &dest);
				
				/* côté bas */
				src.y  = tilesetY + toggle->tileRect.h*2;
				dest.y = dim - toggle->tileRect.h;
				Driver_copy(toggle->tileset->texture, &src, &dest);
			}
			
			if (dim > toggle->tileRect.h*2) {
				src.y  = tilesetY + toggle->tileRect.h;
				dest.y = toggle->tileRect.h;
				dest.w = toggle->tileRect.w;
				dest.h = dim - toggle->tileRect.h*2;

				/* côté gauche */
				src.x  = tilesetX;
				dest.x = 0;
				Driver_copy(toggle->tileset->texture, &src, &dest);
					
				/* côté droite */
				src.x  = tilesetX + toggle->tileRect.w*2;
				dest.x = dim - toggle->tileRect.w;
				Driver_copy(toggle->tileset->texture, &src, &dest);
			}
			
			if ((dim > toggle->tileRect.w*2) && (dim > toggle->tileRect.h*2)) {
				/* centre */
				src.x  = tilesetX + toggle->tileRect.w;
				src.y  = tilesetY + toggle->tileRect.h;
				dest.x = toggle->tileRect.w;
				dest.y = toggle->tileRect.h;
				dest.w = dim - toggle->tileRect.w*2;
				dest.h = dim - toggle->tileRect.h*2;
				Driver_copy(toggle->tileset->texture, &src, &dest);
			}

			/* coin haut gauche */
			src.x = tilesetX;
			src.y = tilesetY;
			toggle->tileRect.x = 0;
			toggle->tileRect.y = 0;
			Driver_copy(toggle->tileset->texture, &src, &toggle->tileRect);

			/* coin haut droit */
			src.x = tilesetX + toggle->tileRect.w*2;
			src.y = tilesetY;
			toggle->tileRect.x = dim - toggle->tileRect.w;
			toggle->tileRect.y = 0;
			Driver_copy(toggle->tileset->texture, &src, &toggle->tileRect);
			
			/* coin bas gauche */
			src.x = tilesetX;
			src.y = tilesetY + toggle->tileRect.h*2;
			toggle->tileRect.x = 0;
			toggle->tileRect.y = dim - toggle->tileRect.h;
			Driver_copy(toggle->tileset->texture, &src, &toggle->tileRect);
			
			/* coin bas droit */
			src.x = tilesetX + toggle->tileRect.w*2;
			src.y = tilesetY + toggle->tileRect.h*2;
			toggle->tileRect.x = dim - toggle->tileRect.w;
			toggle->tileRect.y = dim - toggle->tileRect.h;
			Driver_copy(toggle->tileset->texture, &src, &toggle->tileRect);
			
			Driver_setTarget(widget->texture);
			RectSet(&src, 0, 0, dim, dim);
			RectSet(&dest, posX, posY, dim, dim);
			Driver_copy(toggle->texture, &src, &dest);
		}
	}
}
