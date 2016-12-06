#include "Player.h"

#include<SimpleMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

static const float MAX_VAL=10;


Player::Player()
{
	
}


Player::~Player()
{
}

void Player::Initialaiz()
{
	obj.LoadModelFile(L"Resources/cModels/fighter.cmo");
	obj.SetTrans(Vector3(0.0f, 0.0f, 20.0f));

	Tage.LoadModelFile(L"Resources/cModels/TagetSite.cmo");
	Tage.SetParentMatrix(&obj.GetLocalWorld());
	Tage.DisableLighting();
	Tage.SetScale(Vector3(0.05f, 0.05f, 0.05f));
	Tage.SetTrans(Vector3(0.0f, 0.0f, -10.0f));

	Bullet.Initialaiz(&obj.GetLocalWorld());
	Bullet.ONQ();
	collisionNodeBody.Initialize();
	collisionNodeBody.SetParentMatrix(&obj.GetLocalWorld());
	collisionNodeBody.SetLocalRadius(0.3f);
	collisionNodeBody.SetTrans(Vector3(0.0f, 0.0f, 0.0f));

	collisionNodeBullet.Initialize();
	collisionNodeBullet.SetParentMatrix(&Bullet.GetLocalWorld());
	collisionNodeBullet.SetRot(Vector3(XMConvertToRadians(90), 0.0f, 00.0f));
	collisionNodeBullet.SetLocalRadius(1.0f);
	collisionNodeBullet.SetLocalLength(1.0f);
	m_timeflag = false;
}

void Player::Update()
{
	float x;
	float y;
	x = 0.03f;
	y = 0.03f;
	Vector3 Rot = obj.GetRot();
	if (g_mouse.x<453&&g_mouse.x>226&&g_mouse.y>160&&g_mouse.y<320)
	{
		
	}
	if(g_mouse.x<226&&g_mouse.y<160)
	{
		Rot.y += y;
		Rot.x += x;
		/*Rot.z = XMConvertToRadians(45);*/
	}
	if(g_mouse.x<226&&g_mouse.y<320&& g_mouse.y>160)
	{
		Rot.y += y;
		/*Rot.z = XMConvertToRadians(45);*/
	}
	if(g_mouse.y>320&&g_mouse.x<226)
	{
		Rot.y += y;
		Rot.x -= x;
		/*Rot.z = XMConvertToRadians(45);*/
	}
	if(g_mouse.x>226&&g_mouse.x<453&&g_mouse.y<=160)
	{
		Rot.x += x;
		Rot.z = 0;
	}
	if(g_mouse.x>226 && g_mouse.x<453 && g_mouse.y>320)
	{
		Rot.x -= x;
		Rot.z = 0;
	}
    if(g_mouse.x>453&&g_mouse.y<160)
	{
		Rot.y -= y;
		Rot.x += x;
		/*Rot.z = XMConvertToRadians(315);*/
	}
	if(g_mouse.x>453&&g_mouse.y>160&&g_mouse.y<320)
	{
		Rot.y -= y;
		/*Rot.z = XMConvertToRadians(315);*/
	}
	if(g_mouse.x>453&&g_mouse.y>320)
	{
		Rot.y -= y;
		Rot.x -= x;
		/*Rot.z = XMConvertToRadians(315);*/
	}

	if (Bullet.flage() == true)
	{
		Bullet.SetTrans(Bullet.GetTrans() + m_BulletVel);
		Bullet.BulletUpdate();
	}
	if (g_mouseTracker->leftButton==g_mouseTracker->PRESSED)
	{
		Bullet.ONQ();
		Bullet.Update();
		m_BulletVel = Bullet.BulletVel();
		m_timeflag = true;
	}
	if (m_timeflag == true)
	{
		m_frame++;

		if (m_frame >= 60)
		{
			m_frame = 0;
			m_time++;
		}
		if (m_time >= 1)
		{
			m_time = 0;
			Bullet.OFFQ();
			Bullet.ResetBullet(&obj.GetLocalWorld());
			m_timeflag = false;
		}
	}

	Vector3 Trans = obj.GetTrans();
	Vector3 moveV(0.0f, 0.0f, -0.1f);
	if (g_key.W)
	{
		moveV = Vector3(0.0f, 0.0f, -0.15f);
	}
	if (g_key.S)
	{
		moveV = Vector3(0.0f, 0.0f, -0.05f);
	}

	Matrix roteM = Matrix::CreateRotationY(Rot.y);
	Matrix roteM2 = Matrix::CreateRotationX(Rot.x);
	Matrix roteM3 = Matrix::CreateRotationZ(Rot.z);
	Matrix roteS =roteM3*roteM2*roteM;
	moveV = Vector3::TransformNormal(moveV, roteS);


	Trans += moveV;

	obj.SetTrans(Trans);
	
	obj.SetRot(Rot);
	collisionNodeBody.Update();
	collisionNodeBullet.Update();
	//Bullet.Update();
	
	Calc();
}

void Player::Calc()
{
	obj.Calc();
	Bullet.Calc();
	Tage.Calc();
}

void Player::Draw()
{
	obj.Draw(); 
	Tage.Draw();
	if (Bullet.flage() == true)
	{
		Bullet.Draw();
		collisionNodeBullet.Draw();
	}
	/*Bullet.Draw();*/
	collisionNodeBody.Draw();
}

const DirectX::SimpleMath::Vector3 & Player::GetTrans()
{
	// TODO: return ステートメントをここに挿入します
	return obj.GetTrans();
}

const DirectX::SimpleMath::Vector3 & Player::GetBulletTrans()
{
	// TODO: return ステートメントをここに挿入します
	return Bullet.GetTrans();
}

const DirectX::SimpleMath::Vector3 & Player::GetRot()
{
	// TODO: return ステートメントをここに挿入します
	return obj.GetRot();
}

void Player::SetTrans(const DirectX::SimpleMath::Vector3 & trans)
{
	obj.SetTrans(trans);
}

const DirectX::SimpleMath::Matrix & Player::GetLocalWorld()
{
	// TODO: return ステートメントをここに挿入します
	return obj.GetLocalWorld();
}
