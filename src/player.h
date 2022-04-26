#ifndef PLAYER_H
#define PLAYER_H

/*----------------------- CONSTANTES -----------------------*/

//Pour le joueur
#define PLAYER 20
#define MV 10
#define DANGLE 0.1//2*PI/30

//Pour les events
#define PLAYER_MOVE 1
#define MAP 2




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









#endif