#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL2/SDL.h>
#include "player.h"

/*----------------------- CONSTANTES -----------------------*/

//Define pour l'affichage
#define WIDTH 1300
#define HEIGTH 720
#define CASE_SIZE 64
#define DX 0
#define DY 0
#define N 10

#define NRAYS 200       //Nombre de rayons
#define map_scale 2.5

//Paramètres d'affichage
#define HM (N+2)*64   //taille réelle du mur
#define DE 70   //distance de l'écran
#define LARGEUR WIDTH /NRAYS
// #define LARGEUR (WIDTH - (N-1)*CASE_SIZE)/NRAYS
#define CENTRE N*CASE_SIZE/2

//SDL graphics structure
typedef struct Graphic{
    SDL_Window *window;
    SDL_Renderer *renderer;
}graphic;

/*-------------------- Prototypes des fonctions ---------------------- */

//Initialisations
void init(graphic *G);

//Affichage
void affiche_case(int x,int y,int tab[][N],graphic *G);
void affiche_map(int tab[][N],graphic *G);

//Raffraîchissement de l'affichage
void Display(graphic *G);
void fond_blanc(graphic *G);

//Affichage du joueur
void Dessine_joueur(player *p,graphic *G);

//Affichage des murs
void Dessine_colonne(graphic *G,int pos,int he,int l);

//Libération
void Free_graphic(graphic *G);

#endif