#pragma once
#include"Game\Obj3D.h"
#include"Game\LandShape.h"
#include"Game\CollisionNode.h"
#include"DirectXTK.h"
///////
//弾のクラス
///////
class BulletShot
{
private:
	//モデル
	Obj3D obj;
	//当たり判定
	CapsuleNode collisionNodeBullet;
	//弾の進むベクトル
	DirectX::SimpleMath::Vector3 m_BulletVel;
	bool m_flag;
public:
	BulletShot();
	~BulletShot();
	//初期化
	void Initialaiz(const DirectX::SimpleMath::Matrix*Parent);
	//更新
	void Update();
	///計算
	void Calc();
	//描画
	void Draw();
	//弾の再装填
	void ResetBullet(const DirectX::SimpleMath::Matrix*Parent);
	//クォータニオンを使用する
	void ONQ() { obj.OnQuaater(); };
	//クォータニオンを使用しない
	void OFFQ() { obj.OffQuaater(); };
	//弾を撃ったかどうか
	bool flage() { return m_flag; };
	const DirectX::SimpleMath::Vector3& GetTrans();
	const DirectX::SimpleMath::Vector3& BulletVel() { return m_BulletVel; };
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	//当たり判定の取得
	const CapsuleNode& GetCollisionNodeBullet() { return collisionNodeBullet; }
	void BulletUpdate() { collisionNodeBullet.Update(); };

};

