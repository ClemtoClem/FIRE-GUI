#include "geometrical.h"
#include "../core/driver.h"
#include "../core/input.h"

WIDGET *RectangleWidget_new(char *name_parent, char *name, SDL_Rect *geometry,
    Uint32 borderColor, Uint32 bgColor, Uint32 survolBorderColor, Uint32 survolBgColor, Uint32 borderSize,
	callbackUserFunc userFunc, void *userdata)
{
    WIDGET *widget = NULL;
	RECTANGLE_WIDGET *rectWidget = calloc(1, sizeof(RECTANGLE_WIDGET));
	if (rectWidget != NULL) {
		rectWidget->borderColor = borderColor;
		rectWidget->bgColor = bgColor;
		rectWidget->survolBorderColor = survolBorderColor;
		rectWidget->survolBgColor = survolBgColor;
		rectWidget->borderSize = borderSize;

		bool allocRect;
		if (geometry == NULL) {
			geometry = AllocRect();
			allocRect = true;
		} else {
			allocRect = false;
		}

		widget = Widget_new(name_parent, name, RECTANGLE_WIDGET_TYPE, geometry, NULL, userFunc, userdata,
			rectWidget, NULL /*&RectangleWidget_event*/, NULL /*&RectangleWidget_update*/, &RectangleWidget_draw, &RectangleWidget_free);
		
		if (allocRect) free(geometry);
		SUCCESS("create rectangle widget");
	}
	return widget;
}

void RectangleWidget_free(WIDGET *widget)
{
    if (widget != NULL && widget->type == RECTANGLE_WIDGET_TYPE) {
        free((RECTANGLE_WIDGET *) widget->private);
    }
}

/*
void RectangleWidget_event(WIDGET *widget)
{
    if (widget != NULL && widget->type == RECTANGLE_WIDGET_TYPE) {

    }
}

void RectangleWidget_update(WIDGET *widget)
{
    if (widget != NULL && widget->type == RECTANGLE_WIDGET_TYPE) {
		
    }
}
*/

void RectangleWidget_draw(WIDGET *widget)
{
    if (widget != NULL && widget->type == RECTANGLE_WIDGET_TYPE) {
		RECTANGLE_WIDGET *rectWidget = widget->private;
		Uint32 bgColor = (widget->flags.focused) ? rectWidget->survolBgColor : rectWidget->bgColor;
		SDL_Rect dest = {0, 0, widget->geometry.w, widget->geometry.h};
		if (bgColor != 0) {
			Driver_drawColor(bgColor);
			Driver_drawRect(&dest, true);
		}
		
		Uint32 bdColor = (widget->flags.focused) ? rectWidget->survolBorderColor : rectWidget->borderColor;
		if (bdColor != 0) {
			Driver_drawColor(bdColor);
			for (Uint32 i = 0; i < rectWidget->borderSize; i++) {
				dest.x++;
				dest.y++;
				dest.w-=2;
				dest.h-=2;
				Driver_drawRect(&dest, false);
			}
		}
    }
}
