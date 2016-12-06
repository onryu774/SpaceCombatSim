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
	//初期化
	void Initialaiz();
	//更新処理
	void Update();
	//計算
	void Calc();
	//描写
	void Draw();
	//位置の取得
	const DirectX::SimpleMath::Vector3& GetTrans();
	//弾の位置の取得
	const DirectX::SimpleMath::Vector3& GetBulletTrans();
	//回転の取得
	const DirectX::SimpleMath::Vector3& GetRot();
	//位置の設定
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	//弾の再装填
	void ResetBullet() { Bullet->ResetBullet(&obj.GetLocalWorld()); }
	//ローカルワールド行列の取得
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	//自機のあたり判定の取得
	const CapsuleNode& GetCollisionNodeBullet() { return collisionNodeBullet; }
	//弾のあたり判定の取得
	const SphereNode& GetCollisionNodeBody() { return collisionNodeBody; }
protected:
	//プレイヤーのモデル
	Obj3D obj;
	//ターゲットサイトのモデル
	Obj3D Tage;
private:
	//弾のクラス
	BulletShot* Bullet;
	//弾が打たれたかどうか
	bool m_timeflag;
	//弾の進むベクトル
	DirectX::SimpleMath::Vector3 m_BulletVel;
	//弾が発射されてからのフレーム
	int m_frame;
	//弾が発射されてからの秒数
	int m_time;
	//弾のあたり判定
	CapsuleNode collisionNodeBullet;
	//自機のあたり判定
	SphereNode  collisionNodeBody;


};

