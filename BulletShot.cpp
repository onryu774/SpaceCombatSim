#include "BulletShot.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
BulletShot::BulletShot()
{
	
}


BulletShot::~BulletShot()
{
	
}

void BulletShot::Calc()
{
	obj.Calc();
}

void BulletShot::Draw()
{

	obj.Draw();
}

void BulletShot::Initialaiz(const DirectX::SimpleMath::Matrix* pParentMatrix)
{
	obj.LoadModelFile(L"Resources/cModels/Beam.cmo");
	obj.SetParentMatrix(pParentMatrix);
	obj.DisableLighting();
	obj.SetScale(Vector3(0.2f, 0.2f, 0.2f));
	
	collisionNodeBullet.Initialize();
	collisionNodeBullet.SetParentMatrix(&obj.GetLocalWorld());
	collisionNodeBullet.SetLocalRadius(2);
	collisionNodeBullet.SetLocalLength(10);
	collisionNodeBullet.SetTrans(Vector3(0, 0, 0));
	collisionNodeBullet.SetRot(Vector3(-XM_PIDIV2, 0, 0));
	
}

void BulletShot::Update()
{
	if (m_flag==true)return;

	int g = 0;


	Matrix worlddm = obj.GetLocalWorld();
	Vector3 scale;
	Quaternion rotp;
	Vector3 pos;

	Vector3* m0 = (Vector3*)&worlddm.m[0];
	Vector3* m1 = (Vector3*)&worlddm.m[1];
	Vector3* m2 = (Vector3*)&worlddm.m[2];
	Vector3* m3 = (Vector3*)&worlddm.m[3];
	/*worlddm.Decompose(scale, rotp, pos);*/

	pos = *m3;

	scale = Vector3(m0->Length(), m1->Length(), m2->Length());

	m0->Normalize();
	m1->Normalize();
	m2->Normalize();

	rotp = Quaternion::CreateFromRotationMatrix(worlddm);


	obj.SetParentMatrix(nullptr);
	obj.SetScale(scale);
	obj.SetRotQ(rotp);
	obj.SetTrans(pos);
	g--;
	m_BulletVel = Vector3(0.0f, /*g*0.001f + g / 20*/0, -0.5f);

	m_BulletVel = Vector3::Transform(m_BulletVel, rotp);




	m_flag = true;
}

void BulletShot::ResetBullet(const DirectX::SimpleMath::Matrix * Parent)
{

	if (m_flag==false)return;


	obj.SetParentMatrix(Parent);
	obj.SetTrans(Vector3(0, 0, 0));


	m_flag = false;
}

const DirectX::SimpleMath::Vector3 & BulletShot::GetTrans()
{
	// TODO: return ステートメントをここに挿入します
	return obj.GetTrans();
}

void BulletShot::SetTrans(const DirectX::SimpleMath::Vector3 & trans)
{
	obj.SetTrans(trans);
}

const DirectX::SimpleMath::Matrix & BulletShot::GetLocalWorld()
{
	// TODO: return ステートメントをここに挿入します
	return obj.GetLocalWorld();
}






