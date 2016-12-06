/////////////////////////////////
//敵の行動などの設定
//
//製作　恩田龍登
////////////////////////////////

#include"EnemyShip.h"
#include<SimpleMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

float RandomRange(float min, float max);


EnemyShip::EnemyShip()
{
}

EnemyShip::~EnemyShip()
{
}
//初期化処理
void EnemyShip::Initialaiz(const float x, const float y, const float z)
{
	//モデルの読み込み
	obj.LoadModelFile(L"Resources/cModels/Enemyfighter.cmo");
	//クォータニオンの使用
	obj.OnQuaater();
	//モデルのサイズ変更
	obj.SetScale(Vector3(1.0f, 1.0f, 1.0f));
	int m_x = x;
	int m_y = y;
	int m_z = z;
	//モデルの初期位置の設定
	obj.SetTrans(Vector3(m_x,m_y,m_z));
	//最初の目的地の設定
	Ptage = Vector3(RandomRange(-200, 200), RandomRange(-200, 200), RandomRange(-200, 200));
	//当たり判定の初期化
	collisionNodeBody.Initialize();
	//親行列の設定
	collisionNodeBody.SetParentMatrix(&obj.GetLocalWorld());
	//当たり判定の回転
	collisionNodeBody.SetRot(Vector3(XMConvertToRadians(90),0.0f,00.0f));
	//当たり判定の半径
	collisionNodeBody.SetLocalRadius(0.2f);
	//当たり判定の長さ
	collisionNodeBody.SetLocalLength(1.5f);
	//当たり判定の位置設定
	collisionNodeBody.SetTrans(Vector3(0.0f, 0.0f, -0.7f));
	//ライフの設定
	m_Lifes = 2;
	
	insite = false;
}
//更新処理
void EnemyShip::Update(const Vector3 Tage_Pos)
{
	//モデルの位置を取得
	Vector3 trans = obj.GetTrans();

	Quaternion RotQ;
	//進むベクトルを設定
	move = Vector3(0.0f, 0.0f, 0.1f);
	//自機を見つけているかどうか
	insite = EnemyView(Tage_Pos, move);
	m_frame++;
	if (m_frame >= 60)
	{
		m_time++;
		m_frame = 0;
	}
	

	//行動パターンの変更
	if (insite == true)
	{
		RotQ=FinedMove(Tage_Pos);
	}
	else if(insite==false)
	{
		RotQ=NoFinedMove();
	}
	//移動の計算
	trans = trans + move;
	obj.SetTrans(trans);
	obj.SetRotQ(RotQ);
	Calc();
	//当たり判定の更新
	collisionNodeBody.Update();
	
}
//位置などの計算
void EnemyShip::Calc()
{
	obj.Calc();
}
//描写
void EnemyShip::Draw()
{
	obj.Draw();
	collisionNodeBody.Draw();
}
//位置の取得
const DirectX::SimpleMath::Vector3 & EnemyShip::GetTrans()
{
	// TODO: return ステートメントをここに挿入します
	return obj.GetTrans();
}
//回転の取得
const DirectX::SimpleMath::Vector3 & EnemyShip::GetRot()
{
	// TODO: return ステートメントをここに挿入します
	return obj.GetRot();
}
//位置の設定
void EnemyShip::SetTrans(const DirectX::SimpleMath::Vector3 & trans)
{
	obj.SetTrans(trans);
}
//生きているかどうか
//true:生きている
//false:生きていない
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
//ローカルワールド行列の取得
const DirectX::SimpleMath::Matrix & EnemyShip::GetLocalWorld()
{
	// TODO: return ステートメントをここに挿入します
	return obj.GetLocalWorld();
}
//視野
//true:プレイヤーを見つけれている
//false：プレイヤーを見つけていない
bool EnemyShip::EnemyView(const Vector3 Tage_Pos,const Vector3 Enemymove)
{
	//視野範囲
	int halfradians;
	halfradians = 10;
	//ターゲットへのベクトル
	Vector3 Score;
	Score = Tage_Pos - obj.GetTrans();
	//ターゲットへのベクトルの長さ
	float Scorelength;
	Scorelength = Score.Length();
	//敵の進んでいるベクトルの長さ
	float Enemymovelength;
	Enemymovelength = Enemymove.Length();
	//二つのベクトルのなす角を求める（cosin）
	float cos = Score.Dot(Enemymove)/(Scorelength*Enemymovelength);
	//cosinをラジアン角に変換
	float radians = acos(cos);
	//プレイヤーを見つけているかどうかの判定
	if ((radians<=90)&&((obj.GetTrans().x-Tage_Pos.x)*(obj.GetTrans().x - Tage_Pos.x)+ (obj.GetTrans().z - Tage_Pos.z)*(obj.GetTrans().z - Tage_Pos.z)<=halfradians*halfradians)
		&& ((obj.GetTrans().y - Tage_Pos.y)*(obj.GetTrans().x - Tage_Pos.x) + (obj.GetTrans().z - Tage_Pos.z)*(obj.GetTrans().z - Tage_Pos.z) <= halfradians*halfradians))
	{
		return true;
	}
	return false;
}
//プレイヤーを見つけれていないときの行動
DirectX::SimpleMath::Quaternion EnemyShip::NoFinedMove()
{
	
	//2秒ごとに目的地の設定
	if (m_time>=2)
	{
		Ptage = Vector3(RandomRange(-200, 200), RandomRange(-200, 200), RandomRange(-200, 200));
		m_time = 0;
	}
	//目的地への回転行列の設定
	Matrix rote =LooKAtMatrix(Ptage);
	Quaternion RotQ;
	//マトリックスをクォータニオンに変換
	RotQ = Quaternion::CreateFromRotationMatrix(rote);
	//マトリックスをVector3に変換
	move = Vector3::TransformNormal(move, rote);
	
	return RotQ;
	
}
//プレイヤーを見つけているときの行動
DirectX::SimpleMath::Quaternion EnemyShip::FinedMove(const Vector3 Tage_Pos)
{

	Matrix roteS;
	//プレイヤーへの回転行列
	roteS = LooKAtMatrix(Tage_Pos);
	Quaternion RotQ;
	// マトリックスをクォータニオンに変換
	RotQ = Quaternion::CreateFromRotationMatrix(roteS);
	//マトリックスをVector3に変換
	move = Vector3::TransformNormal(move, roteS);
	return RotQ;
}


//回転行列の計算
DirectX::SimpleMath::Matrix EnemyShip::LooKAtMatrix(const DirectX::SimpleMath::Vector3 Tage_Pos)
{
	//回転行列の格納
	Matrix Point;
	//現在地
	Vector3 pos; 
	pos = obj.GetTrans();;
	//目的地
	Vector3 tage;
	tage = Tage_Pos ;
	Vector3 up; 
	up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 X;
	Vector3 Y;
	Vector3 V;
	Vector3 Z;
	//軸の計算
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

