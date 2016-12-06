//--------------------------------------------------------------------------------------
// File: Line.h
//
// ラインを描画するクラス
//
// Date: 2015.10.18
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include <Windows.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <SimpleMath.h>
#include <DirectXColors.h>

// ラインクラス
class LineBatch
{
	// エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

	// インプットレイアウト
	ID3D11InputLayout* m_pInputLayout;

	// 画面サイズ
	int m_w, m_h;

	// グリッドサイズ
	int m_size;

public:
	// コンストラクタ
	LineBatch(int w, int h, int size);

	// デストラクタ
	~LineBatch();

	// 描画開始
	void Begin();
	void Begin(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// 描画終了
	void End();

	// 描画
	void Draw(float x1, float y1, float x2, float y2, DirectX::XMVECTOR color = DirectX::Colors::White);

	// 描画
	void Draw(DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::XMVECTOR color = DirectX::Colors::White);

	// 描画
	void DrawLine(DirectX::SimpleMath::Vector2 p1, DirectX::XMVECTOR color = DirectX::Colors::White);

	// 描画
	void DrawLine(DirectX::SimpleMath::Vector2 p1, DirectX::SimpleMath::Vector2 p2, DirectX::XMVECTOR color = DirectX::Colors::White);

	// 描画
	void Draw(DirectX::SimpleMath::Vector3 p, DirectX::XMVECTOR color = DirectX::Colors::White);

	// 描画
	void Draw(DirectX::SimpleMath::Vector3 p1, DirectX::SimpleMath::Vector3 p3, DirectX::XMVECTOR color = DirectX::Colors::White);
};
