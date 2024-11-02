#include "timer.h"

TIMER *Timer_new(bool activate)
{
    TIMER *timer = malloc(sizeof(TIMER));
    if (timer != NULL) {
        timer->activate = activate;
        timer->head = NULL;
    }
    return timer;
}

void Timer_free(TIMER *timer)
{
    if (timer != NULL) {
        COUNTER *cnext, *count;
        cnext = timer->head;
        while (cnext != NULL) {
            count = cnext;
            cnext = count->next;
            free(count);
        }
        free(timer);
    }
}

COUNTER *Timer_getCounter(TIMER *timer, Uint32 id)
{
    COUNTER *count = NULL;
    if (timer != NULL) {
        count = timer->head;
        while (count != NULL) {
            if (count->id == id) {
                break;
            }
            count = count->next;
        }
    }
    return count;
}


void Timer_update(TIMER *timer)
{
    if (timer != NULL && timer->activate) {
        COUNTER *count = timer->head;
        while (count != NULL) {
            if (count->activate) {
                if (count->loop && count->finish) count->finish = false;
                if (!count->finish) {
                    count->value++;
                    if (count->value > count->endValue) {
                        count->finish = true;
                        count->value = 0;
                        /* on désactive le compteur si on ne veut pas compter en boucle */
                    }
                }
            }
            count = count->next;
        }
    }
}


void Timer_stop(TIMER *timer)
{
    if (timer != NULL) {
        timer->activate = false;
    }
}

void Timer_activate(TIMER *timer)
{
    if (timer != NULL) {
        timer->activate = true;
    }
}

void Timer_reset(TIMER *timer)
{
    if (timer != NULL) {
        COUNTER *count = timer->head;
        while (count != NULL) {
            count->finish = false;
            count->value = 0;
            count = count->next;
        }
    }
}


void Timer_addCounter(TIMER *timer, Uint32 id, Uint32 endValue, bool loop, bool activate)
{
    if (timer != NULL) {
        COUNTER **count = &timer->head;
        while ((*count) != NULL) {
            if ((*count)->id == id) break;
            count = &((*count)->next);
        }

        if ((*count) == NULL) {
            (*count) = (COUNTER *) malloc(sizeof(COUNTER));
            (*count)->id = id;
            (*count)->next = NULL;
        }
        (*count)->value = 0;
        (*count)->endValue = endValue;
        (*count)->finish = false;
        (*count)->loop = loop;
        (*count)->activate = activate;
    }
}

void Timer_stopCounter(TIMER *timer, Uint32 id)
{
    if (timer != NULL) {
        COUNTER *count = Timer_getCounter(timer, id);
        if (count != NULL) count->activate = false;
    }
}

void Timer_activateCounter(TIMER *timer, Uint32 id)
{
    if (timer != NULL) {
        COUNTER *count = Timer_getCounter(timer, id);
        if (count != NULL) count->activate = true;
    }
}

bool Timer_counterIsActivate(TIMER *timer, Uint32 id)
{
    bool activate = false;
    if (timer != NULL) {
        COUNTER *count = Timer_getCounter(timer, id);
        if (count != NULL) activate = count->activate;
    }
    return activate;
}

void Timer_resetCounter(TIMER *timer, Uint32 id)
{
    if (timer != NULL) {
        COUNTER *count = Timer_getCounter(timer, id);
        if (count != NULL) {
            count->value = 0;
            count->finish = false;
        }
    }
}

bool Timer_counterIsFinish(TIMER *timer, Uint32 id)
{
    bool finish = false;
    if (timer != NULL) {
        COUNTER *count = Timer_getCounter(timer, id);
        if (count != NULL) {
            finish = count->finish;
        }
    }
    return finish;
}

Uint32 Timer_getValueCounter(TIMER *timer, Uint32 id)
{
    Uint32 value = 0;
    if (timer != NULL) {
        COUNTER *count = Timer_getCounter(timer, id);
        if (count != NULL) {
            value = count->value;
        }
    }
    return value;
}

void Timer_deleteCounter(TIMER *timer, Uint32 id)
{
    if (timer != NULL && timer->head != NULL) {
        COUNTER *cnext;
        COUNTER *count = timer->head;
        if (count->id == id) {
            cnext = count->next;
            timer->head = cnext;
            free(count);
            count = NULL;
        } else {
            while (count != NULL) {
                cnext = count->next;
                if (cnext->id == id) {
                    count = cnext->next;
                    free(cnext);
                    cnext = NULL;
                    break;
                }
                count = cnext;
            }
        }
    }
}
