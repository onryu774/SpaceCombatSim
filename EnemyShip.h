#pragma once
#include"Game\Obj3D.h"
#include"Game\LandShape.h"
#include"Game\CollisionNode.h"
#include"DirectXTK.h"
class EnemyShip
{
public:
	EnemyShip();
	~EnemyShip();
	void Initialaiz(const int x,const int y,const int z);
	void Update(const DirectX::SimpleMath::Vector3 Player_Pos);
	void Calc();
	void Draw();
	const DirectX::SimpleMath::Vector3& GetTrans();
	const DirectX::SimpleMath::Vector3& GetRot();
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	void Damegy() { m_Lifes--;}
	bool Livese();
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	const CapsuleNode& GetCollisionNodeBody() { return collisionNodeBody; }
	
protected:
	Obj3D obj;
private:
	
	bool m_Liveflag;
	DirectX::SimpleMath::Matrix LooKAtMatrix(const DirectX::SimpleMath::Vector3 Player_Pos);
	DirectX::SimpleMath::Vector3 Ptage;
	bool EnemyView(const DirectX::SimpleMath::Vector3 Player_Pos, const DirectX::SimpleMath::Vector3 EnemyMove);
	void NoFinedMove(int frem);
	void FinedMove();
	DirectX::SimpleMath::Vector3 Slep(const DirectX::SimpleMath::Vector3 start,const DirectX::SimpleMath::Vector3 end,float frem);
	int m_frame;
	int m_time;
	int m_Lifes;
	int m_x;
	int m_y;
	int m_z;
	CapsuleNode  collisionNodeBody;
};