#ifndef __CORE_UTILS_H__
#define __CORE_UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 *          __    __  __
 *  __ __ _/  |_ |__||  |    ______ 
 * |  |  \\   __\ __ |  |   /  ___/ 
 * |  |  / |  |  |  ||  |__ \___ \  
 * |____/  |__|  |__||____//____  > 
                                \/  
 * Fonctions pour manipulation de formes geometriques entre elle (rectangle, cercle, point)
 */

typedef struct t_CIRCLE {
    Sint32 x, y, radius;
} CIRCLE;

typedef struct t_FCIRCLE {
    float x, y, radius;
} FCIRCLE;

#define ABS(x) ((x<0) ? (-x) : x)

SDL_Rect *AllocRect();
SDL_FRect *AllocFloatRect();

void RectSet(SDL_Rect *rectDest, Sint32 x, Sint32 y, Sint32 w, Sint32 h);
void FloatRectSet(SDL_FRect *rectDest, float x, float y, float w, float h);

/**
 * @details Copie d'un rectangle dans un autre
 * @param rectDest rectangle dans le quel on doit intéger les valeurs
 * @param rectSrc rectangle source
*/
void RectCopy(SDL_Rect *rectDest, SDL_Rect *rectSrc);
void FloatRectCopy(SDL_FRect *rectDest, SDL_FRect *rectSrc);

/**
 * @details Met à zero le rectangle
 * @param rectDest rectangle dans le quel on doit intéger les valeurs
*/
void RectZero(SDL_Rect *rectDest);
void FloatRectZero(SDL_FRect *rectDest);


void CircleSet(CIRCLE *circleDest, Sint32 x, Sint32 y, Sint32 radius);
void FloatCircleSet(FCIRCLE *circleDest, float x, float y, float radius);

/**
 * @details Copie d'un cercle dans un autre
 * @param destCircle cercle dans le quel on doit intéger les valeurs
 * @param srcCircle cercle source
*/
void CircleCopy(CIRCLE *destCircle, CIRCLE *srcCircle);
void FloatCircleCopy(FCIRCLE *destCircle, FCIRCLE *srcCircle);

/**
 * @details Met à zero le cercle
 * @param circle cercle dans le quel on doit intéger les valeurs
*/
void CircleZero(CIRCLE *circle);
void FloatCircleZero(FCIRCLE *circle);

/**
 * @details Calcul de collision entre un point et un rectangle
 * @param x ascisse point
 * @param y ordonnée point
 * @param box rectangle
*/
bool CollisionPointRect(Sint32 x, Sint32 y, SDL_Rect *box);
bool FloatCollisionPointRect(float x, float y, SDL_FRect *box);

/**
 * @details Calcul de collision entre deux rectangles
 * @param box1 rectangle 1
 * @param box2 rectangle 2
*/
bool CollisionRect(SDL_Rect *box1, SDL_Rect *box2);
bool FloatCollisionRect(SDL_FRect *box1, SDL_FRect *box2);

/**
 * @details Calcul de collision entre un point et un cercle
 * @param x ascisse point
 * @param y ordonnée point
 * @param c cercle
*/
bool CollisionPointCircle(Sint32 x, Sint32 y, CIRCLE *c);
bool FloatCollisionPointCircle(float x, float y, FCIRCLE *c);

/**
 * @details Calcul de collision entre deux cercles
 * @param c1 cercle 1
 * @param c2 cercle 2
*/
bool CollisionCircle(CIRCLE *c1, CIRCLE *c2);
bool FloatCollisionCircle(FCIRCLE *c1, FCIRCLE *c2);

/**
 * @details Calcul de collision entre deux cercles
 * @param c1 cercle 1
 * @param c2 cercle 2
*/
bool CollisionRectCircle(SDL_Rect *box, CIRCLE *c);
bool FloatCollisionRectCircle(SDL_FRect *box, FCIRCLE *c);

bool ProjectionSurSegment(Sint32 Cx, Sint32 Cy, Sint32 Bx, Sint32 By, Sint32 Ax, Sint32 Ay);
bool FloatProjectionSurSegment(float Cx, float Cy, float Bx, float By, float Ax, float Ay);

#endif /* __CORE_UTILS_H__ */