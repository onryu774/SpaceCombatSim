#pragma once
#include"Game\Obj3D.h"
#include"Game\LandShape.h"
#include"Game\CollisionNode.h"
#include"BulletShot.h"
#include"DirectXTK.h"
class Player
{
public:
	Player();
	~Player();
	void Initialaiz();
	void Update();
	void Calc();
	void Draw();
	const DirectX::SimpleMath::Vector3& GetTrans();
	const DirectX::SimpleMath::Vector3& GetBulletTrans();
	const DirectX::SimpleMath::Vector3& GetRot();
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	void ResetBullet() { Bullet.ResetBullet(&obj.GetLocalWorld()); }
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	/*void SetInputManeger(DirectX::Keyboard::State* pInputManeger) { m_pInputManeger = pInputManeger; }*/
	const CapsuleNode& GetCollisionNodeBullet() { return collisionNodeBullet; }
	const SphereNode& GetCollisionNodeBody() { return collisionNodeBody; }
protected:
	Obj3D obj;
	Obj3D Tage;
private:
	BulletShot Bullet;
	bool m_timeflag;
	DirectX::SimpleMath::Vector3 m_BulletVel;
	int m_frame;
	int m_time;
	CapsuleNode collisionNodeBullet;
	SphereNode  collisionNodeBody;


};

