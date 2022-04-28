#ifndef PLAYER_H
#define PLAYER_H

/*----------------------- CONSTANTES -----------------------*/

//Pour le joueur
#define PLAYER 20
#define MV 5
#define DANGLE 0.1//2*PI/30

//Pour les events
#define PLAYER_MOVE 1
#define MAP 2

//Taille de la map
#define CASE_SIZE 64

//Paramètres angulaires
#define PI 3.14
#define FOV 50*PI/180        //Field of view



//player structure
typedef struct Player{
    float x;
    float y;
    float angle;
}player;

//Event structure
typedef struct Event{
    int map;
}event;

//Keyboard structure
typedef struct Key
{
    int up;
    int down;
    int left;
    int right;

}key;










#endif