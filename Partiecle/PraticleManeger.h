#pragma once
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>


class ParticleManager
{
public:
	ParticleManager();
	ParticleManager(ID3D11DeviceContext *context);
	ParticleManager(ID3D11DeviceContext *context, uint32_t num);
	virtual ~ParticleManager();

	// 初期化
	void Initialize();
	void Create(ID3D11DeviceContext *context);
	void Create(ID3D11DeviceContext *context, uint32_t num);

	// 更新
	void Update();

	// エントリー
	void Entry(
		int frame,
		float sx, float sy, float sz,
		float vx, float vy, float vz,
		float ax, float ay, float az,
		float startAngle, float endAngle,
		float startScale, float endScale,
		float startRed, float startGreen, float startBlue, float startAlpha,
		float endRed, float endGreen, float endBlue, float endAlpha
		);

	// 描画
	void Draw(DirectX::CXMMATRIX view, DirectX::CXMMATRIX proj);

	// テクスチャロード
	void LoadTexture(const wchar_t* filename);

	// テクスチャの設定
	void SetTexture(ID3D11ShaderResourceView* texture);

	// 重力の設定
	void SetGravity(float gx, float gy, float gz);

	// ビルボード行列の設定
	void SetBillboard(DirectX::CXMMATRIX billboadTransform);

	// ビルボードの自動設定
	void SetAutoBillboard();

	// エントリーのリセット
	void Reset();

	// 情報
	std::wstring ToString();

private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};