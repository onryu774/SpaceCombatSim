#include "Contrail.h"

float RandomRange(float min, float max);

void Contrail::Initialize(ParticleManager *argParticleManager,float x,float y,float z,int argCount)
{
	particleManager = argParticleManager;

	pos_x = x;
	pos_y = y;
	pos_z = z;

	count = argCount;


}

bool Contrail::Run()
{
	if( --count < 0 ) return false;

	particleManager->Entry(
		60,
		pos_x, pos_y, pos_z,
		0.0f,0.0f,0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 10.0f,
		1.0f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f);


	return true;
}
