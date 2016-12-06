#include"BeamParticle.h"
#include<SimpleMath.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;

float RandomRange(float min, float max);

void BeamParticle::Initialize(ParticleManager *argParticleManager, TaskManager *argTaskManager, float x, float y, float z, int argCount)
{
	particleManager = argParticleManager;

	taskManager = argTaskManager;



	pos_x = x;
	pos_y = y;
	pos_z = z;

	vel_x = 0.0f;
	vel_y = 0.0f;
	vel_z = 0.0f;

	count = argCount;


}

bool BeamParticle::Run()
{
	if (--count < 0) return false;

	

	particleManager->Entry(
		60,
		pos_x, pos_y, pos_z,
		RandomRange(-0.2f,0.2f), RandomRange(-0.2f, 0.2f), RandomRange(-0.2f, 0.2f),
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		0.1f, 1.0f,
		1.0f, 0.5f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f);


	return true;
}
