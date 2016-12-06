#pragma once
#include<ctime>
#include <DirectXColors.h>
#include "Direct3D.h"
#include "DirectXTK.h"
#include <SimpleMath.h>
#include "DebugCamera.h"
#include"Game\Obj3D.h"
#include"Game\Camera.h"
#include"Partiecle\PraticleManeger.h"
#include"Partiecle\TaskManager.h"
class GameTitle
{
private:
	Obj3D* obj;
	std::unique_ptr<ParticleManager> particleManager;
	DirectX::EffectFactory*factory;
	Camera* camera;
public:
	GameTitle();
	void TitleInit();
	bool TitleRun();
	void TitleRender();
	~GameTitle();
};

