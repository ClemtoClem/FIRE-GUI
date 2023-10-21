#include "widget.h"
#include "gui.h"
#include "../core/logger.h"
#include "../core/driver.h"
#include "../core/input.h"


WIDGET *findChild(ARRAY *children, char *name);
void noVisibleChild(WIDGET *widget);



WIDGET *Widget_alloc()
{
	WIDGET *widget = calloc(1, sizeof(WIDGET));
	if (widget == NULL) LOG_ERROR("Echec d'alloaction de la structure Widget");
	return widget;
}

WIDGET *Widget_new(char *name_parent, char *name, TypeData type, SDL_Rect *geometry, SDL_Rect *area,
    callbackUserFunc userFunc, void *userdata,
    void *private, callbackFunc eventFunc, callbackFunc updateFunc, callbackFunc drawFunc, callbackFunc freeFunc)
{
	WIDGET *widget = Widget_alloc();
	if (widget != NULL) {
		widget->id = GUI_incrId();
		widget->type = type;

		if (name != NULL) {
			widget->name = (char *) strdup(name);
		} else {
			widget->name = (char *) malloc(sizeof(Uint32)+1);
			sprintf(widget->name, "%u", widget->id);
		}

		if (name_parent != NULL) widget->parent = (WIDGET *) GUI_getWidget(name_parent);
		
		widget->allPages = Array_new(NO_LIMITED);
		widget->currentWorkPages = 0;
		Array_append(widget->allPages, Data_new(0, NULL, ARRAY_TYPE, Array_new(NO_LIMITED), NULL));

		RectCopy(&widget->geometry, geometry);
		widget->position.x = geometry->x;
		widget->position.y = geometry->y;
		Widget_setArea(widget, area);

		widget->nb_row = widget->nb_col = 1;
		widget->row = widget->col = 0;

		widget->userdata = userdata;
		widget->private = private;

		widget->userFunc.user = userFunc;
		widget->eventFunc = eventFunc;
		widget->updateFunc = updateFunc;
		widget->drawFunc = drawFunc;
		widget->freeFunc = freeFunc;

		widget->enable = true;
		widget->visible = true;
		widget->resetParentEvent = true;
		widget->needUpdate = true;

		widget->bgColor = 0x000000FF;

		Widget_update(widget);

		if (widget->parent != NULL) {
			Widget_add(widget->parent, widget);
		}
	}
	return widget;
}

void Widget_free(WIDGET *widget)
{
	if (widget != NULL) {
		Array_free(widget->allPages, true);
		free(widget->name);
		free(widget);
	}
}

void Widget_Data_free(void *buffer, TypeData type)
{
	if (buffer != NULL && type >= ROOT_WIDGET_TYPE && type <= COMBOBOX_WIDGET_TYPE) {
		Widget_free((WIDGET *) buffer);
	}
}

void Widget_remove(WIDGET *parent, WIDGET *widget)
{
	if (parent != NULL && widget != NULL) {
		Array_delFromId(parent->allPages, widget->id, true);
	}
}

void Widget_add(WIDGET *parent, WIDGET *widget)
{
	if (widget != NULL) {
		if (parent != widget->parent) {
			Widget_remove(widget->parent, widget);
			if (parent == NULL) parent = GUI_getRoot();
		}

		DATA *data = Array_getFromIndex(parent->allPages, parent->currentWorkPages);
		if (data->type == ARRAY_TYPE) {
			ARRAY *children = data->buffer;
			Array_append(children, Data_new(widget->id, widget->name, widget->type, widget, Widget_Data_free));
		}
	}
}

void Widget_placeAxis(WIDGET *widget, Sint32 x, Sint32 y)
{
	if (widget != NULL && widget->type != ROOT_WIDGET_TYPE) {
		if (widget->align_pourcent.x != 0) widget->align_pourcent.x = 0.0;
		widget->position.x = x;
		if (widget->align_pourcent.y != 0) widget->align_pourcent.y = 0.0;
		widget->position.y = y;

		widget->needUpdate = true;
	}
}

void Widget_placeGrid(WIDGET *widget, Sint32 row, Sint32 col)
{
	if (widget != NULL && widget->type != ROOT_WIDGET_TYPE) {
		if (widget->parent != NULL) {
			widget->row = row;
			widget->col = col;
			if (widget->parent->nb_row < row+1) widget->parent->nb_row = row+1;
			if (widget->parent->nb_col < col+1) widget->parent->nb_col = col+1;
		}

		widget->needUpdate = true;
	}
}

void Widget_placeAlignment(WIDGET *widget, float x_pourcent, Sint32 x_offset, bool x_centered, float y_pourcent, Sint32 y_offset, bool y_centered)
{
	if (widget != NULL && widget->type != ROOT_WIDGET_TYPE && widget->parent != NULL) {
		widget->align_pourcent.x = (x_pourcent < 0)? 0.0 : x_pourcent;
		if (widget->align_pourcent.x == 0) widget->position.x = x_offset;
		else widget->align_offset.x = x_offset;
		widget->x_centered = x_centered;
		
		widget->align_pourcent.y = (y_pourcent < 0)? 0.0 : y_pourcent;
		if (widget->align_pourcent.y == 0) widget->position.y = y_offset;
		else widget->align_offset.y = y_offset;
		widget->y_centered = y_centered;

		widget->needUpdate = true;
	}
}

void Widget_setXAlignment(WIDGET *widget, float x_pourcent, Sint32 x_offset, bool x_centered)
{
	if (widget != NULL && widget->type != ROOT_WIDGET_TYPE && widget->parent != NULL) {
		widget->align_pourcent.x = (x_pourcent < 0)? 0.0 : x_pourcent;
		if (widget->align_pourcent.x == 0) widget->position.x = x_offset;
		else widget->align_offset.x = x_offset;
		widget->x_centered = x_centered;

		widget->needUpdate = true;
	}
}

void Widget_setYAlignment(WIDGET *widget, float y_pourcent, Sint32 y_offset, bool y_centered)
{
	if (widget != NULL && widget->type != ROOT_WIDGET_TYPE && widget->parent != NULL) {
		widget->align_pourcent.y = (y_pourcent < 0)? 0.0 : y_pourcent;
		if (widget->align_pourcent.y == 0) widget->position.y = y_offset;
		else widget->align_offset.y = y_offset;
		widget->y_centered = y_centered;

		widget->needUpdate = true;
	}
}

void Widget_move(WIDGET *widget, Sint32 dx, Sint32 dy)
{
	if (widget != NULL && widget->type != ROOT_WIDGET_TYPE) {
		if (widget->drag == true) {
			if (widget->align_pourcent.x != 0) widget->align_pourcent.x = 0;
			if (widget->align_pourcent.y != 0) widget->align_pourcent.y = 0;
			if (widget->row != 0) widget->row = 0;
			if (widget->col != 0) widget->col = 0;

			DEBUG("move widget (%d, %d)", dx, dy);
			widget->position.x += dx;
			widget->position.y += dy;
			DEBUG("widget position (%d, %d)", widget->position.x, widget->position.y);

		} else {
			if (widget->parent != NULL && widget->align_pourcent.x != 0)
				widget->align_offset.x += dx;
			else
				widget->position.x += dx;
			
			if (widget->parent != NULL && widget->align_pourcent.y != 0)
				widget->align_offset.y += dy;
			else
				widget->position.y += dy;
		}

		widget->needUpdate = true;
	}
}

void Widget_resize(WIDGET *widget, Sint32 w, Sint32 h)
{
	if (widget != NULL && widget->type != ROOT_WIDGET_TYPE) {
		if (widget->align_pourcent.w != 0) widget->align_pourcent.w = 0.0;
		widget->geometry.w = w;
		if (widget->align_pourcent.h != 0) widget->align_pourcent.h = 0.0;
		widget->geometry.h = h;

		widget->needUpdate = true;
	}
}

void Widget_resizeAlignment(WIDGET *widget, float w_pourcent, Sint32 w_offset, float h_pourcent, Sint32 h_offset)
{
	if (widget != NULL && widget->type != ROOT_WIDGET_TYPE && widget->parent != NULL) {
		widget->align_pourcent.w = (w_pourcent < 0)? 0.0 : w_pourcent;
		if (widget->align_pourcent.w == 0) widget->align_offset.w = 0.0;
		else widget->align_offset.w = w_offset;
		widget->align_pourcent.h = (h_pourcent < 0)? 0.0 : h_pourcent;
		if (widget->align_pourcent.h == 0) widget->align_offset.h = 0.0;
		else widget->align_offset.h = h_offset;

		widget->needUpdate = true;
	}
}

void Widget_setWidthAlignment(WIDGET *widget, float w_pourcent, Sint32 w_offset)
{
	if (widget != NULL && widget->type != ROOT_WIDGET_TYPE && widget->parent != NULL) {
		widget->align_pourcent.w = (w_pourcent < 0)? 0.0 : w_pourcent;
		if (widget->align_pourcent.w == 0) widget->align_offset.w = 0.0;
		else widget->align_offset.w = w_offset;

		widget->needUpdate = true;
	}
}

void Widget_setHeightAlignment(WIDGET *widget, float h_pourcent, Sint32 h_offset)
{
	if (widget != NULL && widget->type != ROOT_WIDGET_TYPE && widget->parent != NULL) {
		widget->align_pourcent.h = (h_pourcent < 0)? 0.0 : h_pourcent;
		if (widget->align_pourcent.h == 0) widget->align_offset.h = 0.0;
		else widget->align_offset.h = h_offset;

		widget->needUpdate = true;
	}
}

void Widget_areaEnable(WIDGET *widget)
{
	if (widget != NULL && !widget->usedArea) {
		widget->usedArea = true;
	}
}

void Widget_areaDisable(WIDGET *widget)
{
	if (widget != NULL && widget->usedArea) {
		widget->usedArea = false;
	}
}

bool Widget_areaIsEnable(WIDGET *widget)
{
	if (widget != NULL) {
		return widget->usedArea;
	}
	return false;
}

void Widget_setArea(WIDGET *widget, SDL_Rect *area)
{
	if (widget != NULL) {
		if (area != NULL) {
			if (area->w < widget->geometry.w) { area->w = widget->geometry.w; area->x = 0; }
			if (area->h < widget->geometry.h) { area->h = widget->geometry.h; area->y = 0; }
			RectCopy(&widget->area, area);
			if (widget->geometry.w != area->w && widget->geometry.h != area->h)
				widget->usedArea = true;
		} else {
			RectSet(&widget->area, 0, 0, widget->geometry.w, widget->geometry.h);
			widget->usedArea = false;
		}
		widget->needUpdate = true;
	}
}

bool Widget_moveArea(WIDGET *widget, Sint32 dx, Sint32 dy)
{
	bool overflow = false;
	if (widget != NULL && widget->usedArea) {
		if (dx != 0) {
			Sint32 x = widget->area.x + dx;
			Sint32 maxW = widget->area.w - widget->geometry.w - widget->borderSize.left - widget->borderSize.right;
			if (x < 0) { x = 0; overflow = true; }
			else if (x > maxW) { x = maxW; overflow = true; }
			widget->area.x = x;
		}
		if (dy != 0) {
			Sint32 y = widget->area.y + dy;
			Sint32 maxH = widget->area.h - widget->geometry.h - widget->borderSize.top - widget->borderSize.bottom;
			if (y < 0) { y = 0; overflow = true; }
			else if (y > maxH) { y = maxH; overflow = true; }
			widget->area.y = y;
		}
		widget->needUpdate = true;
	}
	return overflow;
}

void Widget_setBorderSize(WIDGET *widget, Sint32 left, Sint32 top, Sint32 right, Sint32 bottom)
{
	if (widget != NULL && widget->type != ROOT_WIDGET_TYPE) {
		widget->borderSize.left = left;
		widget->borderSize.top = top;
		widget->borderSize.right = right;
		widget->borderSize.bottom = bottom;
		widget->needUpdate = true;
	}
}

void Widget_enable(WIDGET *widget)
{
	if (widget != NULL && !widget->enable) {
		widget->enable = true;
		widget->needUpdate = true;
	}
}

void Widget_disable(WIDGET *widget)
{
	if (widget != NULL && widget->enable) {
		widget->enable = false;
		widget->flags.focused = false;
		widget->flags.lose = false;
		widget->flags.gain = false;
		widget->flags.pressed = false;
		widget->flags.released = false;
		widget->flags.held = false;
	}
}

bool Widget_isEnable(WIDGET *widget)
{
	if (widget != NULL) {
		return widget->enable;
	}
	return false;
}

bool Widget_isVisible(WIDGET *widget)
{
	if (widget != NULL) {
		return widget->visible;
	}
	return false;
}

void Widget_resetParentEvent(WIDGET *widget, bool active)
{
	if (widget != NULL) {
		widget->resetParentEvent = active;
	}
}


void Widget_addPage(WIDGET *widget, Uint32 nbPages)
{
	if (widget != NULL) {
		for (Uint32 i = 0; i < nbPages; i++) {
			widget->nbPages++;
			Array_append(widget->allPages, Data_new(widget->nbPages, NULL, ARRAY_TYPE, Array_new(NO_LIMITED), NULL));
		}
	}
}

void Widget_setWorkPage(WIDGET *widget, Uint32 page)
{
	if (widget != NULL) {
		if (page <= widget->nbPages) widget->currentWorkPages = page;
		widget->needUpdate = true;
	}
}

Uint32 Widget_getWorkPage(WIDGET *widget)
{
	Uint32 page = 0;
	if (widget != NULL) {
		page = widget->currentWorkPages;
	}
	return page;
}

void Widget_nextWorkPage(WIDGET *widget)
{
	if (widget != NULL) {
		if (widget->nbPages > 0) {
			widget->currentWorkPages++;
			if (widget->currentWorkPages > widget->nbPages) widget->currentWorkPages = 1;
			widget->needUpdate = true;
		}
	}
}

void Widget_afterWorkPage(WIDGET *widget)
{
	if (widget != NULL) {
		if (widget->nbPages > 0) {
			widget->currentWorkPages--;
			if (widget->currentWorkPages == 0) widget->currentWorkPages = widget->nbPages;
			widget->needUpdate = true;
		}
	}
}


void noVisibleChild(WIDGET *widget)
{
	if (widget != NULL && widget->currentWorkPages != 0) {
		DATA *page = Array_getFromIndex(widget->allPages, widget->currentActivatePage);
		if (page->type == ARRAY_TYPE) {
			ARRAY *children = page->buffer;
			if (children != NULL) {
				DATA *data = children->first;
				while (data != NULL) {
					((WIDGET *) data->buffer)->visible = false;
					data = data->next;
				}
			}
		}
	}
}

void Widget_setActivePage(WIDGET *widget, Uint32 page)
{
	if (widget != NULL) {
		if (page <= widget->nbPages) {
			noVisibleChild(widget);
			widget->currentActivatePage = page;
		}
		widget->needUpdate = true;
	}
}

Uint32 Widget_getActivePage(WIDGET *widget)
{
	Uint32 page = 0;
	if (widget != NULL) {
		page = widget->currentActivatePage;
	}
	return page;
}

void Widget_nextActivePage(WIDGET *widget)
{
	if (widget != NULL) {
		if (widget->nbPages > 0) {
			noVisibleChild(widget);
			widget->currentActivatePage++;
			if (widget->currentActivatePage > widget->nbPages) widget->currentActivatePage = 1;
			widget->needUpdate = true;
		}
	}
}

void Widget_afterActivePage(WIDGET *widget)
{
	if (widget != NULL) {
		if (widget->nbPages > 0) {
			noVisibleChild(widget);
			widget->currentActivatePage--;
			if (widget->currentActivatePage == 0) widget->currentActivatePage = widget->nbPages;
			widget->needUpdate = true;
		}
	}
}

void Widget_pressedSound(WIDGET *widget, char *soundName)
{
	if (widget != NULL) {
		if (widget->pressedSoundName != NULL) {
			free(widget->pressedSoundName);
			widget->pressedSoundName = NULL;
		} else if (Driver_loadSound(soundName) != NULL)
			widget->pressedSoundName = strdup(soundName);
	}
}

void Widget_releasedSound(WIDGET *widget, char *soundName)
{
	if (widget != NULL) {
		if (widget->releasedSoundName != NULL) {
			free(widget->releasedSoundName);
			widget->releasedSoundName = NULL;
		} else if (Driver_loadSound(soundName) != NULL)
			widget->releasedSoundName = strdup(soundName);
	}
}


void Widget_setFunc(WIDGET *widget, callbackUserFunc func)
{
	if (widget != NULL) {
		widget->userFunc.user = func;
	}
}

void Widget_setGainFocusedFunc(WIDGET *widget, callbackUserFunc func)
{
	if (widget != NULL) {
		widget->userFunc.gainFocused = func;
	}
}

void Widget_setLoseFocusedFunc(WIDGET *widget, callbackUserFunc func)
{
	if (widget != NULL) {
		widget->userFunc.gainFocused = func;
	}
}

void Widget_setOnFocusedFunc(WIDGET *widget, callbackUserFunc func)
{
	if (widget != NULL) {
		widget->userFunc.gainFocused = func;
	}
}

void Widget_setOnPressedFunc(WIDGET *widget, callbackUserFunc func)
{
	if (widget != NULL) {
		widget->userFunc.onPressed = func;
	}
}

void Widget_setOnReleasedFunc(WIDGET *widget, callbackUserFunc func)
{
	if (widget != NULL) {
		widget->userFunc.onReleased = func;
	}
}

void Widget_setOnHeldFunc(WIDGET *widget, callbackUserFunc func)
{
	if (widget != NULL) {
		widget->userFunc.onHeld = func;
	}
}

void Widget_setOnDragFunc(WIDGET *widget, callbackUserDragFunc func)
{
	if (widget != NULL) {
		widget->userFunc.onDrag = func;
	}
}


void Widget_dragable(WIDGET *widget, bool dragable)
{
	if (widget != NULL) {
		widget->dragable = dragable;
	}
}

bool Widget_isDragable(WIDGET *widget)
{
	if (widget != NULL) {
		return widget->dragable;
	}
	return false;
}

bool Widget_gainFocused(WIDGET *widget)
{
	if (widget != NULL) {
		return widget->flags.gain;
	}
	return false;
}

bool Widget_loseFocused(WIDGET *widget)
{
	if (widget != NULL) {
		return widget->flags.lose;
	}
	return false;
}

bool Widget_onFocused(WIDGET *widget)
{
	if (widget != NULL) {
		return widget->flags.focused;
	}
	return false;
}

bool Widget_onPressed(WIDGET *widget)
{
	if (widget != NULL) {
		return widget->flags.pressed;
	}
	return false;
}

bool Widget_onReleased(WIDGET *widget)
{
	if (widget != NULL) {
		return widget->flags.released;
	}
	return false;
}

bool Widget_onHeld(WIDGET *widget)
{
	if (widget != NULL) {
		return widget->flags.held;
	}
	return false;
}

WIDGET *findChild(ARRAY *children, char *name)
{
	/* on cherche dans toute la liste */
	WIDGET *thisWidget = NULL;
	if (children != NULL) {
		DATA *data = children->first;
		while (data != NULL) {
			if (strcmp(data->key, name) == 0) {
				thisWidget = data->buffer;
				break;
			} else {
				thisWidget = Widget_findChild(data->buffer, name);
				if (thisWidget != NULL) break;
			}
			data = data->next;
		}
	}
	return thisWidget;
}

WIDGET *Widget_findChild(WIDGET *widget, char *name)
{
	WIDGET *thisWidget = NULL;
	if (widget != NULL) {
		/* on cherche dans toutes les pages (widget->allPages est une liste de listes de widgets) */
		DATA *data = widget->allPages->first;
		while (data != NULL) {
			thisWidget = findChild((ARRAY *) data->buffer, name);
			if (thisWidget != NULL) break;
			data = data->next;
		}
	}
	return thisWidget;
}

void Widget_callbackEvent(WIDGET *widget, bool callback)
{
	if (widget != NULL) {
		if (callback) {
			if (widget->flags.gain && widget->userFunc.gainFocused != NULL)
				widget->userFunc.gainFocused(widget, widget->userdata);
			
			if (widget->flags.lose && widget->userFunc.gainFocused != NULL)
				widget->userFunc.gainFocused(widget, widget->userdata);
			
			if (widget->flags.pressed) {
				if (widget->pressedSoundName != NULL)
					Driver_playSound(widget->pressedSoundName, 0);
				if (widget->userFunc.onPressed != NULL)
					widget->userFunc.onPressed(widget, widget->userdata);
			}
			
			if (widget->flags.released) {
				if (widget->releasedSoundName != NULL)
					Driver_playSound(widget->releasedSoundName, 0);
				if (widget->userFunc.onReleased != NULL)
					widget->userFunc.onReleased(widget, widget->userdata);
			};

			if (widget->drag && widget->userFunc.onDrag != NULL) {
				widget->userFunc.onDrag(widget, widget->dragMoveX, widget->dragMoveY, widget->userdata);
			}

			if (widget->userFunc.user != NULL)
				widget->userFunc.user(widget, widget->userdata);
		}

		if (widget->flags.focused && widget->userFunc.gainFocused != NULL)
			widget->userFunc.gainFocused(widget, widget->userdata);
		
		if (widget->flags.held && widget->userFunc.onHeld != NULL)
			widget->userFunc.onHeld(widget, widget->userdata);
	}
}

void Widget_eventChildren(ARRAY *children)
{
	if (children != NULL) {
		DATA *data = children->first;
		while (data != NULL) {
			Widget_event((WIDGET *) data->buffer);
			data = data->next;
		}
	}
}

void Widget_resetEvent(WIDGET *widget)
{
	if (widget != NULL && widget->enable &&widget->visible) {
		if (widget->flags.lose) widget->flags.lose = false;
		/*if (widget->flags.focused && !widget->flags.lose) {
			widget->flags.lose = true;
			callback = true;
			widget->flags.focused = false;
		}*/
		if (widget->flags.gain) widget->flags.gain = false;

		if (widget->flags.pressed) widget->flags.pressed = false;
		if (widget->flags.released) widget->flags.released = false;
		if (widget->flags.held) widget->flags.held = false;

		if (widget->dragEnable) widget->dragEnable = false;
		if (widget->drag) widget->drag = false;
	}
}

void Widget_event(WIDGET *widget)
{
	if (widget != NULL && widget->enable &&widget->visible) {
		bool callback = false;
		SDL_Point mouse;
		Input_getMousePosition(&mouse);

		/* si la souris est sur le widget on modifie les flags d'évènements */
		if (CollisionPointRect(mouse.x, mouse.y, &widget->absolute)) {
			if (!widget->flags.focused) {
				widget->flags.focused = true;
				widget->flags.gain = true;
				callback = true;
			} else {
				widget->flags.gain = false;
			}
			
			if (!widget->flags.held && Input_wasMousePressed(SDL_BUTTON_LEFT)) {
				widget->flags.pressed = true;
				widget->flags.held = true;

				if (widget->dragable && !widget->dragEnable) {
					widget->dragEnable = true;
					Input_getMousePosition(&widget->dragLastPos);
				}
				callback = true;
			} else {
				if (widget->flags.pressed) widget->flags.pressed = false;
			}

			if (widget->flags.held && Input_wasMouseReleased(SDL_BUTTON_LEFT)) {
				widget->flags.released = true;
				widget->flags.held = false;

				if (widget->dragEnable) widget->dragEnable = false;
				callback = true;
			} else {
				if (widget->flags.released) widget->flags.released = false;
			}

			if (widget->dragEnable && (Input_getMouseMoveX() || Input_getMouseMoveY())) {
				widget->drag = true;
				SDL_Point pos;
				Input_getMousePosition(&pos);
				Widget_move(widget, pos.x - (widget->dragLastPos).x, pos.y - (widget->dragLastPos).y);
				//Widget_move(widget, Input_getMouseMoveX(), Input_getMouseMoveY());
				Input_getMousePosition(&widget->dragLastPos);

				DEBUG("widget drag");
				
				callback = true;
			} else {
				if (widget->drag) widget->drag = false;	
			}

			if (widget->resetParentEvent)
				Widget_resetEvent(widget->parent);
		} else {
			if (widget->flags.lose) widget->flags.lose = false;
			if (widget->flags.focused && !widget->flags.lose) {
				widget->flags.lose = true;
				callback = true;
				widget->flags.focused = false;
			}
			if (widget->flags.gain) widget->flags.gain = false;

			if (widget->flags.pressed) widget->flags.pressed = false;
			if (widget->flags.released) widget->flags.released = false;
			if (widget->flags.held) widget->flags.held = false;

			if (widget->dragEnable) widget->dragEnable = false;
			if (widget->drag) widget->drag = false;
		}
		
		if (widget->eventFunc != NULL) widget->eventFunc(widget);
		
		DATA *page = widget->allPages->first;
		if (page->type == ARRAY_TYPE) Widget_eventChildren(page->buffer);

		page = Array_getFromIndex(widget->allPages, widget->currentActivatePage);
		if (page->type == ARRAY_TYPE) Widget_eventChildren(page->buffer);

		Widget_callbackEvent(widget, callback);
	}
}

void Widget_updateChildren(ARRAY *children, bool needUpdate)
{
	if (children != NULL) {
		DATA *data = children->first;
		while (data != NULL) {
			WIDGET *child = data->buffer;
			if (needUpdate) child->needUpdate = true;
			Widget_update(child);
			data = data->next;
		}
	}
}

void Widget_update(WIDGET *widget)
{
	if (widget != NULL && widget->enable) {
		if (widget->needUpdate == true) {
			widget->visible = true;
			if (widget->parent != NULL) {
				SDL_Rect *geom = &widget->geometry;
				SDL_Rect area;
				Sint32 row_height, col_width;
				
				RectSet(&widget->geometryWithoutBorder,
					geom->x + widget->borderSize.left,
					geom->y + widget->borderSize.top,
					geom->w - widget->borderSize.left - widget->borderSize.right,
					geom->h - widget->borderSize.top - widget->borderSize.bottom);

				if (widget->parent->usedArea) {
					RectCopy(&area, &widget->parent->area);
				} else {
					RectSet(&area, 0, 0, widget->parent->geometry.w, widget->parent->geometry.h);
				}

				row_height = area.h / widget->parent->nb_row;
				col_width  = area.w / widget->parent->nb_col;

				if (widget->align_pourcent.w != 0) {
					float w = (col_width * widget->align_pourcent.w)/100.0;
					geom->w = (Sint32) w + widget->align_offset.w;
				}

				if (widget->align_pourcent.h != 0) {
					float h = (row_height * widget->align_pourcent.h)/100.0;
					geom->h = (Sint32) h + widget->align_offset.h;
				}

				if (widget->align_pourcent.x != 0) {
					float x = (col_width * widget->align_pourcent.x)/100.0;
					geom->x = (Sint32) x + widget->col * col_width + widget->align_offset.x;
					if (widget->x_centered) geom->x -= geom->w/2;
				} else {
					geom->x = widget->position.x;
					if (widget->col != 0) geom->x += widget->col * col_width;
					DEBUG("geom->x = %d", geom->x);
				}

				if (widget->align_pourcent.y != 0) {
					float y = (row_height * widget->align_pourcent.y)/100.0;
					geom->y = (Sint32) y + widget->row * row_height + widget->align_offset.y;
					if (widget->y_centered) geom->y -= geom->h/2;
				} else {
					geom->y = widget->position.y;
					if (widget->row != 0) geom->y += widget->row * row_height;
					DEBUG("geom->y = %d", geom->y);
				}

				widget->absolute.x = widget->parent->absolute.x + widget->parent->borderSize.left + geom->x - area.x;
				if (widget->absolute.x < widget->parent->absolute.x)
					widget->absolute.x = widget->parent->absolute.x;

				widget->absolute.y = widget->parent->absolute.y + widget->parent->borderSize.top + geom->y - area.y;
				if (widget->absolute.y < widget->parent->absolute.y)
					widget->absolute.y = widget->parent->absolute.y;

				widget->composite.w = widget->absolute.w = geom->w;
				widget->composite.h = widget->absolute.h = geom->h;

				if (geom->x < area.x) {
					widget->composite.x = area.x - geom->x;
					widget->absolute.w -= widget->composite.x;
				} else {
					widget->composite.x = 0;
				}

				if (geom->y < area.y) {
					widget->composite.y = area.y - geom->y;
					widget->absolute.h -= widget->composite.y;
				} else {
					widget->composite.y = 0;
				}

				RectSet(&widget->clipRect, widget->absolute.x - widget->parent->absolute.x,
					widget->absolute.y - widget->parent->absolute.y,
					widget->absolute.w /*- widget->borderSize.left - widget->borderSize.right*/,
					widget->absolute.h /*- widget->borderSize.top - widget->borderSize.bottom*/);

				if (widget->clipRect.x + widget->clipRect.w > widget->parent->clipRect.w)
					widget->absolute.w -= (widget->clipRect.x + widget->clipRect.w) - widget->parent->clipRect.w;

				if (widget->clipRect.y + widget->clipRect.h > widget->parent->clipRect.h)
					widget->absolute.h -= (widget->clipRect.y + widget->clipRect.h) - widget->parent->clipRect.h;
				
				if (widget->absolute.w <= 0) {
					widget->absolute.w = 0;
					widget->visible = false;
				}
				if (widget->absolute.h <= 0) {
					widget->absolute.h = 0;
					widget->visible = false;
				}
			} else {
				RectCopy(&widget->geometryWithoutBorder, &widget->geometry);
				RectCopy(&widget->absolute, &widget->geometry);
				RectCopy(&widget->clipRect, &widget->geometry);
				RectSet(&widget->composite, 0, 0, widget->geometry.w, widget->geometry.h);
			}

			if (widget->visible) {
				/* Si les dimensions du widget ont augmenté, la texture est détruite
				 * et est recréée avec les bonnes dimensions */
				if (widget->texture != NULL) {
					Sint32 w, h;
					SDL_QueryTexture(widget->texture, NULL, NULL, &w, &h);
					if (w < widget->geometry.w || h < widget->geometry.h) {
						SDL_DestroyTexture(widget->texture);
						widget->texture = NULL;
					}
				}
				
				if (widget->texture == NULL) {
					widget->texture = Driver_createTexture(widget->geometry.w, widget->geometry.h);
				}
			}
		}

		if (widget->visible) {
			if (widget->updateFunc != NULL) widget->updateFunc(widget);
			
			DATA *page = widget->allPages->first;
			if (page->type == ARRAY_TYPE) {
				Widget_updateChildren(page->buffer, widget->needUpdate);
			}
			page = Array_getFromIndex(widget->allPages, widget->currentActivatePage);
			if (page->type == ARRAY_TYPE) {
				Widget_updateChildren(page->buffer, widget->needUpdate);
			}
		}

		if (widget->needUpdate == true) {
			widget->needUpdate = false;
		}
	}
}

void Widget_drawChildren(ARRAY *children)
{
	if (children != NULL) {
		DATA *data = children->first;
		while (data != NULL) {
			Widget_draw((WIDGET *) data->buffer);
			data = data->next;
		}
	}
}

void Widget_draw(WIDGET *widget)
{
	if (widget != NULL && widget->enable && widget->visible) {
		Driver_setTarget(widget->texture);
		Driver_drawColor(widget->bgColor);
		Driver_clear();

		if (widget->drawFunc != NULL) widget->drawFunc(widget);
		
		DATA *page = widget->allPages->first;
		if (page->type == ARRAY_TYPE) {
			Widget_drawChildren(page->buffer);
		}
		page = Array_getFromIndex(widget->allPages, widget->currentActivatePage);
		if (page->type == ARRAY_TYPE) {
			Widget_drawChildren(page->buffer);
		}
		
		if (widget->parent != NULL) {
			Driver_setTarget(widget->parent->texture);
		} else {
			Driver_setTarget(NULL);
		}
		
		Driver_copy(widget->texture, &widget->composite, &widget->clipRect);
	}
}


void Widget_drawTile(Sint32 tilesetX, Sint32 tilesetY, IMAGE *tileset, SDL_Rect *tileRect, SDL_Rect *geometry)
{
	if (tileset != NULL) {
		SDL_Rect src, dest;

		src.w = tileRect->w;
		src.h = tileRect->h;

		if (geometry->w > tileRect->w*2) {
			src.x = tilesetX + tileRect->w;
			dest.x = tilesetY + tileRect->w;
			dest.w = geometry->w - tileRect->w*2;
			dest.h = tileRect->h;

			/* côté haut */
			src.y = dest.y = 0;
			Driver_copy(tileset->texture, &src, &dest);
				
			/* côté bas */
			src.y = tileRect->h*2;
			dest.y = geometry->h - tileRect->h;
			Driver_copy(tileset->texture, &src, &dest);
		}
		
		if (geometry->h > tileRect->h*2) {
			src.y = tileRect->h;
			dest.y = tileRect->h;
			dest.w = tileRect->w;
			dest.h = geometry->h - tileRect->h*2;

			/* côté gauche */
			src.x = tilesetX;
			dest.x = tilesetY;
			Driver_copy(tileset->texture, &src, &dest);
				
			/* côté droite */
			src.x = tilesetX + tileRect->w*2;
			dest.x = tilesetY + geometry->w - tileRect->w;
			Driver_copy(tileset->texture, &src, &dest);
		}
		
		if ((geometry->w > tileRect->w*2) && (geometry->h > tileRect->h*2)) {
			/* centre */
			src.x = tilesetX + tileRect->w;
			src.y = tilesetY + tileRect->h;
			dest.x = tileRect->w;
			dest.y = tileRect->h;
			dest.w = geometry->w - tileRect->w*2;
			dest.h = geometry->h - tileRect->h*2;
			Driver_copy(tileset->texture, &src, &dest);
		}

		/* coin haut gauche */
		src.x = tilesetX;
		src.y = tilesetY;
		tileRect->x = tileRect->y = 0;
		Driver_copy(tileset->texture, &src, tileRect);

		/* coin haut droit */
		src.x = tilesetX + tileRect->w*2;
		src.y = tilesetY;
		tileRect->x = geometry->w - tileRect->w;
		tileRect->y = 0;
		Driver_copy(tileset->texture, &src, tileRect);
		
		/* coin bas gauche */
		src.x = tilesetX;
		src.y = tilesetY + tileRect->h*2;
		tileRect->x = 0;
		tileRect->y = geometry->h - tileRect->h;
		Driver_copy(tileset->texture, &src, tileRect);
		
		/* coin bas droit */
		src.x = tilesetX + tileRect->w*2;
		src.y = tilesetY + tileRect->h*2;
		tileRect->x = geometry->w - tileRect->w;
		tileRect->y = geometry->h - tileRect->h;
		Driver_copy(tileset->texture, &src, tileRect);
	}
}