#include "engine.hpp"

// cube
// SOUTH
triangle tri1 = {vec3d(0.0f, 0.0f, 0.0f), vec3d(0.0f, 1.0f, 0.0f), vec3d(1.0f, 1.0f, 0.0f)};
triangle tri2 = {vec3d(0.0f, 0.0f, 0.0f), vec3d(1.0f, 1.0f, 0.0f), vec3d(1.0f, 0.0f, 0.0f)};
// EAST
triangle tri3 = {vec3d(1.0f, 0.0f, 0.0f), vec3d(1.0f, 1.0f, 0.0f), vec3d(1.0f, 1.0f, 1.0f)};
triangle tri4 = {vec3d(1.0f, 0.0f, 0.0f), vec3d(1.0f, 1.0f, 1.0f), vec3d(1.0f, 0.0f, 1.0f)};
// NORTH
triangle tri5 = {vec3d(1.0f, 0.0f, 1.0f), vec3d(1.0f, 1.0f, 1.0f), vec3d(0.0f, 1.0f, 1.0f)};
triangle tri6 = {vec3d(1.0f, 0.0f, 1.0f), vec3d(0.0f, 1.0f, 1.0f), vec3d(0.0f, 0.0f, 1.0f)};
// WEST
triangle tri7 = {vec3d(0.0f, 0.0f, 1.0f), vec3d(0.0f, 1.0f, 1.0f), vec3d(0.0f, 1.0f, 0.0f)};
triangle tri8 = {vec3d(0.0f, 0.0f, 1.0f), vec3d(0.0f, 1.0f, 0.0f), vec3d(0.0f, 0.0f, 0.0f)};
// TOP
triangle tri9 = {vec3d(0.0f, 1.0f, 0.0f), vec3d(0.0f, 1.0f, 1.0f), vec3d(1.0f, 1.0f, 1.0f)};
triangle tri10 = {vec3d(0.0f, 1.0f, 0.0f), vec3d(1.0f, 1.0f, 1.0f), vec3d(1.0f, 1.0f, 0.0f)};
// BOTTOM
triangle tri11 = {vec3d(1.0f, 0.0f, 1.0f), vec3d(0.0f, 0.0f, 1.0f), vec3d(0.0f, 0.0f, 0.0f)};
triangle tri12 = {vec3d(1.0f, 0.0f, 1.0f), vec3d(0.0f, 0.0f, 0.0f), vec3d(1.0f, 0.0f, 0.0f)};
mesh cube{tri1, tri2, tri3, tri4, tri5, tri6, tri7, tri8, tri9, tri10, tri11, tri12};

// BOTTOM
triangle tri13 = {vec3d(1.0f, 0.0f, 1.0f), vec3d(0.0f, 0.0f, 1.0f), vec3d(0.0f, 0.0f, 0.0f)};
triangle tri14 = {vec3d(1.0f, 0.0f, 1.0f), vec3d(0.0f, 0.0f, 0.0f), vec3d(1.0f, 0.0f, 0.0f)};
// SOUTH
triangle tri15 = {vec3d(0.0f, 0.0f, 0.0f), vec3d(sqrt(2) / 2.0f, 1.0f, sqrt(2) / 2.0f), vec3d(1.0f, 0.0f, 0.0f)};
// EAST
triangle tri16 = {vec3d(1.0f, 0.0f, 0.0f), vec3d(sqrt(2) / 2.0f, 1.0f, sqrt(2) / 2.0f), vec3d(1.0f, 0.0f, 1.0f)};
// NORTH
triangle tri17 = {vec3d(1.0f, 0.0f, 1.0f), vec3d(sqrt(2) / 2.0f, 1.0f, sqrt(2) / 2.0f), vec3d(0.0f, 0.0f, 1.0f)};
// WEST
triangle tri18 = {vec3d(0.0f, 0.0f, 1.0f), vec3d(sqrt(2) / 2.0f, 1.0f, sqrt(2) / 2.0f), vec3d(0.0f, 0.0f, 0.0f)};
mesh piramid{tri13, tri14, tri15, tri16, tri17, tri18};

triangle tri19 = {vec3d(1.0f, 0.0f, 0.0f), vec3d(0.0f, 0.0f, 0.0f), vec3d(0.0f, 0.0f, 1.0f)};
triangle tri20 = {vec3d(0.0f, 0.0f, 0.0f), vec3d(sqrt(2) / 2.0f, 1.0f, sqrt(2) / 2.0f), vec3d(1.0f, 0.0f, 0.0f)};
triangle tri21 = {vec3d(1.0f, 0.0f, 0.0f), vec3d(sqrt(2) / 2.0f, 1.0f, sqrt(2) / 2.0f), vec3d(0.0f, 0.0f, 1.0f)};
triangle tri22 = {vec3d(0.0f, 0.0f, 1.0f), vec3d(sqrt(2) / 2.0f, 1.0f, sqrt(2) / 2.0f), vec3d(0.0f, 0.0f, 0.0f)};
mesh halfPira = {tri19, tri20, tri21, tri22};

triangle tri23 = {vec3d(1.0f, 1.0f, 1.0f), vec3d(2.0f, 1.0f, 1.0f), vec3d(2.0f, 2.0f, 2.0f)};
mesh trian{tri23};

int main()
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  window = SDL_CreateWindow("3D Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  bool running = true;
  bool rend = true;
  Engine3D render(window, renderer, halfPira);
  SDL_Event event;
  while (running){
    if (SDL_PollEvent(&event)){
      switch (event.type){
		case SDL_KEYDOWN:
			rend = false;
			break;
		case SDL_KEYUP:
			rend = true;
			break;
		default:
			break;
      }
    }if (SDL_QuitRequested()){
      running = false;
      break;
    }if (rend){
      render.Render(1.0f);
    }
  }
  return 0;
}