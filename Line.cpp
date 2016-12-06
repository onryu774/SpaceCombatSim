//--------------------------------------------------------------------------------------
// File: Line.cpp
//
// ラインを描画するクラス
//
// Date: 2015.10.18
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include <SimpleMath.h>
#include <DirectXColors.h>
#include "Line.h"
#include "Direct3D.h"
#include "DirectXTK.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//--------------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------------
LineBatch::LineBatch(int w, int h, int size) : m_w(w), m_h(h), m_size(size)
{
	// エフェクトオブジェクト生成
	m_basicEffect.reset(new BasicEffect(g_pd3dDevice.Get()));
	// 頂点カラー(有効)
	m_basicEffect->SetVertexColorEnabled(true);
	// プリミティブオブジェクト生成
	m_primitiveBatch.reset(new PrimitiveBatch<VertexPositionColor>(g_pImmediateContext.Get()));
	// インプットレイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	g_pd3dDevice.Get()->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		&m_pInputLayout);
}

//--------------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------------
LineBatch::~LineBatch()
{
	// 入力レイアウトを解放する
	if (m_pInputLayout) m_pInputLayout->Release();
}

//--------------------------------------------------------------------------------------
// 描画の開始
//--------------------------------------------------------------------------------------
void LineBatch::Begin()
{
	Matrix world, view, proj;

	// ２Ｄ用行列を作成してエフェクトへセット
	world = Matrix::Identity;
	view = Matrix::Identity;
	proj = Matrix::CreateTranslation(0.5f, 0.5f, 0.0f) * Matrix::CreateOrthographicOffCenter(0.0f, (float)m_w, (float)m_h, 0.0f, 0.0f, 100.0f);

	m_basicEffect->SetWorld(world);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);

	m_basicEffect->Apply(g_pImmediateContext.Get());
	g_pImmediateContext.Get()->IASetInputLayout(m_pInputLayout);

	m_primitiveBatch->Begin();
}

//--------------------------------------------------------------------------------------
// 描画の開始
//--------------------------------------------------------------------------------------
void LineBatch::Begin(Matrix view, Matrix proj)
{
	Matrix world;

	world = Matrix::Identity;

	m_basicEffect->SetWorld(world);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);

	m_basicEffect->Apply(g_pImmediateContext.Get());
	g_pImmediateContext.Get()->IASetInputLayout(m_pInputLayout);

	m_primitiveBatch->Begin();
}

//--------------------------------------------------------------------------------------
// 描画の終了
//--------------------------------------------------------------------------------------
void LineBatch::End()
{
	m_primitiveBatch->End();
}

//--------------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------------
void LineBatch::Draw(float x1, float y1, float x2, float y2, XMVECTOR color)
{
	Draw(Vector2(x1, y1), Vector2(x2, y2), color);
}

//--------------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------------
void LineBatch::Draw(Vector2 p1, Vector2 p2, XMVECTOR color)
{
	VertexPositionColor v1(p1, color);
	VertexPositionColor v2(p2, color);
	m_primitiveBatch->DrawLine(v1, v2);
}

//--------------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------------
void LineBatch::DrawLine(Vector2 p1, XMVECTOR color)
{
	VertexPositionColor v1((Vector2(p1.x * (float)m_size, -p1.y * (float)m_size) + Vector2(m_w / 2.0f, m_h / 2.0f)), color);
	VertexPositionColor v2(Vector2(m_w / 2.0f, m_h / 2.0f), color);
	m_primitiveBatch->DrawLine(v1, v2);
}

//--------------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------------
void LineBatch::DrawLine(Vector2 p1, Vector2 p2, XMVECTOR color)
{
	VertexPositionColor v1((Vector2(p1.x * (float)m_size, -p1.y * (float)m_size) + Vector2(m_w / 2.0f, m_h / 2.0f)), color);
	VertexPositionColor v2((Vector2(p2.x * (float)m_size, -p2.y * (float)m_size) + Vector2(m_w / 2.0f, m_h / 2.0f)), color);
	m_primitiveBatch->DrawLine(v1, v2);
}

//--------------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------------
void LineBatch::Draw(Vector3 p, XMVECTOR color)
{
	VertexPositionColor v1(Vector3(0.0f, 0.0f, 0.0f), color);
	VertexPositionColor v2(p, color);
	m_primitiveBatch->DrawLine(v1, v2);
}

//--------------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------------
void LineBatch::Draw(Vector3 p1, Vector3 p2, XMVECTOR color)
{
	VertexPositionColor v1(p1, color);
	VertexPositionColor v2(p2, color);
	m_primitiveBatch->DrawLine(v1, v2);
}

