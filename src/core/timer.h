#ifndef __Timer_H__
#define __Timer_H__

/*
 ********************************************
 *				SOURCE CODE					*
 ********************************************
 * Compteurs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct t_COUNTER {
    Uint32 id;
    Uint32 value, endValue;
    bool finish, loop, activate;

    struct t_COUNTER *next;
} COUNTER;

typedef struct t_TIMER {
    COUNTER *head;
    bool activate;
} TIMER;

TIMER *Timer_new(bool activate);
void Timer_free(TIMER *timer);

COUNTER *Timer_getCounter(TIMER *timer, Uint32 id);

/* mettre à jour tout les compteurs */
void Timer_update(TIMER *timer);

/* arrêter le timer */
void Timer_stop(TIMER *timer);
/* activer le timer */
void Timer_activate(TIMER *timer);
/* reset tout les compteurs */
void Timer_reset(TIMER *timer);

/* Ajouter un compteur (si il existe déjà on modifie les variables endValue, loop et activate et remet value à 0)*/
void Timer_addCounter(TIMER *timer, Uint32 id, Uint32 endValue, bool loop, bool activate);
/* arrêter le compter */
void Timer_stopCounter(TIMER *timer, Uint32 id);
/* activer le compteur */
void Timer_activateCounter(TIMER *timer, Uint32 id);
/* renvoie true si le compteur est activé sinon false */
bool Timer_counterIsActivate(TIMER *timer, Uint32 id);
/* reset le compteur */
void Timer_resetCounter(TIMER *timer, Uint32 id);
/* vérifier si le compteur à fini ou à rebouclé */
bool Timer_counterIsFinish(TIMER *timer, Uint32 id);
/* retourner la valeur du compteur */
Uint32 Timer_getValueCounter(TIMER *timer, Uint32 id);
/* supprimer le compteur */
void Timer_deleteCounter(TIMER *timer, Uint32 id);

#endif