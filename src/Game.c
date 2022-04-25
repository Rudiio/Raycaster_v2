#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "Graphic.h"
#include "Game.h"
#include "raycasting.h"

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

void mainloop()
{
    /* Boucle de jeu principale */

    //Paramètres de jeu
    graphic G;
    player p;
    int etat=1;
    int dist[NRAYS];

    //Initialisation
    init(&G);
    init_player(&p);
    affiche_map(map,&G);
    Dessine_joueur(&p,&G);
    Display(&G);

    while(etat!=-1){
    etat = input(&p,map);
    fond_blanc(&G);
    affiche_map(map,&G);
    Dessine_joueur(&p,&G);
    raycasting(&p,&G,map,dist);
    // raydist(&p,map,p.angle,&G);
    Display(&G);
    }

    Free_graphic(&G);
    SDL_Quit();
}