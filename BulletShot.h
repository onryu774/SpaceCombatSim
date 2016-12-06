#pragma once
#include"Game\Obj3D.h"
#include"Game\LandShape.h"
#include"Game\CollisionNode.h"
#include"DirectXTK.h"
class BulletShot
{
private:
	Obj3D obj;
	
	CapsuleNode collisionNodeBullet;
	DirectX::SimpleMath::Vector3 m_BulletVel;
	bool m_flag;
public:
	BulletShot();
	~BulletShot();
	void Initialaiz(const DirectX::SimpleMath::Matrix*Parent);
	void Update();
	void Calc();
	void Draw();
	void ResetBullet(const DirectX::SimpleMath::Matrix*Parent);
	void ONQ() { obj.OnQuaater(); };
	void OFFQ() { obj.OffQuaater(); };
	bool flage() { return m_flag; };
	const DirectX::SimpleMath::Vector3& GetTrans();
	const DirectX::SimpleMath::Vector3& BulletVel() { return m_BulletVel; };
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	const CapsuleNode& GetCollisionNodeBullet() { return collisionNodeBullet; }
	void BulletUpdate() { collisionNodeBullet.Update(); };

};

