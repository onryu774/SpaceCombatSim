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
#include"BeamParticle.h"
#include"Contrail.h"

class PlayScene
{
private:
	static const int ENEMYS_NUM = 10;
	static const int OBJCT_NUM = 2;
	std::unique_ptr<ParticleManager> particleManager;
	std::unique_ptr<ParticleManager> particleManager2;
	std::unique_ptr<TaskManager>taskManager;
	DirectX::EffectFactory*factory;
	LandShapeCommonDef def;
	std::vector<LandShape*>landShapeArray;
	Camera* camera;

	Player* player;

	EnemyShip* Enemys[ENEMYS_NUM];
	int Timelimit;
	int frem;
	struct LandshapeTable
	{
		DirectX::SimpleMath::Vector3 trans;
		DirectX::SimpleMath::Vector3 rot;
		float scale;
		wchar_t*filename_MDL;
		wchar_t*filename_CMO;
	};
	struct LandshapeTable s_LandshapeTable[OBJCT_NUM];
	Obj3D*ten;
	int landShape_tablenum;
	bool debugflag;
public:
	PlayScene();
	void PlaySceneInit();
	bool PlaySceneRun();
	void HitCheck();
	void PlaySceneRender();
	~PlayScene();
	
};

