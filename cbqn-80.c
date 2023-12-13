#include <stdlib.h>
#include "../cbqn/include/bqnffi.h"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string.h>


#define WINDOW_WIDTH 240
#define WINDOW_HEIGHT 136

uint32_t c[]={0xff1a1c2c, 0xff5d275d, 0xffb13e53, 0xffef7d57, 0xffffcd75,
  0xffa7f070, 0xff38b764, 0xff257179, 0xff29366f, 0xff3b5dc9, 0xff41a6f6, 
  0xff73eff7, 0xfff4f4f4, 0xff94b0c2, 0xff566c86, 0xff333c5};

void save_texture(const char* file_name, SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_Texture* target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, texture);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    IMG_SavePNG(surface, file_name);
    SDL_FreeSurface(surface);
    SDL_SetRenderTarget(renderer, target);
}

int main(int argc, char* argv[]) {
  if(argc==1){puts("Usage: cbqn-80 file.bqn [framecount [name]]");
    exit(0);
  }
  FILE* f=fopen(argv[1],"r");
  char* b;
  if(f){
    fseek(f,0,SEEK_END);
    long l=ftell(f);
    fseek(f,0,SEEK_SET);
    b=malloc((l+1)*sizeof(char));
    if(b){fread(b,sizeof(char),l,f);b[l]=0;}
    fclose(f);
  }
  float m=30;
  bool e=argc>3;
  if(argc>2) m=atof(argv[2]);
   
  bqn_init();
  float k=0;
  size_t s[]={WINDOW_HEIGHT,WINDOW_WIDTH};
  int32_t t[WINDOW_WIDTH*WINDOW_HEIGHT]={0};
  BQNV x=bqn_makeI32Arr(2,s,t);
  BQNV w=bqn_makeF64(k);
  BQNV g=bqn_evalCStr(b);

  //sdl
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, 
    &renderer); 
  SDL_Texture *Screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

  int l=SDL_GetTicks();
  while(k<m){
//    if(!e){
      if(SDL_GetTicks()-l<33) continue; // desired fps is around 30.
      l=SDL_GetTicks();
  //  }
    x=bqn_call2(g,w,x);
    uint32_t* a=malloc(WINDOW_HEIGHT*WINDOW_WIDTH*sizeof(uint32_t)); 
    bqn_readI32Arr(x,a);
    for(int i=0;i<WINDOW_HEIGHT*WINDOW_WIDTH;i++){a[i]=c[a[i]&0x0000000f];}               
    SDL_RenderClear(renderer);
    SDL_UpdateTexture(Screen,NULL,a,WINDOW_WIDTH*sizeof(uint32_t));
    SDL_RenderCopy(renderer,Screen,NULL,NULL);
    SDL_RenderPresent(renderer);
    if(e) {
      char y[strlen(argv[3])+5];
      sprintf(y,"%s%03i.png",argv[3],(int)k);
      save_texture(y,renderer,Screen);
    }
    k+=1;
    w=bqn_makeF64(k);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(Screen);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
