#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "Graphic.h"

/*-------------------------------- INITIALISATION -----------------------------------*/

void init(graphic *G)
{
    /*Initialise la SDL ainsi que la window et le renderer*/

    if(SDL_Init(SDL_INIT_VIDEO)==-1){
        SDL_Log("ERREUR: Initialisation echouee> %s\n",SDL_GetError()); //Affichage de l'erreur
		exit(EXIT_FAILURE);		//On quitte le programme
    }

    //taille de la fenêtre
	int L=WIDTH;
	int l=HEIGTH;
	G->window=SDL_CreateWindow("RAYCASTING",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED ,L,l,SDL_WINDOW_RESIZABLE);

    //Vérification de la création de la fenêtre
	if(G->window == NULL){
		SDL_Log("ERREUR: Creation de fenetre echouee > %s\n",SDL_GetError());
		exit(EXIT_FAILURE);
	}

    //Création du rendu
	G->renderer= SDL_CreateRenderer(G->window,-1,SDL_RENDERER_ACCELERATED);

    //Vérification de la création du rendu
	if(G->renderer==NULL){
		SDL_Log("ERREUR: Creation de fenetre echouee > %s\n",SDL_GetError());
		exit(EXIT_FAILURE);
	}

    //Coloriage du fond
	SDL_SetRenderDrawColor(G->renderer, 255, 255, 255, 255); 
	SDL_RenderClear(G->renderer);
    SDL_RenderPresent(G->renderer);
}

/* ------------------------------ AFFICHAGE DU JEU ----------------------------------*/

void affiche_case(int x,int y,int tab[][N],graphic *G)
{
    /* Affiche un élément du décor en 2D sous la forme d'un carré */
    SDL_Rect rect = {DX + x*CASE_SIZE,DY + y*CASE_SIZE,CASE_SIZE,CASE_SIZE};

    //Affichage d'un mur
    if(tab[y][x]==1){
        SDL_SetRenderDrawColor(G->renderer,0,0,0,255);
        SDL_RenderFillRect(G->renderer,&rect);
    }

    //Affichage d'une case vide
    if(tab[y][x]==0){
        SDL_SetRenderDrawColor(G->renderer,0,0,0,255);
        SDL_RenderDrawRect(G->renderer,&rect);
    }
}

void affiche_map(int tab[][N],graphic *G)
{
    /*Affiche la map */
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            affiche_case(j,i,tab,G);
        }
    }
}

void Dessine_joueur(player *p,graphic *G)
{
    /*dessine  le joueur sur la fenêtre sous la forme d'un carré */

    SDL_Rect rect = {p->x -PLAYER/2,p->y -PLAYER/2,PLAYER/2,PLAYER};
    SDL_Rect rect2 = {p->x,p->y -PLAYER/2,PLAYER/2,PLAYER};

    SDL_SetRenderDrawColor(G->renderer,255,215,0,255);
    SDL_RenderFillRect(G->renderer,&rect);
    SDL_RenderFillRect(G->renderer,&rect2);

}

void Dessine_colonne(graphic *G,int pos,int he,int l)
{
    /* Dessine une colonne de mur */

    SDL_Rect Rect = {pos*LARGEUR + (N+1)*CASE_SIZE, CENTRE - he/2,l+1,he};
    SDL_RenderFillRect(G->renderer,&Rect);
}

/* -----------------------RAFFRAICHISSEMENT DE L'AFFICHAGE -------------------- */

void fond_blanc(graphic *G)
{
    /*Colorie le renderer en blanc*/
    
	SDL_SetRenderDrawColor(G->renderer, 255, 255, 250, 255); 
	SDL_RenderClear(G->renderer);
}

void Display(graphic *G)
{
    /* Met à jour l'affichage */
    SDL_RenderPresent(G->renderer);

}

/* -------------------------------- LIBERATION ----------------------------------- */

void Free_graphic(graphic *G)
{
    /* Detruit la window et le renderer */

    SDL_DestroyRenderer(G->renderer);
    SDL_DestroyWindow(G->window);

}