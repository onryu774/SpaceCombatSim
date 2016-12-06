#include "GameTitle.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

GameTitle::GameTitle()
{
}

void GameTitle::TitleInit()
{
	camera = new Camera;
	factory = new EffectFactory(g_pd3dDevice.Get());
	//obj3DÇÃèâä˙âª
	{
		Obj3D::SetDevice(g_pd3dDevice.Get());
		Obj3D::SetDeviceContext(g_pImmediateContext.Get());
		Obj3D::SetStates(g_state.get());
		Obj3D::SetEffectFactory(factory);
		Obj3D::SetCamera(camera);
	}
}

bool GameTitle::TitleRun()
{
	return false;
}

void GameTitle::TitleRender()
{
}


GameTitle::~GameTitle()
{
}
