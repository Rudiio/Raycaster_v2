#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#include "Graphic.h"
#include "Game.h"
#include "textures.h"
#include "Entity.h"

//GAME MAP
// int map[10][10]= {{1,1,1,1,1,1,1,1,1,1},
//             {1,0,1,0,0,0,0,0,0,1},
//             {1,0,1,0,1,1,1,0,0,1},
//             {1,0,1,0,1,0,0,0,0,1},
//             {1,0,1,0,1,0,0,0,0,1},
//             {1,0,1,0,1,1,0,0,0,1},
//             {1,0,0,0,0,0,0,0,0,1},
//             {1,0,0,0,0,1,1,0,0,1},
//             {1,0,0,0,0,0,0,0,0,1},
//             {1,1,1,1,1,1,1,1,1,1}};


void map_generator(int map[][N])
{
    /* génere une carte aléatoire */

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(i==0 || j==0 || j==N-1 || i==N-1)
                map[i][j]=1;
            else
                map[i][j]=0;
        }
    }
    int i=0;

    while(i<N/2){
        int x = rand()%N;
        int y = rand()%N;

        if(map[x][y]==0){
            map[x][y]=1;
            i++;
        }
        
    }

}


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

void init_keys(key *k)
{
    k->up=0;
    k->down=0;
    k->left=0;
    k->right=0;
}

void mainloop()
{
    /* Boucle de jeu principale */

    //Paramètres de jeu

    int map[N][N];
    graphic *G;
    player p;
    int etat=1;
    int dist[NRAYS];
    event *e;
    textures *T;
    entity *pikachu;
    key keys;

    //Initialisations
    map_generator(map);
    G = init_graphic();
    init(G);
    init_image();
    e=init_event();
    init_keys(&keys);

    //Structures
    init_player(&p);
    T=init_textures();
    charge_textures(T,G);
    pikachu = init_entity(2.5*CASE_SIZE,7.5*CASE_SIZE,0,1);

    //Boucle Principale
    while(etat!=-1){
        Uint64 start = SDL_GetPerformanceCounter();
        etat = input_v2(&keys);
        Player_movement(&p,&keys,map);
        // etat = input(&p,map);   //Prise en charge des event

        event_update(e,etat);  //Mise à jour des évenements

        // fond_blanc(G);     //Nettoyage de l'affichage
        background(G);
        //--------------------- RAYCASTING ------------------------ //

        raycasting(&p,G,map,dist,T);     //calul des distances et affichage des murs
        //--------------------- TABLEAU DE BORD ---------------------//

        // Dessine_sprite(pikachu,&p,G,dist);

        event_handling(e,G,&p,map);

        //--------------------- MISE A JOUR DE L'AFFICHAGE ------------//
        Display(G);

        Uint64 end = SDL_GetPerformanceCounter();
        float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
        printf("Current fps = %f\n",1.0f/elapsed);
    }

    free(pikachu);
    free(e);
    free_textures(T);
    Free_graphic(G);
    IMG_Quit();
    SDL_Quit();
}