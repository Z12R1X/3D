#include "render3d.hpp"

Render3d::Render3d(SDL_Window *_window, SDL_Renderer *_renderer, std::vector<Point3d> &_points, std::vector<Vertex> &_vertices)
{
  SDL_GetWindowSize(_window, &windowSizeX, &windowSizeY);
  renderer = _renderer;
  points = _points;
  vertices = _vertices;
}

Point3d Render3d::RotateX(Point3d point)
{
  Point3d returnPoint;
  returnPoint.x = point.x;
  returnPoint.y = cos(rotation) * point.y - sin(rotation) * point.z;
  returnPoint.z = sin(rotation) * point.y + cos(rotation) * point.z;
  return returnPoint;
}

Point3d Render3d::RotateXtry(Point3d point)
{
  Point3d returnPoint;
  returnPoint.x = point.x;
  returnPoint.y = cos(rotation) * point.y + sin(rotation) * point.z;
  returnPoint.z = cos(rotation) * point.z - sin(rotation) * point.z;
  return returnPoint;
}

Point3d Render3d::RotateY(Point3d point)
{
  Point3d returnPoint;
  returnPoint.x = cos(rotation) * point.x + sin(rotation) * point.z;
  returnPoint.y = point.y;
  returnPoint.z = cos(rotation) * point.z - sin(rotation) * point.x;
  return returnPoint;
}

Point3d Render3d::RotateZ(Point3d point)
{
  Point3d returnPoint;
  returnPoint.x = cos(rotation) * point.x - sin(rotation) * point.y;
  returnPoint.y = cos(rotation) * point.y + sin(rotation) * point.x;
  returnPoint.z = point.z;
  return returnPoint;
}

Point2d Render3d::Projection(Point3d point)
{
  Point2d returnPoint = {2*windowSizeX / 20 + (FOV + point.x) / (FOV + point.z)*400, -windowSizeY / 5 + (FOV + point.y) / (FOV + point.z)*400};
  return returnPoint;
}

void Render3d::Render()
{
  auto time1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration(0);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

  rotation += 2 * deltaTime;
  for (auto &vertex : vertices)
  {
    Point3d rotatedStartPoint = RotateX(RotateY(points[vertex.start]));
    Point3d rotatedEndPoint = RotateX(RotateY(points[vertex.end]));
    Point2d start = Projection(rotatedStartPoint);
    Point2d end = Projection(rotatedEndPoint);
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
  }
  SDL_RenderPresent(renderer);
  auto time2 = std::chrono::high_resolution_clock::now();
  duration = time2 - time1;
  deltaTime = duration.count();
  time1 = time2;
}

void Render3d::RenderRotate()
{
  auto time1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration(0);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

  rotation += 0.25 * deltaTime;
  for (auto &vertex : vertices)
  {
    Point3d rotatedStartPoint = RotateX(RotateY(RotateZ(points[vertex.start])));
    Point3d rotatedEndPoint = RotateX(RotateY(RotateZ(points[vertex.end])));
    Point2d start = Projection(rotatedStartPoint);
    Point2d end = Projection(rotatedEndPoint);
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
  }
  SDL_RenderPresent(renderer);
  auto time2 = std::chrono::high_resolution_clock::now();
  duration = time2 - time1;
  deltaTime = duration.count();
  time1 = time2;
}