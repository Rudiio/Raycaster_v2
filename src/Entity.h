#ifndef ENTITY_H
#define ENTITY_H


//entity structure
typedef struct 
{
    int type;
    float x;
    float y;
    float z;

}entity;

entity *init_entity(float x,float y,float z,int type);






#endif