#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL2/SDL.h>
#include "player.h"
#include "textures.h"
#include "Entity.h"
/*----------------------- CONSTANTES -----------------------*/

//Define pour l'affichage
#define WIDTH 1000
#define HEIGTH 720
#define CASE_SIZE 64
#define DX 0
#define DY 0
#define N 20

#define NRAYS 500      //Nombre de rayons
#define map_scale N/4
#define Texture_scale 2

//Paramètres d'affichage
#define HM 1.5*64   //taille réelle du mur        //*2 pour avoir des carrés sinon des rectangles
#define DE (10)*CASE_SIZE   //distance de l'écran
#define LARGEUR WIDTH/NRAYS
// #define LARGEUR (WIDTH - (N-1)*CASE_SIZE)/NRAYS
#define CENTRE 10*CASE_SIZE/2

//SDL graphics structure
typedef struct Graphic{
    SDL_Window *window;
    SDL_Renderer *renderer;
}graphic;

/*-------------------- Prototypes des fonctions ---------------------- */

//Initialisations
graphic *init_graphic();
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
void Dessine_colonne_texture(graphic *G,textures* T,int decalage,int pos,int he,int l,int mur);

//affichage du sol
void Dessine_sol_texture(graphic *G,textures* T,player *p,int proj_x,int proj_y,float theta);
void background(graphic *G);


//sprites
void Dessine_sprite(entity *en,player *p,graphic *G,int depht[]);

//textures
int charge_textures(textures *T,graphic *G);

//Libération
void Free_graphic(graphic *G);

float val_abs(float val);

#endif