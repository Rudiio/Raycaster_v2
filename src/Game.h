#ifndef GAME_H
#define GAME_H

#include "raycasting.h"

/* ------------------ Protoypes des fonctions -------------------*/

void mainloop();

void init_event(event *e);
void event_update(event *e,int etat);
void event_handling(event* e,graphic *G,player *p,int tab[][N]);

#endif