#ifndef RAYCASTING_H
#define RAYCASTING_H

#include "Graphic.h"
#include "player.h"

/* ------------------ Protoypes des fonctions -------------------*/

//Gestion du joueur
void init_player(player *p);
void UP(player *p,int tab[][N]);
void DOWN(player *p,int tab[][N]);

//Input du jeu
int input(player *p,int tab[][N]);
int input_v2(key *keys);
void Player_movement(player *p,key *key,int tab[][N]);

//Moteur du raycasting
void raycasting(player *p,graphic *G, int tab[][N],int dist[],textures *T);
int raydist(player *p,int tab[][N],int pos,float theta,graphic *G,textures *T,int dist[]);

//Fonctions Auxilliaires
float angle_trigo(float angle);
int check_tab(int x,int y);

#endif