#pragma once
#include "Partiecle\PraticleManeger.h"


struct Contrail
{
	ParticleManager  *particleManager;

	float pos_x;
	float pos_y;
	float pos_z;

	int count;



	void Initialize(ParticleManager *argParticleManager,float x,float y,float z,int argCount);

	bool Run();
};
