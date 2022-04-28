#ifndef GAME_H
#define GAME_H

#include "raycasting.h"

/* ------------------ Protoypes des fonctions -------------------*/

void mainloop();

event* init_event();
void event_update(event *e,int etat);
void event_handling(event* e,graphic *G,player *p,int tab[][N]);
void map_generator(int map[][N]);

#endif