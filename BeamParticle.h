#pragma once
#include"Partiecle\PraticleManeger.h"
#include"Partiecle\TaskManager.h"
#include"DirectXTK.h"

struct BeamParticle
{
	ParticleManager  *particleManager;

	TaskManager      *taskManager;

	float pos_x;
	float pos_y;
	float pos_z;

	float vel_x;
	float vel_y;
	float vel_z;

	int count;

	

	void Initialize(ParticleManager *argParticleManager, TaskManager *argTaskManager, float x, float y, float z,int argCount);

	bool Run();
};