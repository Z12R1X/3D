#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <chrono>

struct Point2d
{
  float x, y;
};

struct Point3d : Point2d
{
  float z;
};

struct Vertex
{
  int start, end;
};

class Render3d
{
public:
  Render3d(SDL_Window *window, SDL_Renderer *render, std::vector<Point3d> &point, std::vector<Vertex> &vertices);
  void Render();
  void RenderRotate();

private:
  Point3d RotateX(Point3d point);
  Point3d RotateXtry(Point3d point);
  Point3d RotateY(Point3d point);
  Point3d RotateZ(Point3d point);
  Point2d Projection(Point3d point);

  float rotation = 0.0f;
  float FOV = 8.0;
  float deltaTime = 0.0f;
  int windowSizeX;
  int windowSizeY;
  SDL_Renderer *renderer;
  std::vector<Point3d> points;
  std::vector<Vertex> vertices;
};