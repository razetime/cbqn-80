#include <stdlib.h>
#include "../cbqn/include/bqnffi.h"
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 240
#define WINDOW_HEIGHT 136

uint32_t c[]={0xff1a1c2c, 0xff5d275d, 0xffb13e53, 0xffef7d57, 0xffffcd75,
  0xffa7f070, 0xff38b764, 0xff257179, 0xff29366f, 0xff3b5dc9, 0xff41a6f6, 
  0xff73eff7, 0xfff4f4f4, 0xff94b0c2, 0xff566c86, 0xff333c5};

int main(int argc, char* argv[]) {
  if(argc==1){puts("Usage: cbqn-80 file.bqn [framecount [out.jpg/gif]]");
    exit(0);
  }
  FILE* f=fopen(argv[1],"r");
  char* b;
  if(f){
    fseek(f,0,SEEK_END);
    long l=ftell(f);
    fseek(f,0,SEEK_SET);
    b=malloc(l*sizeof(char));
    if(b)fread(b,sizeof(char),l,f);
    fclose(f);
  }
  float m;
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
  
  while(k<m){
    x=bqn_call2(g,w,x);
    uint32_t* a=malloc(WINDOW_HEIGHT*WINDOW_WIDTH*sizeof(uint32_t)); 
    bqn_readI32Arr(x,a);
    for(int i=0;i<WINDOW_HEIGHT*WINDOW_WIDTH;i++){a[i]=c[a[i]&0x0000000f];}               
    SDL_RenderClear(renderer);
    SDL_UpdateTexture(Screen,NULL,a,WINDOW_WIDTH*sizeof(uint32_t));
    SDL_Rect r;
    r.x=0;r.y=0;r.w=240;r.h=136;
    SDL_RenderCopy(renderer,Screen,NULL,&r);
    SDL_RenderPresent(renderer);
    k+=1;
  }
  //SDL_Delay(3000);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyTexture(Screen);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
