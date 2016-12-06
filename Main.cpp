//--------------------------------------------------------------------------------------
// File: Main.cpp
//
// ���C��
//
// ���l�F���̃v���O������DirectX11���g�p���邽�߂̍Œ�K�v�Ǝv���鏈����g�ݍ��񂾃T���v���ł�
//       �]���ȏ����͋L�q���Ă��܂���̂ŃI���W�i���t���[�����[�N�̃x�[�X�Ɏg�p���Ă�������
//
// Date: 2015.8.27
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include <Windows.h>

#include<ctime>
#include <DirectXColors.h>

#include "Direct3D.h"
#include "DirectXTK.h"
#include "ImaseLib\FPSTimer.h"

#include <SimpleMath.h>

#include "Grid.h"
#include "DebugCamera.h"
#include"Game\Obj3D.h"
#include"Game\Camera.h"
#include "Player.h"
#include"EnemyShip.h"
#include"Partiecle\PraticleManeger.h"
#include"Partiecle\TaskManager.h"
#include"BeamParticle.h"
#include"Contrail.h"

#include"PlayScene.h"
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

//////////////////
// �萔�錾		//
//////////////////
// �E�C���h�E�X�^�C��
static const DWORD WINDOW_STYLE = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

// ��ʉ𑜓x
static const int WINDOW_W = 640;
static const int WINDOW_H = 480;

//////////////////////////////
// �֐��̃v���g�^�C�v�錾	//
//////////////////////////////
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
float RandomRange(float min, float max)
{
	return static_cast<float>(rand()) / RAND_MAX * (max - min) + min;
}
//////////////////////
// �O���[�o���ϐ�	//
//////////////////////
HINSTANCE g_hInst = nullptr;	// �C���X�^���X�n���h��
static const int ENEMYS_NUM = 10;
HWND g_hWnd = nullptr;	// �E�C���h�E�n���h��



//--------------------------------------------------------------------------------------
// ���C��
//--------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	srand((unsigned int)time(NULL));
	PlayScene* playScene;
	playScene = new PlayScene;
	
	bool DoPlay;
	DoPlay = true;
	// 1�b�Ԃ�60�t���[���ŌŒ肷��
	ImaseLib::FPSTimer fpsTimer(60);

	// �E�C���h�E�̍쐬
	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	// DirectX�f�o�C�X�̏�����
	if (FAILED(Direct3D_InitDevice(g_hWnd)))
	{
		// �������Ɏ��s
		Direct3D_CleanupDevice();
		return 0;
	}

	EffectFactory*factory = new EffectFactory(g_pd3dDevice.Get());
	factory->SetDirectory(L"Resources/cModels");

	// DirectXTK�֌W�̏�����
	DirectXTK_Initialize();
	playScene->PlaySceneInit();
	//�^�X�N�������p�[�e�B�N���̎g�p���邽�߂̏�����

	// �O���b�h���I�u�W�F�N�g
	GridFloor gridFloor(20.0f, 20);

	// ���C�����[�v
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		int fps = fpsTimer.GetNowFPS();
		wchar_t buf[16];
		swprintf_s(buf, 16, L"fps = %d", fps);

		/*wchar_t buf2[16];
		swprintf_s(buf2, 16, L"TimeLimit = %d",Timelimit );*/
		// ���b�Z�[�W�����Ă��邩���ׂ�
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// ���b�Z�[�W�����Ă���΃E�C���h�E�v���V�[�W���֑���
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// �L�[���͂�}�E�X���̍X�V
			DirectXTK_UpdateInputState();

			if(!DoPlay==false)
			DoPlay = playScene->PlaySceneRun();
			// �o�b�N�o�b�t�@�̃N���A
			g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView.Get(), DirectX::Colors::MidnightBlue);

			// �[�x�o�b�t�@�̃N���A
			g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			//---------- �����ŕ`��R�}���h�𔭍s����@---------//
			g_spriteBatch->Begin();
			
			// �O���b�h���̕\��
			/*gridFloor.Render(view, proj);*/
			if (!DoPlay == false)
			playScene->PlaySceneRender();
			// fps�̕\��
			g_spriteFont->DrawString(g_spriteBatch.get(), buf, Vector2(0, 0));
			/*g_spriteFont->DrawString(g_spriteBatch.get(), buf2, Vector2(0, 10));*/
			g_spriteBatch->End();

			// ��ʍX�V��҂�
			fpsTimer.WaitFrame();

			// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���
			g_pSwapChain->Present(0, 0);
		}
	}
	/*delete[] Enemys;*/
	// DirectX�f�o�C�X����̏I������
	Direct3D_CleanupDevice();

	return 0;
}

//--------------------------------------------------------------------------------------
// �E�C���h�E�̍쐬
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// �E�C���h�E�N���X��o�^����
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)NULL);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"WindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)NULL);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// �E�C���h�E���쐬����
	g_hInst = hInstance;
	RECT rc = { 0, 0, WINDOW_W, WINDOW_H };	// ���E�C���h�E�T�C�Y
	AdjustWindowRect(&rc, WINDOW_STYLE, FALSE);
	g_hWnd = CreateWindow(L"WindowClass", L"3D�v���O���~���O��b", WINDOW_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!g_hWnd)
		return E_FAIL;

	// �E�C���h�E�̕\��
	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}

//--------------------------------------------------------------------------------------
// �E�C���h�E�v���V�[�W���[
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:		// �E�C���h�E���j�����ꂽ���b�Z�[�W
		PostQuitMessage(0);	// WM_QUIT���b�Z�[�W�𑗏o����i���C�����[�v�𔲂���j
		break;

	case WM_ACTIVATEAPP:	// �E�C���h�E���A�N�e�B�u���A��A�N�e�B�u�����鎞�ɗ��郁�b�Z�[�W
		Keyboard::ProcessMessage(message, wParam, lParam);
		Mouse::ProcessMessage(message, wParam, lParam);
		break;

	case WM_KEYDOWN:		// �L�[���͊֌W�̃��b�Z�[�W
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	case WM_INPUT:			// �}�E�X�֌W�̃��b�Z�[�W
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;

	default:
		// �����ŏ������Ȃ����b�Z�[�W��DefWindowProc�֐��ɔC����
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

