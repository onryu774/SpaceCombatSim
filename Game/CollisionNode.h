#pragma once
#include"Obj3D.h"
#include"Collision.h"

class CollisionNode
{
protected:
	static bool s_DebugFlag;
public:
	static void SetDebugVisible(bool flag) { s_DebugFlag = flag; }
	static bool GetDebugVisible() { return s_DebugFlag; }

};

class  SphereNode :public Sphere
{
public:
	SphereNode();
	void Initialize();
	void Draw();
	void Update();
	void SetParentMatrix(const DirectX::SimpleMath::Matrix* pParentMatrix);
	void SetLocalRadius(float radius) { m_localRadius = radius; }
	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_Trans = trans; }
private:
	Obj3D obj3d;
	float m_localRadius;
	DirectX::SimpleMath::Vector3 m_Trans;
};
class CapsuleNode :public Capsule
{
public:
	CapsuleNode();
	void Initialize();
	void Draw();
	void Update();
	void SetParentMatrix(const DirectX::SimpleMath::Matrix* pParentMatrix);
	void SetRot(const DirectX::SimpleMath::Vector3& rot);
	void SetLocalRadius(float radius) { m_localRadius = radius; }
	void SetLocalLength(float Length) { m_localLength = Length; }
	void SetTrans(const DirectX::SimpleMath::Vector3& trans) {m_Trans = trans;}
private:
	Obj3D obj3d;
	float m_localRadius;
	float m_localLength;
	DirectX::SimpleMath::Vector3 m_Trans;
};