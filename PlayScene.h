#pragma once
#include<ctime>
#include <DirectXColors.h>
#include "Direct3D.h"
#include "DirectXTK.h"
#include <SimpleMath.h>
#include "Grid.h"
#include "DebugCamera.h"
#include"Game\Obj3D.h"
#include"Game\Camera.h"
#include "Player.h"
#include"EnemyShip.h"
#include"Partiecle\PraticleManeger.h"
#include"Partiecle\TaskManager.h"
#include"HitEffect.h"
#include"Contrail.h"

class PlayScene
{
private:
	//�G�̑���
	static const int ENEMYS_NUM = 10;
	//�n�`��I�u�W�F�N�g�̑���
	static const int OBJCT_NUM = 2;
	//�p�[�e�B�N���}�l�[�W��
	std::unique_ptr<ParticleManager> particleManager;
	std::unique_ptr<ParticleManager> particleManager2;
	//�^�X�N�}�l�[�W��
	std::unique_ptr<TaskManager>taskManager;
	//�G�t�F�N�g
	DirectX::EffectFactory*factory;
	//�n�`��I�u�W�F�N�g�Ȃ�
	LandShapeCommonDef def;
	std::vector<LandShape*>landShapeArray;
	//�J����
	Camera* camera;
	//�v���C���[
	Player* player;
	//�G
	EnemyShip* Enemys[ENEMYS_NUM];
	//��������
	int Timelimit;
	//�t���[���J�E���g�p
	int frem;
	//�n�`��I�u�W�F�N�g�p
	struct LandshapeTable
	{
		DirectX::SimpleMath::Vector3 trans;
		DirectX::SimpleMath::Vector3 rot;
		float scale;
		wchar_t*filename_MDL;
		wchar_t*filename_CMO;
	};
	struct LandshapeTable s_LandshapeTable[OBJCT_NUM];
	//�w�i
	Obj3D*ten;
	//�n�`��I�u�W�F�N�g�̑���
	int landShape_tablenum;
	//true:�f�o�b�N��\�� false:��\��
	bool debugflag;
public:
	PlayScene();
	//�v���C�V�[���̏�����
	void PlaySceneInit();
	//�v���C�V�[���̎��s
	bool PlaySceneRun();
	//�����蔻��
	void HitCheck();
	//�v���C�V�[���̕`��
	void PlaySceneRender();
	~PlayScene();
	
};

