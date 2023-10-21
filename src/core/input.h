#pragma once
#ifndef __SDL_INPUT_H__
#define __SDL_INPUT_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 * Sauvegarde des differents évènements dans trois listes:
 *	- pressedKeys pour l'action presser la touche
 *	- releasedKeys pour l'action relever la touche
 *	- heldKeys pour les touches maintenues enfoncees
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#define NB_MOUSE_EVENT 6
#define NB_KEYS_EVENT 125
#define NB_USER_EVENT 100

#define Input_wasKeyPressed(firstKey, ...) _Input_wasKeyPressed(firstKey, ##__VA_ARGS__, -1)
#define Input_wasKeyReleased(firstKey, ...) _Input_wasKeyReleased(firstKey, ##__VA_ARGS__, -1)
#define Input_wasKeyHeld(firstKey, ...) _Input_wasKeyHeld(firstKey, ##__VA_ARGS__, -1)
#define Input_wasMousePressed(firstButton, ...) _Input_wasMousePressed(firstButton, ##__VA_ARGS__, -1)
#define Input_wasMouseReleased(firstButton, ...) _Input_wasMouseReleased(firstButton, ##__VA_ARGS__, -1)
#define Input_wasMouseHeld(firstButton, ...) _Input_wasMouseHeld(firstButton, ##__VA_ARGS__, -1)
#define Input_wasUserEvent(fisrtCode, ...) _Input_wasUserEvent(fisrtCode, ##__VA_ARGS__, -1)

typedef struct t_EVT_MOUSE {
	Sint32 x, y, lastX, lastY;
	bool pressed[NB_MOUSE_EVENT];
	bool released[NB_MOUSE_EVENT];
	bool held[NB_MOUSE_EVENT];
	Sint32 wheelX, wheelY, moveX, moveY;
	bool wheelEvent, moveEvent;
} EVT_MOUSE;

typedef struct t_EVT_WINDOW {
	bool gainFocused, loseFocused, focused, resized;
	bool maximized, minimized, moved;
	Sint32 width, height;
} EVT_WINDOW;

typedef struct t_SDL_INPUT {
	SDL_Event event;
	bool pressedKeys[NB_KEYS_EVENT];
	bool releasedKeys[NB_KEYS_EVENT];
	bool heldKeys[NB_KEYS_EVENT];
	bool userEvent[NB_USER_EVENT];
	EVT_MOUSE mouse;
	EVT_WINDOW window;
	bool usedScancode; /* Scancode for QUERTY, Keycode for AZERTY */
	bool quitEvent, hasEvent;
} SDL_INPUT;

extern SDL_INPUT *intput;

/* initialisation de la sructure Input (input est une variable globale ne pas l'allouer) */
bool Input_init(bool usedScancode);
void Input_free();
SDL_INPUT *Input_get();

/* rénitialisation des évènements instantanés de la frame précédente (presser, relâcher)*/
void Input_resetFrame();

/* générer un évènement */
void Input_pushEvent(Uint32 type, Uint32 code);

/* générer un évènement utilisateur */
void Input_pushUserEvent(Uint32 code);

/* traitement et enregistrement des évènements de la SDL */
void Input_event();

/* renvoie true si le bouton quitter est pressé */
bool Input_quitEvent();
/* renvoie true si le bouton renseigné en paramètre est pressé */
bool _Input_wasKeyPressed(int firstKey, ...);
/* renvoie true si le bouton renseigné en paramètre est relâché */
bool _Input_wasKeyReleased(int firstKey, ...);
/* renvoie true si le bouton renseigné en paramètre est maintenue enfoncé */
bool _Input_wasKeyHeld(int firstKey, ...);

/* renvoie true si le bouton de la souris renseigné en paramètre est pressé */
bool _Input_wasMousePressed(Uint8 firstButton, ...);
/* renvoie true si le bouton de la souris renseigné en paramètre est relâché */
bool _Input_wasMouseReleased(Uint8 firstButton, ...);
/* renvoie true si le bouton de la souris renseigné en paramètre est maintenue enfoncé */
bool _Input_wasMouseHeld(Uint8 firstButton, ...);
/* renvoie la position de la souris */
void Input_getMousePosition(SDL_Point *pos);
/* renvoie true si la souris c'est déplacée */
bool Input_wasMotionEvent();
/* renvoie true si la molette a tournée */
bool Input_wasWheelEvent(Sint32 *wheelX, Sint32 *wheelY);

/* renvoie true si l'évènement renseigné en paramètre est activé */
bool _Input_wasUserEvent(int fisrtCode, ...);

int Input_getMouseMoveX();
int Input_getMouseMoveY();

#endif /* __SDL_INPUT_H__ */