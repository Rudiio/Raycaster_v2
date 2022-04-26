#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

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

void UP(player *p,int tab[][N])
{
    /* Fait avancer le joueur dans sa direction actuelle */
    int tx=p->x +MV*cos(p->angle);
    int ty=p->y +MV*sin(p->angle);
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
    int tx=p->x -MV*cos(p->angle);
    int ty=p->y -MV*sin(p->angle);
    int x=(int)(tx/CASE_SIZE);
    int y=(int)(ty/CASE_SIZE);

    if(tab[y][x]!=1){
        p->x=tx;
        p->y=ty;
    }
}

/*---------------------------- RAYCASTING --------------------------*/

void raycasting(player *p,graphic *G, int tab[][N],int dist[])
{
    /* Calcule les distances jusqu'aux murs et les stocke dans un tableau.
    Gère aussi l'affichage 3D */    
    float d_angle = FOV/NRAYS;        //variation d'angle
    float theta = angle_trigo(p->angle - FOV/2);   //angle de direction du rayon
    
    for(int i=0;i<NRAYS;i++){
        theta = angle_trigo(theta+d_angle); 
        int d = raydist(p,tab,i,theta,G);     //Calcule de la distance et dessine les murs
        dist[i] = d;
    }
}

int raydist(player *p,int tab[][N],int pos,float theta,graphic *G)
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
    
    //On renvoie la distance la plus courte
    if(rh < rv){    //distance horizontale

        SDL_SetRenderDrawColor(G->renderer, 0,255*0.5, 0, 255);     
        // SDL_RenderDrawLine(G->renderer,p->x,p->y,hx,hy);         //Affichage des rayons
        
        rh *= cos(angle_trigo(theta -p->angle + PI/16));    //Correction du fisheye
        he = HM*DE/rh;
        l=LARGEUR; 
        Dessine_colonne(G,pos,he,l);
        return rh;
    }
    
    //distance verticale

    SDL_SetRenderDrawColor(G->renderer, 0,200, 0, 255); 

    // SDL_RenderDrawLine(G->renderer,p->x,p->y,cx,cy);     //Affichage des rayons
    
    rv *= cos(angle_trigo(theta - p->angle + PI/16));   //Correction du fisheye
    he = HM*DE/rv;
    l=LARGEUR;

    Dessine_colonne(G,pos,he,l);
    
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

float val_abs(float val)
{
    /* Fonction valeur absolue pour les flottants*/
    if(val < 0)
        return -val;
    return val;
}

int check_tab(int x,int y)
{
    /* Renvoie 1 si la case est dans le tableau, renvoie 0 sinon */

    if( (0<=x && x<N) && (0<=y && y<=N))
        return 1;
    return 0;
}
