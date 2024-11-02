#include "image.h"

IMAGE *Image_new(SDL_Texture *texture, int w, int h)
{
	IMAGE *image = malloc(sizeof(IMAGE));
	if (image != NULL) {
		image->texture = texture;
		image->x = 0;
		image->y = 0;
		image->w = w;
		image->h = h;
	}
	return image;
}

void Image_free(IMAGE *image)
{
	if (image != NULL) {
		if(image->texture != NULL) SDL_DestroyTexture(image->texture);
		free(image);
	}
}

