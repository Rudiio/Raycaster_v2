#include <stdio.h>
#include <stdlib.h>
#include "Entity.h"


entity *init_entity(float x,float y,float z,int type)
{
    /*Renvoie une entité */

    entity *en =(entity*)malloc(sizeof(entity));
    en->x=x;
    en->y=y;
    en->z = z;
    en->type=type;

    return en;
}


