#pragma once
#include"Partiecle\PraticleManeger.h"
#include"Partiecle\TaskManager.h"
#include"DirectXTK.h"


//hit時のパーティクルの構造体
struct HitEffect
{
	//パーティクル
	ParticleManager  *particleManager;
	
	//開始位置
	float pos_x;
	float pos_y;
	float pos_z;

	//生存時間
	int count;

	
	//更新処理
	void Initialize(ParticleManager *argParticleManager, float x, float y, float z,int argCount);
	//実行
	bool Run();
};