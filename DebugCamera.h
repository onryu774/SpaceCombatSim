//--------------------------------------------------------------------------------------
// File: DebugCamera.h
//
// デバッグ用カメラクラス
//
// Date: 2015.11.23
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "DirectXTK.h"
#include <SimpleMath.h>

// デバッグ用カメラクラス
class DebugCamera
{
	// カメラの距離
	static const float DEFAULT_CAMERA_DISTANCE;

	// 横回転
	float m_yAngle, m_yTmp;

	// 縦回転
	float m_xAngle, m_xTmp;

	// ドラッグされた座標
	int m_x, m_y;

	float m_sx, m_sy;

	// 生成されたビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// スクロールフォイール値
	int m_scrollWheelValue;

private:

	void Motion(int x, int y);

public:
	// コンストラクタ
	DebugCamera(int w, int h)
		: m_yAngle(0.0f), m_yTmp(0.0f), m_xAngle(0.0f), m_xTmp(0.0f), m_x(0), m_y(0), m_scrollWheelValue(0)
	{
		// 画面サイズに対する相対的なスケールに調整
		m_sx = 1.0f / (float)w;
		m_sy = 1.0f / (float)h;
		m_view = DirectX::SimpleMath::Matrix::Identity;
	}

	// 更新
	void Update();

	DirectX::SimpleMath::Matrix GetCameraMatrix()
	{
		return m_view;
	}
};
