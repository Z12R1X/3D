#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <numbers>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

struct vec3d{
	public:
		float x,y,z,w;
		vec3d():x(0.0f),y(0.0f),z(0.0f),w(1.0f){};
		vec3d(float xCor, float yCor, float zCor):x(xCor),y(yCor),z(zCor),w(1.0f){};
	public:
		float dotProduct(const vec3d& vec1,const vec3d& vec2);
};
struct triangle{
  	vec3d p[3];
};
using mat4x4=float[4][4];
using mesh=std::vector<triangle>;

class Engine3D{
	public:
		Engine3D(SDL_Window* window, SDL_Renderer* render);
		void Render(const float speedOfRotation);
		void Create();
		bool loadMesh(const std::string& filename);
	private:
		SDL_Renderer* renderer;
		mesh meshObject;
		mat4x4 matProj={0};
		vec3d camera;
		float fTheta;
		float deltaTime=0.0f;
		int ScreenWidth,ScreenHeight;
		void MultiplyMatrixVector(const vec3d& i, vec3d& o,const mat4x4& m);
};