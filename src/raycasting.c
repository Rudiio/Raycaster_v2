#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <omp.h>

#include "raycasting.h"

/*------------------------ GESTION DU JOUEUR -------------------------*/
void init_player(player *p)
{
    /*Initialise les paramètres du joueur */
    p->x = CASE_SIZE*1.5;
    p->y = CASE_SIZE*1.5;
    p->angle = PI/2; 
}

int input(player *p,int tab[][N])
{
    /* Gère les input du jeu */
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT :
                return -1;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                        UP(p,tab);
                        return PLAYER_MOVE;

                    case SDLK_DOWN:
                        DOWN(p,tab);
                        return PLAYER_MOVE;

                    case SDLK_LEFT:
                        p->angle = angle_trigo(p->angle - DANGLE);
                        return PLAYER_MOVE;

                    case SDLK_RIGHT:
                        p->angle = angle_trigo(p->angle + DANGLE);
                        return PLAYER_MOVE;
                    
                    case SDLK_m:
                        return MAP;
                        
                    case SDLK_ESCAPE:
                        return -1;

                    
                } 
        }
    }
    return 0;
}

int input_v2(key *keys)
{
    /* Vérifie si un bouton est préssé */
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT :
                return -1;

            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                        keys->up=1;
                        return PLAYER_MOVE;

                    case SDLK_DOWN:
                        keys->down=1;
                        return PLAYER_MOVE;

                    case SDLK_LEFT:
                        keys->left=1;
                        return PLAYER_MOVE;

                    case SDLK_RIGHT:
                        keys->right=1;
                        return PLAYER_MOVE;
                    
                    case SDLK_m:
                        return MAP;
                        
                    case SDLK_ESCAPE:
                        return -1;
                }
            case SDL_KEYUP:
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                        keys->up=0;
                        return PLAYER_MOVE;

                    case SDLK_DOWN:
                        keys->down=0;
                        return PLAYER_MOVE;

                    case SDLK_LEFT:
                        keys->left=0;
                        return PLAYER_MOVE;

                    case SDLK_RIGHT:
                        keys->right=0;
                        return PLAYER_MOVE;
                } 
                    
        }
    }
    return 0;
}

void Player_movement(player *p,key *key,int tab[][N])
{
    /* gère les mouvements du joueur */
    if(key->up)
        UP(p,tab);
    if(key->down)
        DOWN(p,tab);
    if(key->left)
        p->angle = angle_trigo(p->angle - DANGLE);
    if(key->right)
        p->angle = angle_trigo(p->angle + DANGLE);

}

void UP(player *p,int tab[][N])
{
    /* Fait avancer le joueur dans sa direction actuelle */
    float tx=p->x +MV*cos(p->angle);
    float ty=p->y +MV*sin(p->angle);
    int x=(int)(tx/CASE_SIZE);
    int y=(int)(ty/CASE_SIZE);

    if(tab[y][x]!=1){
        p->x=tx;
        p->y=ty;
    }
}

void DOWN(player *p,int tab[][N])
{
    /* Fait reculer le joueur dans sa direction actuelle */
    float tx=p->x -MV*cos(p->angle);
    float ty=p->y -MV*sin(p->angle);
    int x=(int)(tx/CASE_SIZE);
    int y=(int)(ty/CASE_SIZE);

    if(tab[y][x]!=1){
        p->x=tx;
        p->y=ty;
    }
}

/*---------------------------- RAYCASTING --------------------------*/

void raycasting(player *p,graphic *G, int tab[][N],int dist[],textures *T)
{
    /* Calcule les distances jusqu'aux murs et les stocke dans un tableau.
    Gère aussi l'affichage 3D */    
    float d_angle = FOV/NRAYS;        //variation d'angle
    float theta = angle_trigo(p->angle - FOV/2);   //angle de direction du rayon
    
    #pragma omp parallel for 
    for(int i=0;i<NRAYS;i++){
        theta = angle_trigo(theta+d_angle); 
        int d = raydist(p,tab,i,theta,G,T,dist);     //Calcule de la distance et dessine les murs
        dist[i] = d;
    }
}

int raydist(player *p,int tab[][N],int pos,float theta,graphic *G,textures *T,int dist[])
{
    /* Calcule la distance du joueur au prochain mur dans une certaine direcction */

    float cx, cy, dx,dy;
    int i=0;
    float atan = -1/tan(theta);

    //Ligne horizontale
    //Détermination de l'intersection la plus proche
    if(theta < PI){
        cy = ((int)p->y/CASE_SIZE + 1) *CASE_SIZE;   //Vers le bas
        cx = ((int)p->y - cy)*atan + p->x;
        dy = CASE_SIZE;
        dx = -dy*atan;

        //Recherche du mur
        while(i<N && check_tab((int)cy/CASE_SIZE,(int)cx/CASE_SIZE)  && tab[(int) cy/CASE_SIZE][(int)cx/CASE_SIZE]!=1 ){
            cy += dy ;
            cx += dx;
            i++;
        }
    }
    else if(PI < theta ){   //vers le haut
        cy = ((int)p->y/CASE_SIZE) *CASE_SIZE;
        cx = ((int)p->y - cy)*atan + p->x;
        dy = -CASE_SIZE;
        dx = -dy*atan;

        //Recherche du mur
        while(i<N && check_tab((int)cy/CASE_SIZE -1,(int)cx/CASE_SIZE) && tab[(int)cy/CASE_SIZE -1][(int)cx/CASE_SIZE]!=1){
            cy += dy ;
            cx += dx;
            i++;
        }
    }
    else if( val_abs(theta)<0.05 || val_abs(theta - PI)<0.05){    //cas limites
        cx = p->x;
        cy = p->y;
    }

    int hx = cx;
    int hy = cy;

    //Ligne verticale
    i=0;
    float ntan = -tan(theta);
    
    //Détermination de l'intersection la plus proche
    if( val_abs(theta - PI/2) <0.00001 || val_abs(theta-3*PI/2)<0.00001){   //cas limites
        cx = p->x;
        cy = p->y;

        //Recherche du mur
        while(i<N && check_tab((int)cy/CASE_SIZE,(int)cx/CASE_SIZE) && tab[(int)cy/CASE_SIZE][(int)cx/CASE_SIZE]!=1){
            cy += dy ;
            cx += dx;
            i++;
        }
    }

    else if((0< theta && theta < PI/2) || (3*PI/2 < theta && theta < 2*PI)){    //vers la droite
        cx = ((int)p->x/CASE_SIZE +1) *CASE_SIZE;
        cy = ((int)p->x - cx)*ntan + p->y;
        dx = CASE_SIZE;
        dy = -dx*ntan;

        //Recherche du mur
        while(i<N && check_tab((int)cy/CASE_SIZE,cx/CASE_SIZE) && tab[(int)cy/CASE_SIZE][(int)cx/CASE_SIZE]!=1){
            
            cy += dy ;
            cx += dx;
            i++;
        }
        
    }

    else if(theta > PI/2 && theta < 3*PI/2){    //vers la gauche
        cx = ((int)p->x/CASE_SIZE) *CASE_SIZE;
        cy = ((int)p->x - cx)*ntan + p->y;
        dx = -CASE_SIZE;
        dy = -dx*ntan;

        //Recherche du mur
        while(i<N && check_tab((int)cy/CASE_SIZE,(int)cx/CASE_SIZE-1) && tab[(int)cy/CASE_SIZE][(int)cx/CASE_SIZE-1]!=1){
            cy += dy ;
            cx += dx;
            i++;
        }
    }

    int he;
    int l;

    float rh = sqrt((hx-p->x)*(hx-p->x) + (hy-p->y) * (hy-p->y)); //Distance horizontale
    float rv = sqrt((cx-p->x)*(cx-p->x) + (cy-p->y)*(cy-p->y)); //Distance verticale
    float x;
    float y;
    int decalage;

    //On renvoie la distance la plus courte
    if(rh < rv){    //distance horizontale

        //harmonisation des coins
        // if(pos!=0){
        //     if(rh - dist[pos-1]>=CASE_SIZE){
        //         rh = dist[pos]-1;
        //     }
        // }

        SDL_SetRenderDrawColor(G->renderer, 0,255*0.5, 0, 255);     
        // SDL_RenderDrawLine(G->renderer,p->x/map_scale,p->y/map_scale,hx/map_scale,hy/map_scale);         //Affichage des rayons


        rh *= cos(angle_trigo(theta -p->angle + PI/16));    //Correction du fisheye
        he = HM*DE/rh;
        l=LARGEUR; 

        //Calculs de x et y
        x = rh *cos(theta) + p->x;
        y = rh *sin(theta) + p->y;

        //décalage sur la texture
        decalage =(int) x%CASE_SIZE;

        // Dessine_colonne(G,pos,he,l);     //sans texture
        Dessine_colonne_texture(G,T,decalage,pos,he,l,0); //Avec texture

        //texturation du sol
        // Dessine_sol_texture(G,T,p,LARGEUR*pos,CENTRE + he/2 +1,theta);

        return rh;  
    }
    
    //distance verticale

    //harmonisation des coins
    // if(pos!=0){
    //     if(val_abs(rv - dist[pos-1])>=2*CASE_SIZE){
    //         rv = dist[pos-1]-1;
    //     }
    // }

    SDL_SetRenderDrawColor(G->renderer, 0,200, 0, 255); 

    // SDL_RenderDrawLine(G->renderer,p->x/map_scale,p->y/map_scale,cx/map_scale,cy/map_scale);     //Affichage des rayons


    rv *= cos(angle_trigo(theta - p->angle + PI/16));   //Correction du fisheye
    he = HM*DE/rv;
    l=LARGEUR;

    //Calculs de x et y
    x = rv *cos(theta) + p->x;
    y = rv *sin(theta) + p->y;

    //décalage sur la texture
    decalage =(int) y%CASE_SIZE;

    // Dessine_colonne(G,pos,he,l); //Sans texture
    Dessine_colonne_texture(G,T,decalage,pos,he,l,1); //avec texture

    //texturation du sol
    // Dessine_sol_texture(G,T,p,LARGEUR*pos,CENTRE + he/2,theta);

    return rv;
}

/*------------------------- Fonctions auxilliaires ------------------------*/

float angle_trigo(float angle)
{
    /*Permet de ne pas avoir de valeur d'angle négative ou de dépasser 2PI*/
    if(angle<0)
        return angle +2*PI;
    if(angle>2*PI)
        return angle -2*PI;
    
    return angle;
}

int check_tab(int x,int y)
{
    /* Renvoie 1 si la case est dans le tableau, renvoie 0 sinon */

    if( (0<=x && x<N) && (0<=y && y<=N))
        return 1;
    return 0;
}
