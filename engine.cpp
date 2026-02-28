#include "engine.hpp"

Engine3D::Engine3D(SDL_Window *_window, SDL_Renderer *_renderer, mesh &obj)
{
  SDL_GetWindowSize(_window, &ScreenWidth, &ScreenHeight);
  renderer = _renderer;
  meshCube = obj;
  Create();
}

void Engine3D::MultiplyMatrixVector(vec3d &i, vec3d &o, mat4x4 &m)
{
  o.x = i.x * m[0][0] + i.y * m[1][0] + i.z * m[2][0] + i.w * m[3][0];
  o.y = i.x * m[0][1] + i.y * m[1][1] + i.z * m[2][1] + i.w * m[3][1];
  o.z = i.x * m[0][2] + i.y * m[1][2] + i.z * m[2][2] + i.w * m[3][2];
  float w = i.x * m[0][3] + i.y * m[1][3] + i.z * m[2][3] + i.w * m[3][3];
  if (w != 0.0f)
  {
    o.x /= w;
    o.y /= w;
    o.z /= w;
  }
}

void Engine3D::Create()
{

  // Projection Matrix
  float fNear = 0.1f;
  float fFar = 1000.0f;
  float fFov = 90.0f;
  float fAspectRatio = (float)ScreenHeight / (float)ScreenWidth;
  float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      matProj[i][j] = 0.0f;

  matProj[0][0] = fAspectRatio * fFovRad;
  matProj[1][1] = fFovRad;
  matProj[2][2] = fFar / (fFar - fNear);
  matProj[3][2] = (-fFar * fNear) / (fFar - fNear);
  matProj[2][3] = 1.0f;
  matProj[3][3] = 0.0f;
}

void Engine3D::Render(float speedOfRotation)
{
  auto time1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration(0);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

  mat4x4 matRotZ = {0}, matRotX = {0};
  fTheta += speedOfRotation * deltaTime;

  // Rotation Z
  matRotZ[0][0] = cosf(fTheta);
  matRotZ[0][1] = sinf(fTheta);
  matRotZ[1][0] = -sinf(fTheta);
  matRotZ[1][1] = cosf(fTheta);
  matRotZ[2][2] = 1;
  matRotZ[3][3] = 1;

  // Rotation X
  matRotX[0][0] = 1;
  matRotX[1][1] = cosf(fTheta * 0.5f);
  matRotX[1][2] = sinf(fTheta * 0.5f);
  matRotX[2][1] = -sinf(fTheta * 0.5f);
  matRotX[2][2] = cosf(fTheta * 0.5f);
  matRotX[3][3] = 1;

  // Draw Triangles
  for (auto tri : meshCube)
  {
    triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

    // Rotate in Z-Axis
    MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
    MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
    MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

    // Rotate in X-Axis
    MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
    MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
    MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

    // Offset into the screen
    triTranslated = triRotatedZX;
    triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
    triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
    triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

    // Project triangles from 3D --> 2D
    MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
    MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
    MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

    // Scale into view
    triProjected.p[0].x += 1.0f;
    triProjected.p[0].y += 1.0f;
    triProjected.p[1].x += 1.0f;
    triProjected.p[1].y += 1.0f;
    triProjected.p[2].x += 1.0f;
    triProjected.p[2].y += 1.0f;
    triProjected.p[0].x *= 0.5f * (float)ScreenWidth;
    triProjected.p[0].y *= 0.5f * (float)ScreenHeight;
    triProjected.p[1].x *= 0.5f * (float)ScreenWidth;
    triProjected.p[1].y *= 0.5f * (float)ScreenHeight;
    triProjected.p[2].x *= 0.5f * (float)ScreenWidth;
    triProjected.p[2].y *= 0.5f * (float)ScreenHeight;
    SDL_FPoint points[4] = {triProjected.p[0].x, triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y, triProjected.p[2].x, triProjected.p[2].y, triProjected.p[0].x, triProjected.p[0].y};
    SDL_RenderDrawLinesF(renderer, points, 4);
  }
  SDL_RenderPresent(renderer);
  auto time2 = std::chrono::high_resolution_clock::now();
  duration = time2 - time1;
  deltaTime = duration.count();
  time1 = time2;
}
