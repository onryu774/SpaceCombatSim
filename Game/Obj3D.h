#pragma once
#include<Model.h>
#include<Effects.h>
#include"Camera.h"
#include<map>
class Obj3D
{
private:
	// 静的メンバ変数（全オブジェクトで共有）
	// デバイスへのポインタ
	static ID3D11Device* s_pDevice;
	// デバイスコンテキストへのポインタ
	static ID3D11DeviceContext* Obj3D::s_pDeviceContext;
	// 描画ステートへのポインタ
	static DirectX::CommonStates* Obj3D::s_pStates;
	// 共用のエフェクトファクトリ
	static DirectX::EffectFactory* s_pEffectFactory;
	// 共用のカメラ（描画時に使用）
	static Camera* s_pCamera;
	// 読み込み済みモデルコンテナ
	static std::map<std::wstring, std::unique_ptr<DirectX::Model>> s_modelarray;

	// モデルデータへのポインタ
	const DirectX::Model* m_pModel;
	// 平行移動
	DirectX::SimpleMath::Vector3 m_Trans;
	// 回転
	DirectX::SimpleMath::Vector3 m_Rot;
	// スケーリング
	DirectX::SimpleMath::Vector3 m_Scale;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_LocalWorld;
	// 親のワールド行列へのポインタ
	const DirectX::SimpleMath::Matrix* m_pParentMatrix;
	//クォータニオンを持つか
	bool m_UseQuaternion;

	DirectX::SimpleMath::Quaternion m_quaternipn;

public:
	static void SetDevice(ID3D11Device* pDevice) { s_pDevice = pDevice; }
	static void SetDeviceContext(ID3D11DeviceContext* pDeviceContext) { s_pDeviceContext = pDeviceContext; }
	static void SetStates(DirectX::CommonStates* pStates) { s_pStates = pStates; }
	static void SetEffectFactory(DirectX::EffectFactory* pfx) { s_pEffectFactory = pfx; }
	// カメラのsetter
	static void SetCamera(Camera* pCamera) {s_pCamera = pCamera; }

	Obj3D();
	virtual ~Obj3D();

	// ファイルからモデルを読み込む
	void LoadModelFile(const wchar_t*filename = nullptr);
	// 行列の計算
	void Calc();
	// 描画
	void Draw();
	// オブジェクトのライティングを無効にする
	void DisableLighting();
	// アクセッサ
	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_Trans = trans; }
	void SetRot(const DirectX::SimpleMath::Vector3& rot) { m_Rot = rot; }
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_Scale = scale; }
	void SetLocalWorld(const DirectX::SimpleMath::Matrix& mat) { m_LocalWorld = mat; }
	void SetParentMatrix(const DirectX::SimpleMath::Matrix* pParentMatrix) { m_pParentMatrix = pParentMatrix; }
	const DirectX::SimpleMath::Vector3& GetTrans() { return m_Trans; }
	const DirectX::SimpleMath::Vector3& GetRot() { return m_Rot; }
	const DirectX::SimpleMath::Vector3& GetScale() { return m_Scale; }
	const DirectX::SimpleMath::Matrix& GetLocalWorld() { return m_LocalWorld; }
	void SetRotQ(const DirectX::SimpleMath::Quaternion& rot) { m_quaternipn = rot; }
	DirectX::SimpleMath::Quaternion GetRotQ() {return m_quaternipn; }
	void OnQuaater() { m_UseQuaternion = true; }
	void OffQuaater() { m_UseQuaternion = false; }
};

