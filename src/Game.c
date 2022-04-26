#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#include "Graphic.h"
#include "Game.h"
#include "textures.h"

//GAME MAP
int map[10][10]= {{1,1,1,1,1,1,1,1,1,1},
            {1,0,1,0,0,0,0,0,0,1},
            {1,0,1,0,1,1,1,0,0,1},
            {1,0,1,0,1,0,0,0,0,1},
            {1,0,1,0,1,0,0,0,0,1},
            {1,0,1,0,1,1,0,0,0,1},
            {1,0,0,0,0,1,1,0,0,1},
            {1,0,0,0,0,1,1,0,0,1},
            {1,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1}};

event* init_event()
{
    /* Initialise la structure d'evenements */
    event *e= (event*)malloc(sizeof(event));
    e->map=0;

    return e;
}

void event_update(event *e,int etat)
{
    /* Met à jour les évenements selon les inputs */
    switch (etat)
    {
    case MAP:
        if(e->map)
            e->map =0;
        else
            e->map=1;
        break;
    
    default:
        break;
    }
}

void event_handling(event* e,graphic *G,player *p,int tab[][N])
{
    /* Prends en charge les paramètres d'évènement */

    if(e->map){
        affiche_map(tab,G);       //affichage de la mini-map
        Dessine_joueur(p,G);     //affichage du joueur sur la mini-map
    }

}


void mainloop()
{
    /* Boucle de jeu principale */

    //Paramètres de jeu
    graphic *G;
    player p;
    int etat=1;
    int dist[NRAYS];
    event *e;
    textures *T;

    //Initialisations
    G = init_graphic();
    init(G);
    init_image();
    e=init_event();

    //Structures
    init_player(&p);
    T=init_textures();
    charge_textures(T,G);

    //Boucle Principale
    while(etat!=-1){
    etat = input(&p,map);   //Prise en charge des event

    event_update(e,etat);  //Mise à jour des évenements

    fond_blanc(G);     //Nettoyage de l'affichage

    //--------------------- RAYCASTING ------------------------ //

    raycasting(&p,G,map,dist,T);     //calul des distances et affichage des murs
    //--------------------- TABLEAU DE BORD ---------------------//

    event_handling(e,G,&p,map);
    // SDL_Rect dst = {0, 0,64,64 };
    // SDL_RenderCopy(G->renderer, T->Wall, NULL, &dst);

    //--------------------- MISE A JOUR DE L'AFFICHAGE ------------//
    Display(G);

    }

    free(e);
    free_textures(T);
    Free_graphic(G);
    IMG_Quit();
    SDL_Quit();
}