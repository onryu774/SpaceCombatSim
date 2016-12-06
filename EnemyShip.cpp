#include"EnemyShip.h"
#include<SimpleMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
EnemyShip::EnemyShip()
{
}

EnemyShip::~EnemyShip()
{
}

void EnemyShip::Initialaiz(const int x, const int y, const int z)
{
	
	obj.LoadModelFile(L"Resources/cModels/Enemyfighter.cmo");
	obj.OnQuaater();
	obj.SetScale(Vector3(1.0f, 1.0f, 1.0f));
	m_x = x;
	m_y = y;
    m_z = z;
	obj.SetTrans(Vector3(m_x,m_y,m_z));
	Ptage = Vector3(0, 0, 0);
	collisionNodeBody.Initialize();
	collisionNodeBody.SetParentMatrix(&obj.GetLocalWorld());
	collisionNodeBody.SetRot(Vector3(XMConvertToRadians(90),0.0f,00.0f));
	collisionNodeBody.SetLocalRadius(0.2f);
	collisionNodeBody.SetLocalLength(1.5f);
	collisionNodeBody.SetTrans(Vector3(0.0f, 0.0f, -0.7f));
	m_Lifes = 2;
}

void EnemyShip::Update(const Vector3 Player_Pos)
{
	bool insite;
	insite = false;
	Vector3 trans = obj.GetTrans();
	
	Vector3 move;
	move = Vector3(0.0f, 0.0f, 0.1f);
	Matrix roteS ;
	insite = EnemyView(Player_Pos, move);
	roteS = LooKAtMatrix(Player_Pos);
	Quaternion RotM;
	RotM= Quaternion::CreateFromRotationMatrix(roteS);
	
	
	
	move = Vector3::TransformNormal(move,roteS);
	trans = trans + move;
	if (insite == true)
	{
		obj.SetTrans(trans);
	}
	obj.SetRotQ(RotM);
	Calc();
	collisionNodeBody.Update();
	
}

void EnemyShip::Calc()
{
	obj.Calc();
}

void EnemyShip::Draw()
{
	obj.Draw();
	collisionNodeBody.Draw();
}

const DirectX::SimpleMath::Vector3 & EnemyShip::GetTrans()
{
	// TODO: return ステートメントをここに挿入します
	return obj.GetTrans();
}

const DirectX::SimpleMath::Vector3 & EnemyShip::GetRot()
{
	// TODO: return ステートメントをここに挿入します
	return obj.GetRot();
}

void EnemyShip::SetTrans(const DirectX::SimpleMath::Vector3 & trans)
{
	obj.SetTrans(trans);
}

bool EnemyShip::Livese()
{
	if (m_Lifes<=0)
	{
		return false;
	}
	else
	{
		return true;
	}
	/*return true;*/
}

const DirectX::SimpleMath::Matrix & EnemyShip::GetLocalWorld()
{
	// TODO: return ステートメントをここに挿入します
	return obj.GetLocalWorld();
}

bool EnemyShip::EnemyView(const Vector3 Player_Pos,const Vector3 Enemymove)
{
	int halfradians;
	halfradians = 5;
	Vector3 Score;
	Score = Player_Pos - obj.GetTrans();
	float Scorelength;
	Scorelength = Score.Length();
	float Enemymovelength;
	Enemymovelength = Enemymove.Length();
	float cos = Score.Dot(Enemymove)/(Scorelength*Enemymovelength);
	float radians = acos(cos);
	if ((radians<=90)&&((obj.GetTrans().x-Player_Pos.x)*(obj.GetTrans().x - Player_Pos.x)+ (obj.GetTrans().z - Player_Pos.z)*(obj.GetTrans().z - Player_Pos.z)<=halfradians*halfradians)
		&& ((obj.GetTrans().y - Player_Pos.y)*(obj.GetTrans().x - Player_Pos.x) + (obj.GetTrans().z - Player_Pos.z)*(obj.GetTrans().z - Player_Pos.z) <= halfradians*halfradians))
	{
		return true;
	}
	return false;
}

void EnemyShip::NoFinedMove(int frem)
{

	
}

void EnemyShip::FinedMove()
{
}

DirectX::SimpleMath::Vector3 EnemyShip::Slep(const SimpleMath::Vector3 start,const SimpleMath::Vector3 end, float frem)
{
	Vector3 out;
	Vector3 StartVec;
	Vector3 EndVec;
	StartVec = start;
	EndVec = end;
	StartVec.Normalize();
	EndVec.Normalize();

	float angle; 
	angle= acos(StartVec.Dot(EndVec));
	float sinTh;
	sinTh = sin(angle);

	float Points = sin(angle*(1 - frem));
	float Pointe = sin(angle* frem);

	out = (Points*StartVec + Pointe*EndVec)/sinTh;

	return out;
}

DirectX::SimpleMath::Matrix EnemyShip::LooKAtMatrix(const DirectX::SimpleMath::Vector3 Player_Pos)
{
	Matrix Point;
	Vector3 pos; 
	pos = obj.GetTrans();;
	Vector3 tage;
	tage = Player_Pos ;
	Vector3 up; 
	up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 X;
	Vector3 Y;
	Vector3 V;
	Vector3 Z;
	V = tage - pos;
	V.Normalize();
	X = up.Cross(V);
	X.Normalize();
	Y = V.Cross(X);
	Y.Normalize();
	Z = X.Cross(Y);
	Point._11 = X.x;  Point._12 = X.y;	 Point._13 = X.z;  Point._14 = 0;
	Point._21 = Y.x;  Point._22 = Y.y;	 Point._23 = Y.z;  Point._24 = 0;
	Point._31 = V.x;  Point._32 = V.y;	 Point._33 = V.z;  Point._34 = 0;
	Point._41 = pos.x;  Point._42 = pos.y;	 Point._43 = pos.z;  Point._44 = 1.0f;
	return Point;
}

