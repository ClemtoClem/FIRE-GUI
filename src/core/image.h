#ifndef __Image_H__
#define __Image_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 * Structure permettant de lie une texture SDL avec un rectangle
 * déterminant la taille de la textures et sa position à l'écran
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct t_IMAGE {
	SDL_Texture *texture;
	Sint32 x, y, w, h;
} IMAGE;

IMAGE *Image_new(SDL_Texture *texture, int w, int h);
void Image_free(IMAGE *image);

#endif /* __Image_H__ */