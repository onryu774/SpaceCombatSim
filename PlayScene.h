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
	//敵の総数
	static const int ENEMYS_NUM = 10;
	//地形やオブジェクトの総数
	static const int OBJCT_NUM = 2;
	//パーティクルマネージャ
	std::unique_ptr<ParticleManager> particleManager;
	std::unique_ptr<ParticleManager> particleManager2;
	//タスクマネージャ
	std::unique_ptr<TaskManager>taskManager;
	//エフェクト
	DirectX::EffectFactory*factory;
	//地形やオブジェクトなど
	LandShapeCommonDef def;
	std::vector<LandShape*>landShapeArray;
	//カメラ
	Camera* camera;
	//プレイヤー
	Player* player;
	//敵
	EnemyShip* Enemys[ENEMYS_NUM];
	//制限時間
	int Timelimit;
	//フレームカウント用
	int frem;
	//地形やオブジェクト用
	struct LandshapeTable
	{
		DirectX::SimpleMath::Vector3 trans;
		DirectX::SimpleMath::Vector3 rot;
		float scale;
		wchar_t*filename_MDL;
		wchar_t*filename_CMO;
	};
	struct LandshapeTable s_LandshapeTable[OBJCT_NUM];
	//背景
	Obj3D*ten;
	//地形やオブジェクトの総数
	int landShape_tablenum;
	//true:デバックを表示 false:非表示
	bool debugflag;
public:
	PlayScene();
	//プレイシーンの初期化
	void PlaySceneInit();
	//プレイシーンの実行
	bool PlaySceneRun();
	//当たり判定
	void HitCheck();
	//プレイシーンの描写
	void PlaySceneRender();
	~PlayScene();
	
};

