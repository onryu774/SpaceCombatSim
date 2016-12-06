//--------------------------------------------------------------------------------------
// File: DirectXTK.h
//
// DirectXTKに関する関数群
//
// Date: 2015.8.27
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include <Windows.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <SpriteFont.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <CommonStates.h>

//////////////////////////////
// 関数のプロトタイプ宣言	//
//////////////////////////////
void DirectXTK_Initialize();
void DirectXTK_UpdateInputState();
void DirectXTK_ResetScrollWheelValue();

//////////////////////////////
// グローバル変数			//
//////////////////////////////

// キーボード関係
extern DirectX::Keyboard::State g_key;
extern std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> g_keyTracker;

// マウス関係
extern DirectX::Mouse::State g_mouse;
extern std::unique_ptr<DirectX::Mouse::ButtonStateTracker> g_mouseTracker;

// コモンステート
extern std::unique_ptr < DirectX::CommonStates> g_state;

// スプライトバッチ
extern std::unique_ptr<DirectX::SpriteBatch> g_spriteBatch;

// スプライトフォント
extern std::unique_ptr<DirectX::SpriteFont> g_spriteFont;


