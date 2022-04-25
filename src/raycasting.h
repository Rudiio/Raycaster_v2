#ifndef RAYCASTING_H
#define RAYCASTING_H

#include "Graphic.h"
#include "player.h"

/*----------------------- CONSTANTES -----------------------*/

//Taille de la map
#define CASE_SIZE 64

//Paramètres angulaires
#define PI 3.14
#define FOV PI/2        //Field of view


/* ------------------ Protoypes des fonctions -------------------*/

//Gestion du joueur
void init_player(player *p);
void UP(player *p,int tab[][N]);
void DOWN(player *p,int tab[][N]);

//Input du jeu
int input(player *p,int tab[][N]);

//Moteur du raycasting
void raycasting(player *p,graphic *G, int tab[][N],int dist[]);
int raydist(player *p,int tab[][N],int pos,float theta,graphic *G);

//Fonctions Auxilliaires
float angle_trigo(float angle);
float val_abs(float val);
int check_tab(int x,int y);

#endif