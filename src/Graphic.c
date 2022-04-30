#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#include "Graphic.h"

/*-------------------------------- INITIALISATION -----------------------------------*/
graphic *init_graphic()
{
    /*initialise la structure graphic */

    graphic *G =(graphic*)malloc(sizeof(graphic));
    G->renderer=NULL;
    G->renderer=NULL;

    return G;
}

void init_ttf()
{
    /* Initialise la bibliothèque SDL_ttf pour afficher du texte */
    if (TTF_Init() == -1) {
	printf("Error SDL2_ttf Initialization\n");
    }
}

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
	G->renderer= SDL_CreateRenderer(G->window,-1,SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    //Vérification de la création du rendu
	if(G->renderer==NULL){
		SDL_Log("ERREUR: Creation de fenetre echouee > %s\n",SDL_GetError());
		exit(EXIT_FAILURE);
	}

    //Coloriage du fond
	SDL_SetRenderDrawColor(G->renderer, 255, 255, 255, 255); 
	SDL_RenderClear(G->renderer);
    // SDL_RenderPresent(G->renderer);
}

/* ------------------------------ AFFICHAGE DU JEU ----------------------------------*/

void affiche_case(int x,int y,int tab[][N],graphic *G)
{
    /* Affiche un élément du décor en 2D sous la forme d'un carré */
    SDL_Rect rect = {DX + x*CASE_SIZE/(map_scale),DY + y*CASE_SIZE/(map_scale),CASE_SIZE/(map_scale)+1,CASE_SIZE/(map_scale)+1};

    //Affichage d'un mur
    if(tab[y][x]==1){
        SDL_SetRenderDrawColor(G->renderer,0,0,0,255);
        SDL_RenderFillRect(G->renderer,&rect);
    }
    if(tab[y][x]==0){
        SDL_SetRenderDrawColor(G->renderer,255,255,255,255);
        SDL_RenderFillRect(G->renderer,&rect);
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

    SDL_Rect rect = {(p->x -PLAYER/2)/(map_scale),(p->y -PLAYER/2)/(map_scale),PLAYER/(2*map_scale),PLAYER/(map_scale)};
    SDL_Rect rect2 = {p->x/(map_scale),(p->y -PLAYER/2)/(map_scale),PLAYER/(2*map_scale),PLAYER/(map_scale)};

    SDL_SetRenderDrawColor(G->renderer,0,215,0,255);
    SDL_RenderFillRect(G->renderer,&rect);
    SDL_RenderFillRect(G->renderer,&rect2);
    SDL_RenderDrawLine(G->renderer,(int)p->x/(map_scale),(int)p->y/(map_scale),(int)(50*cos(p->angle)+ p->x)/(map_scale),(int)(50*sin(p->angle) + p->y)/(map_scale));

}

void Dessine_colonne(graphic *G,int pos,int he,int l)
{
    /* Dessine une colonne de mur */

    SDL_Rect Rect = {pos*LARGEUR, CENTRE - he/2,l+1,he};
    // SDL_Rect Rect = {pos*LARGEUR + (N+1)*CASE_SIZE, CENTRE - he/2,l+1,he};
    SDL_RenderFillRect(G->renderer,&Rect);
}

void Dessine_colonne_texture(graphic *G,textures* T,int decalage,int pos,int he,int l,int mur)
{
    /* Dessine une colonne de mur texturée */
    // int large = l;
    // printf("decalage = %d\n",large*large/CASE_SIZE);
    if(mur){
        SDL_Rect On_texture = {decalage*l/Texture_scale,0,l/Texture_scale,CASE_SIZE };
        SDL_Rect On_render = {pos*l, CENTRE - he/2,l,he};
        SDL_RenderCopy(G->renderer, T->Wall_shade,&On_texture, &On_render);
    }
    else{
        SDL_Rect On_texture = {decalage*l/Texture_scale,0,l/Texture_scale,CASE_SIZE };
        SDL_Rect On_render = {pos*l, CENTRE - he/2,l+2,he};
        SDL_RenderCopy(G->renderer, T->Wall,&On_texture, &On_render);
    }
}

void Dessine_sol_texture(graphic *G,textures* T,player *p,int proj_x,int proj_y,float theta)
{
    /*Dessine les textures du sol */

    #pragma omp parallel for
    for(int i=(int)proj_y;i<HEIGTH;i++){

        float r = HM/2 - i ;
        float ds = HM/2*DE/r;
        float beta = theta - p->angle;
        float d = ds/cos(beta);

        float fx = p->x + cos(theta)*d;
        float fy =p->y + sin(theta)*d ;

        // printf("px = %d py = %d \n",(int)fx % CASE_SIZE,(int)fy % CASE_SIZE);
        
        //Affichage du pixel
        SDL_Rect On_texture = {(int)fx %CASE_SIZE,(int)fy % CASE_SIZE,1,1};
        SDL_Rect On_render = {(int)proj_x,i,2,2};
        SDL_RenderCopy(G->renderer, T->Floor,&On_texture, &On_render);
    }
}

void background(graphic *G)
{
    /*Affiche le ciel et le le sol de couleurs différentes */
    //déclaration des variables
    SDL_Rect rect1={0,0,WIDTH,CENTRE};
    SDL_Rect rect2={0,CENTRE,WIDTH,HEIGTH};

    //On affiche le ciel
    SDL_SetRenderDrawColor(G->renderer, 135, 206, 235, 255); //en bleu
    SDL_RenderFillRect(G->renderer,&rect1);
    
    //On affiche le ciel
    SDL_SetRenderDrawColor(G->renderer, 128, 128, 128, 255); //en bleu
    SDL_RenderFillRect(G->renderer,&rect2);

    // int number_of_row = 10;
    // int x = 0;
    // int y = HEIGTH;

    // for(int i=0;i<number_of_row;i++){
    //     SDL_Rect rect2={x,y,WIDTH,(HEIGTH-CENTRE)/number_of_row};

    //     //On affiche le sol
    //     SDL_SetRenderDrawColor(G->renderer, 192 -5*i, 192 -5*i, 192 -5*i, 255); //en bleu
    //     SDL_RenderFillRect(G->renderer,&rect2);
    //     y-= (HEIGTH-CENTRE)/number_of_row;
    // }


}

/* ----------------------- SPRITES --------------------------------------------*/

void Dessine_sprite(entity *en,player *p,graphic *G,int depht[])
{
    /*Dessine un sprite à l'écran */

    //On va vérifier si le sprite est visible
    float dx = en->x - p->x;
    float dy = en->y - p->y;
    float cs = cos(2*PI - p->angle);
    float sn = sin(2*PI - p->angle);

    float a = cs*dy +sn*dx;
    float b = dx*cs-dy*sn;

    dx = a;
    dy = b;

    dx= (dx*50/dy) + WIDTH/2;
    dy= (en->z*50/dy) +3*HEIGTH/4;

    printf("dx=%d dy =%d \n",(int)dx,(int)dy);
    SDL_Rect rect={dx,dy,5,5};
    SDL_SetRenderDrawColor(G->renderer,0,0,255,0);
    SDL_RenderFillRect(G->renderer,&rect);
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
    free(G);
}

/* -------------------------------- CHARGEMENT DES TEXTURES ----------------------------------- */

int charge_textures(textures *T,graphic *G)
{
    /* Charge les textures du jeu */

    //Pour les murs horizontaux
    T->Wall = IMG_LoadTexture(G->renderer,"./textures/Wall.png");
    if(T->Wall==NULL){
        SDL_Log("ERREUR: Creation de tmp echouee > %s\n",SDL_GetError());
	    exit(EXIT_FAILURE);
    }   

    //Pour les murs verticaux
    T->Wall_shade = IMG_LoadTexture(G->renderer,"./textures/Wall.png");
    if(T->Wall_shade==NULL){
        SDL_Log("ERREUR: Creation de tmp echouee > %s\n",SDL_GetError());
	    exit(EXIT_FAILURE);
    }   

    if(SDL_SetTextureColorMod(T->Wall_shade,175,175,175)<0){
        SDL_Log("ERREUR: Shading de la texture %s\n",SDL_GetError());
	    exit(EXIT_FAILURE);
    }

    // Pour le sol
    T->Floor = IMG_LoadTexture(G->renderer,"./textures/floor.png");
    if(!T->Floor){
        printf("Erreur lors du chargement de la texture du sol\n");
        return 0;
    }

    // Pour le sol
    T->sprite1 = IMG_LoadTexture(G->renderer,"./textures/pikachu.png");
    if(!T->Floor){
        printf("Erreur lors du chargement du sprite\n");
        return 0;
    }

    // //pour le plafond
    // T->ceiling = IMG_LoadTexture(G->renderer,"../textures/ceiling.png");
    // if(!T->ceiling){
    //     printf("Erreur lors du chargement de la texture du plafond\n");
    //     return 0;
    // }
    
    return 1;
}

/*-------------------------------- TTFF$ -----------------------------------------------*/

TTF_Font *charge_font(char * path,int size)
{
    /* Charge une font .ttf avec la taille de police size*/

    TTF_Font *font = NULL;
    font = TTF_OpenFont(path,size);
    if(!font)
    {
        SDL_Log("ERREUR: Chargement font > %s\n",SDL_GetError());
	    exit(EXIT_FAILURE);
    }
    return font;
}

void Diplay_fps(graphic *G,char *fps,TTF_Font *font)
{
    /* Affiche les FPS sur l'écran */

    //Chargement du texte dans une surface
    SDL_Color color = { 0, 0, 0 };
    SDL_Surface * surface = TTF_RenderText_Solid(font,fps, color);

    //Conversion de la surface en texture
    SDL_Texture * texture = SDL_CreateTextureFromSurface(G->renderer, surface);

    //affichage de la texture
    SDL_Rect On_render = {WIDTH - 150, 0,100,100};
    SDL_RenderCopy(G->renderer, texture,NULL, &On_render);

    //Libération de la mémoire
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

/* -------------------------------- MATHS AUX ----------------------------------- */

float val_abs(float val)
{
    /* Fonction valeur absolue pour les flottants*/
    if(val < 0)
        return -val;
    return val;
}