#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "Graphic.h"
#include "Game.h"


//GAME MAP
int map[10][10]= {{1,1,1,1,1,1,1,1,1,1},
            {1,0,1,0,0,0,0,0,0,1},
            {1,0,1,0,1,1,1,0,0,1},
            {1,0,1,0,1,0,0,0,0,0},
            {1,0,1,0,1,0,0,0,0,0},
            {1,0,1,0,1,1,0,0,0,1},
            {1,0,0,0,0,1,1,0,0,1},
            {1,0,0,0,0,1,1,0,0,1},
            {1,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1}};

void init_event(event *e)
{
    /* Initialise la structure d'evenements */
    e->map=0;
}

void event_update(event *e,int etat)
{
    /* Met à jour les évenements selon les inputs */
    switch (etat)
    {
    case MAP:
        e->map = !e->map;
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
    graphic G;
    player p;
    int etat=1;
    int dist[NRAYS];
    event e;

    //Initialisation
    init(&G);
    init_player(&p);
    affiche_map(map,&G);
    Dessine_joueur(&p,&G);
    Display(&G);

    while(etat!=-1){
    etat = input(&p,map);   //Prise en charge des event

    event_update(&e,etat);  //Mise à jour des évenements

    fond_blanc(&G);     //Nettoyage de l'affichage

    //--------------------- RAYCASTING ------------------------ //

    raycasting(&p,&G,map,dist);     //calul des distances et affichage des murs
    
    //--------------------- TABLEAU DE BORD ---------------------//

    event_handling(&e,&G,&p,map);

    //--------------------- MISE A JOUR DE L'AFFICHAGE ------------//
    Display(&G);
    }

    Free_graphic(&G);
    SDL_Quit();
}