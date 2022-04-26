#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "textures.h"

void init_image()
{
    /* Initialise la library SDL_image */

    if (IMG_Init(IMG_INIT_PNG) == 0) {
	printf("Error SDL2_image Initialization\n");
    }

}

textures *init_textures()
{
    /* Initialise la structure contenant les textures */

    textures *T= (textures*)malloc(sizeof(textures));
    T->Wall=NULL;
    T->Floor=NULL;
    T->ceiling=NULL;

    return T;
}

// int charge_textures(textures *T,graphic *G)
// {
//     /* Charge les textures du jeu */

//     //Pour les murs
//     T->Wall = IMG_LoadTexture(G->renderer,"./textures/Wall.png");
//     if(T->Wall==NULL){
//         SDL_Log("ERREUR: Creation de tmp echouee > %s\n",SDL_GetError());
// 	    exit(EXIT_FAILURE);
//     }   

//     // Pour le sol
//     // T->Floor = IMG_LoadTexture(G->renderer,"../textures/Floor.png");
//     // if(!T->Floor){
//     //     printf("Erreur lors du chargement de la texture du sol\n");
//     //     return 0;
//     // }

//     // //pour le plafond
//     // T->ceiling = IMG_LoadTexture(G->renderer,"../textures/ceiling.png");
//     // if(!T->ceiling){
//     //     printf("Erreur lors du chargement de la texture du plafond\n");
//     //     return 0;
//     // }
    
//     return 1;
// }

void free_textures(textures *T)
{
    /* Libère les textures du jeu */

    SDL_DestroyTexture(T->Wall);
    SDL_DestroyTexture(T->Floor);
    SDL_DestroyTexture(T->ceiling);
    free(T);

}