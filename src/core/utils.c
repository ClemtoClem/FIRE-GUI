#include "utils.h"

SDL_Rect *AllocRect()
{
    SDL_Rect *rect = calloc(1, sizeof(SDL_Rect));
    return rect;
}

SDL_FRect *AllocFloatRect()
{
    SDL_FRect *frect = calloc(1, sizeof(SDL_FRect));
    return frect;
}


void RectSet(SDL_Rect *rectDest, Sint32 x, Sint32 y, Sint32 w, Sint32 h)
{
    rectDest->x = x;
    rectDest->y = y;
    rectDest->w = w;
    rectDest->h = h;
}

void FloatRectSet(SDL_FRect *rectDest, float x, float y, float w, float h)
{
    rectDest->x = x;
    rectDest->y = y;
    rectDest->w = w;
    rectDest->h = h;
}

void RectCopy(SDL_Rect *rectDest, SDL_Rect *rectSrc)
{
    rectDest->x = rectSrc->x;
    rectDest->y = rectSrc->y;
    rectDest->w = rectSrc->w;
    rectDest->h = rectSrc->h;
}

void FloatRectCopy(SDL_FRect *rectDest, SDL_FRect *rectSrc)
{
    rectDest->x = rectSrc->x;
    rectDest->y = rectSrc->y;
    rectDest->w = rectSrc->w;
    rectDest->h = rectSrc->h;
}

void RectZero(SDL_Rect *rectDest)
{
    rectDest->x = 0;
    rectDest->y = 0;
    rectDest->w = 0;
    rectDest->h = 0;
}

void FloatRectZero(SDL_FRect *rectDest)
{
    rectDest->x = 0;
    rectDest->y = 0;
    rectDest->w = 0;
    rectDest->h = 0;
}

void CircleSet(CIRCLE *circleDest, Sint32 x, Sint32 y, Sint32 radius)
{
    circleDest->x = x;
    circleDest->y = y;
    circleDest->radius = radius;
}

void FloatCircleSet(FCIRCLE *circleDest, float x, float y, float radius)
{
    circleDest->x = x;
    circleDest->y = y;
    circleDest->radius = radius;
}

void CircleCopy(CIRCLE *destCircle, CIRCLE *srcCircle)
{
    destCircle->x = srcCircle->x;
    destCircle->y = srcCircle->y;
    destCircle->radius = srcCircle->radius;
}

void FloatCircleCopy(FCIRCLE *destCircle, FCIRCLE *srcCircle)
{
    destCircle->x = srcCircle->x;
    destCircle->y = srcCircle->y;
    destCircle->radius = srcCircle->radius;
}

void CircleZero(CIRCLE *circle)
{
    circle->x = 0;
    circle->y = 0;
    circle->radius = 0;
}

void FloatCircleZero(FCIRCLE *circle)
{
    circle->x = 0;
    circle->y = 0;
    circle->radius = 0;
}

bool CollisionPointRect(Sint32 x, Sint32 y, SDL_Rect *box)
{
    if ((x >= box->x)
        && (x < box->x + box->w)
        && (y >= box->y)
        && (y < box->y + box->h))
        return true;
    else
        return false;
}

bool FloatCollisionPointRect(float x, float y, SDL_FRect *box)
{
    if ((x >= box->x)
        && (x < box->x + box->w)
        && (y >= box->y)
        && (y < box->y + box->h))
        return true;
    else
        return false;
}

bool CollisionRect(SDL_Rect *box1, SDL_Rect *box2)
{
    if ((box2->x >= box1->x + box1->w)     // trop à droite
        || (box2->x + box2->w <= box1->x)  // trop à gauche
        || (box2->y >= box1->y + box1->h)  // trop en bas
        || (box2->y + box2->h <= box1->y)) // trop en haut
        return false;
    else
        return true;
}

bool FloatCollisionRect(SDL_FRect *box1, SDL_FRect *box2)
{
    if ((box2->x >= box1->x + box1->w)     // trop à droite
        || (box2->x + box2->w <= box1->x)  // trop à gauche
        || (box2->y >= box1->y + box1->h)  // trop en bas
        || (box2->y + box2->h <= box1->y)) // trop en haut
        return false;
    else
        return true;
}

bool CollisionPointCircle(Sint32 x, Sint32 y, CIRCLE *c)
{
    Sint32 d2 = (x - c->x) * (x - c->x) + (y - c->y) * (y - c->y);
    if (d2 > c->radius*c->radius)
        return false;
    else
        return true;
}

bool FloatCollisionPointCircle(float x, float y, FCIRCLE *c)
{
    Sint32 d2 = (x - c->x) * (x - c->x) + (y - c->y) * (y - c->y);
    if (d2 > c->radius*c->radius)
        return false;
    else
        return true;
}

bool CollisionCircle(CIRCLE *c1, CIRCLE *c2)
{
    Sint32 d2 = (c1->x - c2->x) * (c1->x - c2->x) + (c1->y - c2->y) * (c1->y - c2->y);
    if (d2 > (c1->radius + c2->radius) * (c1->radius + c2->radius))
        return false;
    else
        return true;
}

bool FloatCollisionCircle(FCIRCLE *c1, FCIRCLE *c2)
{
    float d2 = (c1->x - c2->x) * (c1->x - c2->x) + (c1->y - c2->y) * (c1->y - c2->y);
    if (d2 > (c1->radius + c2->radius) * (c1->radius + c2->radius))
        return false;
    else
        return true;
}

bool CollisionRectCircle(SDL_Rect *box, CIRCLE *c)
{
    SDL_Rect circleBox = {c->x - c->radius/2, c->y - c->radius/2, c->x + c->radius/2, c->y + c->radius/2};

    if (CollisionRect(box, &circleBox) == false)
        return false;
    if (CollisionPointCircle(box->x, box->y, c)
        || CollisionPointCircle(box->x, box->y + box->h, c)
        || CollisionPointCircle(box->x + box->w, box->y, c)
        || CollisionPointCircle(box->x + box->w, box->y + box->h, c))
        return true;
    if (CollisionPointRect(c->x, c->y, box))
        return true;
    bool projV = ProjectionSurSegment(c->x, c->y, box->x, box->y, box->x, box->y + box->h);
    bool projH = ProjectionSurSegment(c->x, c->y, box->x, box->y, box->x + box->w, box->y);
    if (projV || projH)
        return true;
    return
        false;
}

bool FloatCollisionRectCircle(SDL_FRect *box, FCIRCLE *c)
{
    SDL_FRect circleBox = {c->x - c->radius/2, c->y - c->radius/2, c->x + c->radius/2, c->y + c->radius/2};

    if (FloatCollisionRect(box, &circleBox) == false)
        return false;
    if (FloatCollisionPointCircle(box->x, box->y, c)
        || FloatCollisionPointCircle(box->x, box->y + box->h, c)
        || FloatCollisionPointCircle(box->x + box->w, box->y, c)
        || FloatCollisionPointCircle(box->x + box->w, box->y + box->h, c))
        return true;
    if (FloatCollisionPointRect(c->x, c->y, box))
        return true;
    bool projV = FloatProjectionSurSegment(c->x, c->y, box->x, box->y, box->x, box->y + box->h);
    bool projH = FloatProjectionSurSegment(c->x, c->y, box->x, box->y, box->x + box->w, box->y);
    if (projV || projH)
        return true;
    return
        false;
}

bool ProjectionSurSegment(Sint32 Cx, Sint32 Cy, Sint32 Bx, Sint32 By, Sint32 Ax, Sint32 Ay)
{
    Sint32 ACx = Cx - Ax;
    Sint32 ACy = Cy - Ay;
    Sint32 ABx = Bx - Ax;
    Sint32 ABy = By - Ay;
    Sint32 BCx = Cx - Bx;
    Sint32 BCy = Cy - By;
    Sint32 s1 = (ACx * ABx) + (ACy * ABy);
    Sint32 s2 = (BCx * ABx) + (BCy + ABy);
    if (s1 * s2 > 0) return false;
    else return true;
}

bool FloatProjectionSurSegment(float Cx, float Cy, float Bx, float By, float Ax, float Ay)
{
    float ACx = Cx - Ax;
    float ACy = Cy - Ay;
    float ABx = Bx - Ax;
    float ABy = By - Ay;
    float BCx = Cx - Bx;
    float BCy = Cy - By;
    float s1 = (ACx * ABx) + (ACy * ABy);
    float s2 = (BCx * ABx) + (BCy + ABy);
    if (s1 * s2 > 0) return false;
    else return true;
}