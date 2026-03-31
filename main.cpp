#include "engine.hpp"

int main()
{
  SDL_Window *window=SDL_CreateWindow("3D Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, 0);;
  SDL_Renderer *renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  bool running=true;
  bool rendering=true;
  Engine3D render(window, renderer);
  render.loadMesh("VideoShip.obj");
  SDL_Event event;
  while (running){
    if (SDL_PollEvent(&event)){
      switch (event.type){
		case SDL_KEYDOWN:
			rendering=false;
			break;
		case SDL_KEYUP:
			rendering=true;
			break;
		default:
			break;
      }
    }if (SDL_QuitRequested()){
      running = false;
      break;
    }if (rendering){
      render.Render(0.25f);
    }
  }
  return 0;
}