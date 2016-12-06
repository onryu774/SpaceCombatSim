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
	//初期化
	void Initialaiz(const float x,const float y,const float z);
	//更新
	void Update(const DirectX::SimpleMath::Vector3 Tage_Pos);
	//モデルなどの計算
	void Calc();
	//描写
	void Draw();
	//位置の取得
	const DirectX::SimpleMath::Vector3& GetTrans();
	//回転の取得
	const DirectX::SimpleMath::Vector3& GetRot();
	//位置の設定
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	//ダメージを受ける
	void Damegy() { m_Lifes--;}
	//生きているかどうか
	bool Livese();
	//ローカルワールド行列の取得
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	//当たり判定の取得
	const CapsuleNode& GetCollisionNodeBody() { return collisionNodeBody; }
	
protected:
	//3Dモデル
	Obj3D obj;
private:
	//回転行列の計算
	DirectX::SimpleMath::Matrix LooKAtMatrix(const DirectX::SimpleMath::Vector3 Tage_Pos);
	//目的地のベクトル
	DirectX::SimpleMath::Vector3 Ptage;
	//進む方向ベクトル
	DirectX::SimpleMath::Vector3 move;
	//プレイヤーを見つけているかどうか判定
	bool EnemyView(const DirectX::SimpleMath::Vector3 Tage_Pos, const DirectX::SimpleMath::Vector3 EnemyMove);
	//プレイヤーを見つけていないときの行動
    DirectX::SimpleMath::Quaternion NoFinedMove();
	//プレイヤーを見つけているときの行動
	DirectX::SimpleMath::Quaternion FinedMove(const DirectX::SimpleMath::Vector3 Tage_Pos);
	//プレイヤーを見つけているかどうか
	bool insite;
	//フレーム
	int m_frame;
	//時間
	int m_time;
	//ライフ
	int m_Lifes;
	//当たり判定
	CapsuleNode  collisionNodeBody;
};