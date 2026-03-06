#include "engine.hpp"

float vec3d::dotProduct(const vec3d& vec1,const vec3d& vec2){
	return vec1.x*vec2.x+vec1.y*vec2.y+vec1.z*vec2.z;
}

vec3d vec3d::crossProduct(const vec3d& vec1,const vec3d& vec2){
	vec3d res;
	res.x=vec1.y*vec2.z-vec1.z*vec2.y;
	res.y=vec1.z*vec2.x-vec1.x*vec2.z;
	res.z=vec1.x*vec2.y-vec1.y*vec2.x;
	return res;
}

vec3d vec3d::normal(const vec3d& vec1,const vec3d& vec2){
	vec3d res=crossProduct(vec1,vec2);
	res/=res.len();
	return res;
}

float vec3d::len(){
	return sqrt(this->x*this->x+this->y*this->y+this->z*this->z);
}

vec3d vec3d::operator+(const vec3d& vec){
	vec3d res={this->x+vec.x,this->y+vec.y,this->z+vec.z};
	return res;
}

vec3d vec3d::operator-(const vec3d& vec){
	vec3d res={this->x-vec.x,this->y-vec.y,this->z-vec.z};
	return res;
}

vec3d vec3d::operator*(float scalar){
	vec3d res={this->x*scalar,this->y*scalar,this->z*scalar};
	return res;
}

vec3d vec3d::operator/(float scalar){
	vec3d res={this->x/scalar,this->y/scalar,this->z/scalar};
	return res;
}

void vec3d::operator+=(const vec3d& vec){
	this->x+=vec.x;this->y+=vec.y;this->z+=vec.z;
}

void vec3d::operator-=(const vec3d& vec){
	this->x-=vec.x;this->y-=vec.y;this->z-=vec.z;
}

void vec3d::operator*=(float scalar){
	this->x*=scalar;this->y*=scalar;this->z*=scalar;
}

void vec3d::operator/=(float scalar){
	this->x/=scalar;this->y/=scalar;this->z/=scalar;
}

Engine3D::Engine3D(SDL_Window *window_, SDL_Renderer* renderer_){
	SDL_GetWindowSize(window_, &ScreenWidth, &ScreenHeight);
	renderer=renderer_;
	CreateProjMatrix();
}

void Engine3D::MultiplyMatrixVector(const vec3d &i, vec3d &o,const mat4x4 &m){
	o.x=i.x*m[0][0]+i.y*m[1][0]+i.z*m[2][0]+i.w*m[3][0];
	o.y=i.x*m[0][1]+i.y*m[1][1]+i.z*m[2][1]+i.w*m[3][1];
	o.z=i.x*m[0][2]+i.y*m[1][2]+i.z*m[2][2]+i.w*m[3][2];
	float w=i.x*m[0][3]+i.y*m[1][3]+i.z*m[2][3]+i.w*m[3][3];
	if (w!=0.0f){
		o.x/=w;o.y/=w;o.z/=w;
	}
}

bool Engine3D::loadMesh(const std::string& filename){
	if(!meshObject.empty()){
		meshObject.clear();
	}
	std::ifstream file(filename);
	if(!file.is_open()){
		return false;
	}
	std::vector<vec3d> vertsCache;
	while(!file.eof()){
		char line[128];
		file.getline(line,128);
		std::stringstream s;
		s<<line;
		char junk;
		if(line[0]=='v'){
			vec3d v;
			s>>junk>>v.x>>v.y>>v.z;
			vertsCache.push_back(v);
		}
		if(line[0]=='f'){
			int f[3];
			s>>junk>>f[0]>>f[1]>>f[2];
			meshObject.push_back({vertsCache[f[0]-1],vertsCache[f[1]-1],vertsCache[f[2]-1]});
		}
	}return true;
}

void Engine3D::CreateProjMatrix(){
	// Projection Matrix
	float fNear=0.1f;
	float fFar=1000.0f;
	float fFov=90.0f;
	float fAspectRatio=(float)ScreenHeight/(float)ScreenWidth;
	float fFovRad =1.0f/tanf(fFov*0.5f/180.0f*std::numbers::pi);
	matProj[0][0]=fAspectRatio*fFovRad;
	matProj[1][1]=fFovRad;
	matProj[2][2]=fFar/(fFar-fNear);
	matProj[3][2]=(-fFar*fNear)/(fFar-fNear);
	matProj[2][3]=1.0f;
}

void Engine3D::Render(const float speedOfRotation)
{
	auto time1=std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration(0);
	SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer,255,255,255,SDL_ALPHA_OPAQUE);
	mat4x4 matRotZ={0},matRotX={0},matRotY={0};
	fTheta+=speedOfRotation*deltaTime;
	// Rotation Z
	matRotZ[0][0]=cosf(fTheta);
	matRotZ[0][1]=-sinf(fTheta);//sinf
	matRotZ[1][0]=sinf(fTheta);//-sinf
	matRotZ[1][1]=cosf(fTheta);
	matRotZ[2][2]=1.0f;
	matRotZ[3][3]=1.0f;
	// Rotation X
	matRotX[0][0]=1.0f;
	matRotX[1][1]=cosf(fTheta);
	matRotX[1][2]=-sinf(fTheta);//sinf
	matRotX[2][1]=sinf(fTheta);//-sinf
	matRotX[2][2]=cosf(fTheta);
	matRotX[3][3]=1.0f;

	std::vector<triangle> vecTrianglesToRaster;

	// Draw Triangles
	for (const auto& tri:meshObject){
		triangle triProjected,triTranslated,triRotatedZ,triRotatedZX;
		// Rotate in Z-Axis
		MultiplyMatrixVector(tri.p[0],triRotatedZ.p[0],matRotZ);
		MultiplyMatrixVector(tri.p[1],triRotatedZ.p[1],matRotZ);
		MultiplyMatrixVector(tri.p[2],triRotatedZ.p[2],matRotZ);
		// Rotate in X-Axis
		MultiplyMatrixVector(triRotatedZ.p[0],triRotatedZX.p[0],matRotX);
		MultiplyMatrixVector(triRotatedZ.p[1],triRotatedZX.p[1],matRotX);
		MultiplyMatrixVector(triRotatedZ.p[2],triRotatedZX.p[2],matRotX);

		triTranslated=triRotatedZX;

		triTranslated.p[0].z+=8.0f;
		triTranslated.p[1].z+=8.0f;
		triTranslated.p[2].z+=8.0f;

		vec3d norm,line1,line2;
		line1=triTranslated.p[1]-triTranslated.p[0];
		line2=triTranslated.p[2]-triTranslated.p[0]; 
		norm=vec3d::normal(line1,line2);
		if(vec3d::dotProduct(norm,triTranslated.p[0]-camera)<0.0f){
			MultiplyMatrixVector(triTranslated.p[0],triProjected.p[0],matProj);
			MultiplyMatrixVector(triTranslated.p[1],triProjected.p[1],matProj);
			MultiplyMatrixVector(triTranslated.p[2],triProjected.p[2],matProj);
			// Scale into view
			triProjected.p[0].x+=1.0f;
			triProjected.p[0].y+=1.0f;
			triProjected.p[1].x+=1.0f;
			triProjected.p[1].y+=1.0f;
			triProjected.p[2].x+=1.0f;
			triProjected.p[2].y+=1.0f;
			triProjected.p[0].x*=0.5f*(float)ScreenWidth;
			triProjected.p[0].y*=0.5f*(float)ScreenHeight;
			triProjected.p[1].x*=0.5f*(float)ScreenWidth;
			triProjected.p[1].y*=0.5f*(float)ScreenHeight;
			triProjected.p[2].x*=0.5f*(float)ScreenWidth;
			triProjected.p[2].y*=0.5f*(float)ScreenHeight;
			vecTrianglesToRaster.push_back(triProjected);
		}
		sort(vecTrianglesToRaster.begin(),vecTrianglesToRaster.end(),[](const triangle& t1,const triangle& t2)
		{
			float z1=(t1.p[0].z+t1.p[1].z+t1.p[2].z);
			float z2=(t2.p[0].z+t2.p[1].z+t2.p[2].z);
			return z1>z2;
		});
		for(auto& triProjected:vecTrianglesToRaster){
			SDL_FPoint points[4]={triProjected.p[0].x,triProjected.p[0].y,triProjected.p[1].x,triProjected.p[1].y, triProjected.p[2].x,triProjected.p[2].y,triProjected.p[0].x,triProjected.p[0].y};
			SDL_RenderDrawLinesF(renderer,points,4);
			SDL_Vertex verts[3];
			verts[0].position=points[0];verts[0].color=SDL_Color{255,0,0,255};
			verts[1].position=points[1];verts[1].color=SDL_Color{0,255,0,255};
			verts[2].position=points[2];verts[2].color=SDL_Color{0,0,255,255};
			SDL_RenderGeometry(renderer,nullptr,verts,3,nullptr,0);
		}
	}SDL_RenderPresent(renderer);
	auto time2=std::chrono::high_resolution_clock::now();
	duration=time2-time1;
	deltaTime=duration.count();
	time1=time2;
}
