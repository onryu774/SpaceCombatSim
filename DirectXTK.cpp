//--------------------------------------------------------------------------------------
// File: DirectXTK.cpp
//
// DirectXTKに関する関数群
//
// Date: 2015.8.27
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "Direct3D.h"
#include "DirectXTK.h"

using namespace DirectX;

//////////////////////////////////
// リンクするライブラリ指定		//
//////////////////////////////////
#pragma comment(lib, "DirectXTK.lib")

//////////////////////
// 静的変数			//
//////////////////////
static std::unique_ptr<Keyboard> s_keyboard(new Keyboard);
static std::unique_ptr<Mouse> s_mouse(new Mouse);

//////////////////////
// グローバル変数	//
//////////////////////

// キーボード関係
Keyboard::State g_key;
std::unique_ptr<Keyboard::KeyboardStateTracker> g_keyTracker(new Keyboard::KeyboardStateTracker);

// マウス関係
Mouse::State g_mouse;
std::unique_ptr<Mouse::ButtonStateTracker> g_mouseTracker(new Mouse::ButtonStateTracker);

// スプライトバッチ
std::unique_ptr<SpriteBatch> g_spriteBatch;

// スプライトフォント
std::unique_ptr<SpriteFont> g_spriteFont;

// コモンステート
std::unique_ptr <DirectX::CommonStates> g_state;

//--------------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------------
void DirectXTK_Initialize()
{
	// コモンステートを作成
	g_state.reset(new CommonStates(g_pd3dDevice.Get()));

	// スプライトバッチ
	g_spriteBatch.reset(new SpriteBatch(g_pImmediateContext.Get()));

	// スプライトフォント
	g_spriteFont.reset(new SpriteFont(g_pd3dDevice.Get(), L"myfile.spritefont"));
	/*s_mouse->SetWindow();*/
	/*s_mouse->SetMode(Mouse::Mode::MODE_ABSOLUTE);*/
}

//--------------------------------------------------------------------------------------
// キー入力やマウス情報の更新
//--------------------------------------------------------------------------------------
void DirectXTK_UpdateInputState()
{
	// キー入力情報を取得
	g_key = s_keyboard->GetState();
	g_keyTracker->Update(g_key);

	// マウス情報を取得
	g_mouse = s_mouse->GetState();
	g_mouseTracker->Update(g_mouse);
}

//--------------------------------------------------------------------------------------
// マウスのスクロールフォイール値のリセット関数
//--------------------------------------------------------------------------------------
void DirectXTK_ResetScrollWheelValue()
{
	s_mouse->ResetScrollWheelValue();
}
