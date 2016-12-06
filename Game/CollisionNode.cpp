#include"CollisionNode.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
bool CollisionNode::s_DebugFlag = false;
SphereNode::SphereNode()
{
	m_localRadius = 1.0f;
}

void SphereNode::Initialize()
{
	obj3d.LoadModelFile(L"Resources/cModels/hitsphere.cmo");
}

void SphereNode::Draw()
{
	
	if (CollisionNode::GetDebugVisible())
	{
		obj3d.Draw();
	}
}

void SphereNode::Update()
{
	//////////////////////////
	obj3d.SetTrans(m_Trans);
	Vector3 localScale;
	localScale.x = m_localRadius;
	localScale.y = m_localRadius;
	localScale.z = m_localRadius;
	obj3d.SetScale(localScale);
	obj3d.Calc();
	///////////////////////////
	///////////////////////////////////////////////
	Matrix Worldm = obj3d.GetLocalWorld();
	Vector3 Cnter(0, 0, 0);
	Vector3 right(1, 0, 0);
	Cnter = Vector3::Transform(Cnter, Worldm);
	right = Vector3::Transform(right, Worldm);
	center = Cnter;
	radius = Vector3::Distance(Cnter, right);
	///////////////////////////////////////////////
}

void SphereNode::SetParentMatrix(const DirectX::SimpleMath::Matrix * pParentMatrix)
{
	obj3d.SetParentMatrix(pParentMatrix);
}

CapsuleNode::CapsuleNode()
{
	m_localLength = 1.0f;
	m_localRadius = 1.0f;

}

void CapsuleNode::Initialize()
{
	obj3d.LoadModelFile(L"Resources/cModels/hitcapsule.cmo");
}

void CapsuleNode::Draw()
{
	if (CollisionNode::GetDebugVisible())
	{
		obj3d.Draw();
	}
}

void CapsuleNode::Update()
{
	//////////////////////////
	obj3d.SetTrans(m_Trans);
	Vector3 localScale;
	localScale.x = m_localRadius;
	localScale.z = m_localRadius;
	localScale.y = m_localLength;
	obj3d.SetScale(localScale);
	obj3d.Calc();
	///////////////////////////
	///////////////////////////////////////////////
	Matrix Worldm = obj3d.GetLocalWorld();
	Vector3 Cnter(0, 0, 0);
	Vector3 right(1, 0, 0);
	Vector3 up(0, 1, 0);
	Cnter = Vector3::Transform(Cnter, Worldm);
	right = Vector3::Transform(right, Worldm);
	up = Vector3::Transform(up, Worldm);
	segment.start = Cnter;
	segment.end = up;
	radius = Vector3::Distance(Cnter, right);
	///////////////////////////////////////////////
}

void CapsuleNode::SetParentMatrix(const DirectX::SimpleMath::Matrix * pParentMatrix)
{
	obj3d.SetParentMatrix(pParentMatrix);
}

void CapsuleNode::SetRot(const DirectX::SimpleMath::Vector3 & rot)
{
	obj3d.SetRot(rot);
}
