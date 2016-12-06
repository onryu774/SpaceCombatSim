//////////////////////////////
//ヒットエフェクト
//製作　恩田龍登
///////////////////////////////
#include"HitEffect.h"
#include<SimpleMath.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;

float RandomRange(float min, float max);
//初期化
void HitEffect::Initialize(ParticleManager *argParticleManager, float x, float y, float z, int argCount)
{
	particleManager = argParticleManager;





	pos_x = x;
	pos_y = y;
	pos_z = z;

	

	count = argCount;


}
//実行処理
bool HitEffect::Run()
{
	if (--count < 0) return false;

	

	particleManager->Entry(
		60,//表示パーティクル数
		pos_x, pos_y, pos_z,//パーティクル始まる位置
		RandomRange(-0.2f,0.2f), RandomRange(-0.2f, 0.2f), RandomRange(-0.2f, 0.2f),//パーティクルの終わる位置
		0.0f, 0.0f, 0.0f,//加速度
		0.0f, 0.0f,//回転
		0.1f, 1.0f,//大きさ
		1.0f, 0.5f, 0.5f, 1.0f,//始まりの色
		1.0f, 1.0f, 1.0f, 1.0f//消えるときの色
		);


	return true;
}
