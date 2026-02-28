#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <chrono>

struct vec3d
{
  float x, y, z, w;
  vec3d() : x(0), y(0), z(0), w(1) {};
  vec3d(float xCor, float yCor, float zCor) : x(xCor), y(yCor), z(zCor), w(1) {};
};

struct triangle
{
  vec3d p[3];
};

/*struct mesh
{
  std::vector<triangle> tris;
};*/

/*struct mat4x4
{
  float m[4][4] = {0};
};*/

using mat4x4 = float[4][4];
//using triangle = vec3d[3];
using mesh = std::vector<triangle>;

class Engine3D
{
public:
  Engine3D(SDL_Window *window, SDL_Renderer *render, mesh &obj);
  void Render(float speedOfRotation);
  //void RenderRotate();
  void Create();

private:
  SDL_Renderer *renderer;
  mesh meshCube;
  mat4x4 matProj;
  float fTheta;
  float deltaTime=0.0f;
  int ScreenWidth, ScreenHeight;
  // std::vector<triangle> figura;
  void MultiplyMatrixVector(vec3d &i, vec3d &o, mat4x4 &m);
};