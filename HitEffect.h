#pragma once
#include"Partiecle\PraticleManeger.h"
#include"Partiecle\TaskManager.h"
#include"DirectXTK.h"


//hit���̃p�[�e�B�N���̍\����
struct HitEffect
{
	//�p�[�e�B�N��
	ParticleManager  *particleManager;
	
	//�J�n�ʒu
	float pos_x;
	float pos_y;
	float pos_z;

	//��������
	int count;

	
	//�X�V����
	void Initialize(ParticleManager *argParticleManager, float x, float y, float z,int argCount);
	//���s
	bool Run();
};