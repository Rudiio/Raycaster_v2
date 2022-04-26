#ifndef TEXTURES_H
#define TEXTURES_H

typedef struct Textures{
    SDL_Texture *Wall;
    SDL_Texture *Floor;
    SDL_Texture *ceiling;
}textures;


void init_image();
textures *init_textures();
void free_textures(textures *T);









#endif