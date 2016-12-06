#include "PraticleManeger.h"
#include <memory>
#include <wrl.h>
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include "DirctXHelper.h"
#include <cstdio>
#include <sstream>
#include <array>

namespace
{
	//	char* VertexShader;   // ダミー
	//	char* GeometryShader; // ダミー
	//	char* PixelShader;    // ダミー
#include "ParticleVS.h"
#include "ParticleGS.h"
#include "ParticlePS.h"
	auto &VertexShader = ParticleVS;
	auto &GeometryShader = ParticleGS;
	auto &PixelShader = ParticlePS;
}


using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

// ポイントスプライトの定義
namespace
{
	class PointSprite
	{
	public:
		PointSprite(ID3D11DeviceContext* context, uint32_t num) : context(context), isBillboardAutoUpdate(false), vertex(num)
		{
			Reset();
		}
		~PointSprite() {}

		ComPtr<ID3D11DeviceContext> context; // コンテキスト
		ComPtr<ID3D11Device> device; // デバイス

									 // Vertex buffer
		ComPtr<ID3D11InputLayout> inputLayout; // インプットレイアウト
		ComPtr<ID3D11Buffer> vertexBuffer; // 頂点バッファ
		ComPtr<ID3D11Buffer> indexBuffer; // インデックスバファ

										  // shader
		ComPtr<ID3D11VertexShader> vertexShader;     // 頂点シェーダ
		ComPtr<ID3D11GeometryShader> geometryShader; // ジオメトリシェーダ
		ComPtr<ID3D11PixelShader> pixelShader;       // ピクセルシェーダ

													 // constant buffer
		ComPtr<ID3D11Buffer> matrixesBuffer; // 行列関連のコンスタントバッファ

											 // 
		ComPtr<ID3D11RasterizerState> rasterizerState;      // ラスタライザーステート
		ComPtr<ID3D11BlendState> blendState;                // ブレンドステート
		ComPtr<ID3D11DepthStencilState> depthStencilState;  // 深度ステンシルステート

															// テクスチャ
		ComPtr<ID3D11ShaderResourceView> textureView;  // テクスチャ
		ComPtr<ID3D11SamplerState>       samplerState; // サンプラー


		struct GSVertexInfoType
		{
			uint32_t numVertex;
		};

		struct GSVertexType
		{
			Vector4 pos;
			Vector2 uv;
		};

		std::array<GSVertexType, 4> gsVertexs;

		ComPtr<ID3D11Buffer> gsVertexInfoBuffer; // 情報
		ComPtr<ID3D11Buffer> gsVertexsBuffer;     // 構造体
		ComPtr<ID3D11ShaderResourceView> gsVertexsView;


		bool isBillboardAutoUpdate;   // ビルボードの自動更新

									  // コンスタントバッファに送る
		struct MatrixesBufferType
		{
			Matrix world;
			Matrix view;
			Matrix proj;
			Matrix billboard;
		};
		MatrixesBufferType matrixes;

		// プリミティブの最大数
		//static const int num = 100000;

		// 頂点データ
		struct Vertex
		{
			Vector4 position;
			float angle;
			float scale;
			Vector4 color;
		};
		//Vertex vertex[num];
		vector<Vertex> vertex;

		bool LoadTexture(const wchar_t* filename)
		{
			wstring str;
			str += wstring(filename) + wstring(L".dds");

			// テクスチャの読み込み
			// ここでのエラーは、テクスチャファイルの格納場所や拡張子をチェックしてください。
			DX::ThrowIfFailed(CreateDDSTextureFromFile(device.Get(), str.c_str(), nullptr, &textureView));

			return true;
		}

		bool SetTexture(ID3D11ShaderResourceView* texture)
		{
			textureView = texture;
			return true;
		}


		bool Initialize()
		{
			this->context = context;

			context->GetDevice(&device);

			// 頂点レイアウトの作成
			D3D11_INPUT_ELEMENT_DESC elements[] =
			{
				{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 1, DXGI_FORMAT_R32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			DX::ThrowIfFailed(device->CreateInputLayout(elements, ARRAYSIZE(elements), VertexShader, sizeof(VertexShader), &inputLayout));

			// バーテックスシェーダの作成
			DX::ThrowIfFailed(device->CreateVertexShader(VertexShader, sizeof(VertexShader), nullptr, &vertexShader));

			// ジオメトリシェーダの作成
			DX::ThrowIfFailed(device->CreateGeometryShader(GeometryShader, sizeof(GeometryShader), nullptr, &geometryShader));

			// ピクセルシェーダの作成
			DX::ThrowIfFailed(device->CreatePixelShader(PixelShader, sizeof(PixelShader), nullptr, &pixelShader));


			// コンスタントバッファを作成
			// 設定項目：コンスタントバッファ、サイズはMatrixesBufferType
			D3D11_BUFFER_DESC matrixesBufferDesc = {};
			matrixesBufferDesc.ByteWidth = sizeof(MatrixesBufferType);
			matrixesBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			matrixesBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			matrixesBufferDesc.CPUAccessFlags = 0;
			matrixesBufferDesc.MiscFlags = 0;
			matrixesBufferDesc.StructureByteStride = sizeof(DWORD);
			DX::ThrowIfFailed(device->CreateBuffer(&matrixesBufferDesc, nullptr, &matrixesBuffer));

			// ジオメトリシェーダの形状データを収めるバッファ
			D3D11_BUFFER_DESC gsVertexBufferDesc = {};
			gsVertexBufferDesc.ByteWidth = sizeof(GSVertexType) * 4; // データサイズ
			gsVertexBufferDesc.BindFlags =
				D3D11_BIND_UNORDERED_ACCESS |  // ランダムアクセスにバインド
				D3D11_BIND_SHADER_RESOURCE;    // シェーダリソースにバインド
			gsVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			gsVertexBufferDesc.CPUAccessFlags = 0;
			gsVertexBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 構造体バッファとして作成
			gsVertexBufferDesc.StructureByteStride = sizeof(GSVertexType);

			// 形状データ
			gsVertexs[0].pos = Vector4(-0.5f, 0.5f, 0.0f, 0.0f);
			gsVertexs[0].uv = Vector2(0.0f, 0.0f);
			gsVertexs[1].pos = Vector4(0.5f, 0.5f, 0.0f, 0.0f);
			gsVertexs[1].uv = Vector2(1.0f, 0.0f);
			gsVertexs[2].pos = Vector4(-0.5f, -0.5f, 0.0f, 0.0f);
			gsVertexs[2].uv = Vector2(0.0f, 1.0f);
			gsVertexs[3].pos = Vector4(0.5f, -0.5f, 0.0f, 0.0f);
			gsVertexs[3].uv = Vector2(1.0f, 1.0f);
			D3D11_SUBRESOURCE_DATA srd = {};
			srd.pSysMem = gsVertexs.data();     // データポインタ
			srd.SysMemPitch = gsVertexs.size(); // バイト数
			srd.SysMemSlicePitch = 0;
			// バッファの作成
			DX::ThrowIfFailed(device->CreateBuffer(&gsVertexBufferDesc, &srd, &gsVertexsBuffer));


			// バッファからビューを作成する
			D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
			srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX; // 拡張バッファ
			srvd.BufferEx.FirstElement = 0;
			srvd.BufferEx.NumElements = 4; // 要素数
			srvd.Format = DXGI_FORMAT_UNKNOWN;
			DX::ThrowIfFailed(device->CreateShaderResourceView(gsVertexsBuffer.Get(), &srvd, &gsVertexsView));

			// ラスタライザの作成
			//  設定項目：面描画、カリングなし、深度クリップあり、マルチアンプリングあり
			D3D11_RASTERIZER_DESC rasterizerDesc = {};
			rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
			rasterizerDesc.DepthClipEnable = true;
			rasterizerDesc.MultisampleEnable = true;
			DX::ThrowIfFailed(device->CreateRasterizerState(&rasterizerDesc, &rasterizerState));


			// 深度ステンシル作成
			// 設定項目：深度バッファあり、深度書き込みなし、Z値が小さい時描画、ステンシルを使わない
			D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
			depthStencilDesc.DepthEnable = TRUE;
			depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
			depthStencilDesc.StencilEnable = FALSE;
			depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
			//depthStencilDesc.FrontFace;
			//depthStencilDesc.BackFace;
			DX::ThrowIfFailed(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));

			// ブレンド作成(加算合成)
			//  設定項目：アルファブレンド使う、レンダーターゲットにすべての情報を書き込む、加算合成
			D3D11_BLEND_DESC blendDesc = {};
			blendDesc.AlphaToCoverageEnable = false;
			blendDesc.IndependentBlendEnable = false;
			//			blendDesc.RenderTarget[0].BlendEnable = false;
			//			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = true;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			DX::ThrowIfFailed(device->CreateBlendState(&blendDesc, &blendState));


			// 頂点バッファを作成
			// 設定項目：頂点のバッファ、サイズは最大数、float単位のアクセス
			D3D11_BUFFER_DESC vertexBufferDesc;
			vertexBufferDesc.ByteWidth = sizeof(Vertex)*vertex.size();
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = sizeof(float);
			//		D3D11_SUBRESOURCE_DATA vertexData;
			//		vertexData.pSysMem = vertexs;
			//		vertexData.SysMemPitch = 0;
			//		vertexData.SysMemSlicePitch = 0;
			//		device->CreateBuffer(&vertexBufferDesc,&vertexData,&vertexBuffer);
			DX::ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, nullptr, &vertexBuffer));


			D3D11_SAMPLER_DESC samplerDesc = {};
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerDesc.MipLODBias = 0;                            // 計算されたミップマップ レベルからのバイアス
			samplerDesc.MaxAnisotropy = 16;                        // サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 。
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // 比較オプション。
			samplerDesc.BorderColor[0] = 0.0f; // 境界色
			samplerDesc.BorderColor[1] = 0.0f; // 境界色
			samplerDesc.BorderColor[2] = 0.0f; // 境界色
			samplerDesc.BorderColor[3] = 0.0f; // 境界色
			samplerDesc.MinLOD = 0;                                // アクセス可能なミップマップの下限値
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                // アクセス可能なミップマップの上限値
			DX::ThrowIfFailed(device->CreateSamplerState(&samplerDesc, &samplerState));

			return true;
		}

		int index;

		void Add(float x, float y, float z, float angle, float scale, float r, float g, float b, float a)
		{
			auto &v = vertex[index];
			v.position = Vector4(x, y, z, 1);
			v.angle = angle;
			v.scale = scale;
			v.color = Vector4(r, g, b, a);

			index++;
		}

		void Reset()
		{
			index = 0;
		}

		void Applay()
		{
			context->UpdateSubresource(vertexBuffer.Get(), 0, nullptr, vertex.data(), 0, 0);
		}

		void Draw()
		{
			// コンスタントバッファのアップデート
			context->UpdateSubresource(matrixesBuffer.Get(), 0, nullptr, &matrixes, 0, 0);

			// 形状の設定
			context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

			// インプットレイアウトを設定
			context->IASetInputLayout(inputLayout.Get());

			// 頂点バッファの入力
			ID3D11Buffer *vertexBuffers[] = { vertexBuffer.Get() };
			UINT strides[] = { sizeof(Vertex) };
			UINT offsets[] = { 0 };
			context->IASetVertexBuffers(0, 1, vertexBuffers, strides, offsets);

			// 頂点バッファを設定
			context->VSSetShader(vertexShader.Get(), nullptr, 0);

			// 頂点バッファのコンスタントバッファの設定
			ID3D11Buffer *cbuffers[] = { matrixesBuffer.Get() };
			context->VSSetConstantBuffers(0, 1, cbuffers);

			// ジオメトリシェーダ設定
			context->GSSetShader(geometryShader.Get(), nullptr, 0);

			// ジオメトリシェーダのコンスタントバッファを設定
			context->GSSetConstantBuffers(0, 1, cbuffers);

			// ジオメトリシェーダ
			ID3D11ShaderResourceView *gsStructherds[] = { gsVertexsView.Get() };
			context->GSSetShaderResources(1, 1, gsStructherds);

			// ピクセルシェーダ設定
			context->PSSetShader(pixelShader.Get(), nullptr, 0);

			// ピクセルシェーダにテクスチャ設定
			ID3D11ShaderResourceView *tex[] = { textureView.Get() };
			context->PSSetShaderResources(0, 1, tex);


			ID3D11SamplerState * sampler[] = { samplerState.Get() };
			context->PSSetSamplers(0, 1, sampler);

			// ラスタライザーの設定
			context->RSSetState(rasterizerState.Get());

			// ブレンドの設定
			float blendFactor[] = { 1, 1, 1, 1 };
			context->OMSetBlendState(blendState.Get(), blendFactor, 0xFFFFFFFF);

			// 深度ステンシルを設定
			context->OMSetDepthStencilState(depthStencilState.Get(), 0);


			// 描画
			context->Draw(index, 0);


			// 他の描画に影響を与える設定を消しておく
			// ブレンドの設定
			context->OMSetBlendState(nullptr, blendFactor, 0xFFFFFFFF);

			// 深度ステンシルを設定
			context->OMSetDepthStencilState(nullptr, 0);

			// 
			ID3D11ShaderResourceView *nulltex[] = { nullptr };
			context->PSSetShaderResources(0, 1, nulltex);

			// 
			ID3D11SamplerState * nullsampler[] = { nullptr };
			context->PSSetSamplers(0, 1, nullsampler);


			context->VSSetShader(nullptr, nullptr, 0);
			context->GSSetShader(nullptr, nullptr, 0);
			context->PSSetShader(nullptr, nullptr, 0);
		}

		void SetMatrix(
			Matrix world,
			Matrix view,
			Matrix proj,
			bool isBillboardUpdate)
		{
			matrixes.world = world.Transpose();
			matrixes.view = view.Transpose();
			matrixes.proj = proj.Transpose();

			if (isBillboardAutoUpdate)
			{
				matrixes.billboard = view.Invert();
				matrixes.billboard._41 = 0.0f;
				matrixes.billboard._42 = 0.0f;
				matrixes.billboard._43 = 0.0f;
				matrixes.billboard = matrixes.billboard.Transpose();
			}
		}

		void SetBillboard(CXMMATRIX billboadTransform)
		{
			matrixes.billboard = XMMatrixTranspose(billboadTransform);
			isBillboardAutoUpdate = false;
		}

		void SetAutoBillboard()
		{
			isBillboardAutoUpdate = true;
		}

	};  // end of class PointSprite


} // end of namespace



class ParticleManager::Impl
{
public:

	unique_ptr<PointSprite> pointSprite;

	Impl() : gx(0), gy(0), gz(0), tableNum(100000 + 1), infoTable(tableNum)
	{
		Clean();
	}

	Impl(ID3D11DeviceContext *context, uint32_t num) : pointSprite(new PointSprite(context, num)), gx(0), gy(0), gz(0), tableNum(num + 1), infoTable(tableNum)
	{
		Initialize();
	};

	~Impl() {};


	//----------------------------------------
	// パーティクル関連


	// エントリー情報
	struct Info
	{
		float x, y, z;    // 現在の座標
		float vx, vy, vz; // 速度
		float ax, ay, az; // 加速度


		float angle;   // 現在の角度
		float s_angle; // 開始角度
		float e_angle; // 終了角度


		float scale;   // 現在のスケール
		float s_scale; // 開始スケール
		float e_scale; // 終了スケール


		float r, g, b, a;         // 現在の色
		float s_r, s_g, s_b, s_a; // 開始の色
		float e_r, e_g, e_b, e_a; // 終了の色


		int   frame;     // 現在のフレームカウント
		int   num_frame; // 終了のフレームカウント


		Info* next; // 次のデータポインタ
		Info* prev; // 前のデータポインタ
	};


	const int tableNum;// エントリーの限界数
	vector<Info> infoTable;


	Info freeList;// フリーなエントリー情報
	Info activeList;// アクティブなエントリー情報


	float gx, gy, gz;

	// アクティブリストにエントリー情報を追加する
	Info* AddInfo()
	{
		// フリーのエントリー情報を取得
		Info *info = freeList.next;

		// フリーのエントリー情報がない場合NULLを返す
		if (info->next == &freeList) return nullptr;

		// 取得したエントリー情報を、フリーリストから切り離す
		freeList.next = freeList.next->next;
		freeList.next->prev = &freeList;

		// エントリー情報をアクティブリストに接続
		info->prev = &activeList;
		info->next = activeList.next;
		info->prev->next = info;
		info->next->prev = info;

		// 取得したエントリー情報のポインタを返す
		return info;

	}

	// 指定のエントリー情報をアクティブリストから、フリーリストへ移す
	Info* Free(Info *info)
	{
		// エントリー情報の次のエントリー情報を取得する
		Info *temp = info->next;

		// アクティブリストから、エントリー情報を切り離す
		info->next->prev = info->prev;
		info->prev->next = info->next;

		// エントリー情報をフリーリストに接続する
		info->prev = &freeList;
		info->next = freeList.next;
		info->next->prev = info;
		info->prev->next = info;

		// 切り離した、次のエントリー情報を返す
		return temp;
	}

	// 初期化
	void Clean()
	{
		// アクティブリストの初期設定（空にする）
		activeList.next = &activeList;
		activeList.prev = &activeList;

		// フリーリストの初期設定（空にする）
		freeList.next = &freeList;
		freeList.next = &freeList;

		// エントリー情報の配列をすべてフリーリストに接続する
		//  テーブルの数だけ繰り返す
		for (int i = 0; i < tableNum; i++)
		{
			// フリーリストに接続
			Info* p = &infoTable[i];
			p->prev = &freeList;
			p->next = freeList.next;
			p->prev->next = p;
			p->next->prev = p;
		}
	}

	void Entry(
		int frame,// フレーム数
		float x, float y, float z,// 座標
		float vx, float vy, float vz,// 速度
		float ax, float ay, float az,// 加速度
		float startAngle, float endAngle,// 拡縮・開始倍率と終了倍率
		float startScale, float endScale,// 回転・開始角度と終了角度
		float startRed, float startGreen, float startBlue, float startAlpha,// 開始カラー
		float endRed, float endGreen, float endBlue, float endAlpha// 終了カラー
		)
	{
		// エントリー情報を取得する
		Impl::Info* info = AddInfo();

		// エントリー情報を取得できなかった場合、エントリーしない
		if (info == nullptr)  return;

		// 初期座標設定
		info->x = x;
		info->y = y;
		info->z = z;

		// 初速度設定
		info->vx = vx;
		info->vy = vy;
		info->vz = vz;

		// 加速度設定
		info->ax = ax;
		info->ay = ay;
		info->az = az;


		// 開始スケールと、終了スケールを設定
		info->scale = startScale;
		info->s_scale = startScale;
		info->e_scale = endScale;

		// 開始アングルと、終了アングルの設定
		info->angle = startAngle;
		info->s_angle = startAngle;
		info->e_angle = endAngle;

		// 開始カラーの設定（現在のカラー）
		info->a = startAlpha;
		info->r = startRed;
		info->g = startGreen;
		info->b = startBlue;

		// 開始カラーの設定
		info->s_a = startAlpha;
		info->s_r = startRed;
		info->s_g = startGreen;
		info->s_b = startBlue;

		// 終了からの設定
		info->e_a = endAlpha;
		info->e_r = endRed;
		info->e_g = endGreen;
		info->e_b = endBlue;

		// 開始フレームの設定
		info->frame = 0;
		// 終了フレームの設定
		info->num_frame = frame;

		// ポイントスプライトを登録
		pointSprite->Add(info->x, info->y, info->z,
			info->angle, info->scale,
			info->r, info->g, info->b, info->a);
	}

	// 線形補間
	float Lerp(float t, float s, float e)
	{
		return t * (e - s) + s;
	}

	// エントリー情報のアップデート
	bool Update(Info *info)
	{
		// フレームカウントを進める(時間経過)
		info->frame++;

		// 終了フレームカウントに達していたら、終了する
		if (info->frame > info->num_frame) return false;

		// 時間経過を係数(0.0f～1.0f)に変換
		float f = static_cast<float>(info->frame) / info->num_frame;

		// 速度に加速度を加える
		info->vx += info->ax;
		info->vy += info->ay;
		info->vz += info->az;

		// 速度に重力加速度を加える
		info->vx += gx;
		info->vy += gy;
		info->vz += gz;

		// 現在の座標に速度を加える
		info->x += info->vx;
		info->y += info->vy;
		info->z += info->vz;

		// 経過時間の角度を求める（線形補間）
		info->angle = Lerp(f, info->s_angle, info->e_angle);
		// 経過時間のスケールを求める（線形補間）
		info->scale = Lerp(f, info->s_scale, info->e_scale);

		// 経過時間の色を求める（線形補間）
		info->r = Lerp(f, info->s_r, info->e_r);
		info->g = Lerp(f, info->s_g, info->e_g);
		info->b = Lerp(f, info->s_b, info->e_b);
		info->a = Lerp(f, info->s_a, info->e_a);

		// ポイントスプライトを登録
		pointSprite->Add(info->x, info->y, info->z,
			info->angle, info->scale,
			info->r, info->g, info->b, info->a);

		// 継続する
		return true;
	}

	// すべてのアクティブなエントリー情報を更新する
	void Update()
	{
		// ポイントスプライトをクリア
		pointSprite->Reset();

		// アクティブなエントリー情報の先頭ポインタを取得する
		auto *i = activeList.next;

		// リストの終端まで繰り返す
		while (i != &activeList)
		{
			// 更新の実行
			if (Update(i))
			{
				// 処理続行(true)の場合
				// 次のエントリー情報へ
				i = i->next;
			}
			else
			{
				// 処理終了(false)の場合
				// アクティブリストから切り離す
				i = Free(i);
			}
		}
	}


	// 初期化
	void Initialize()
	{
		// パーティクルリストのクリア
		Clean();

		// ポイントスプライトの初期化
		pointSprite->Initialize();
	}

	void Create(ID3D11DeviceContext* context, uint32_t num)
	{
		Clean();

		pointSprite = make_unique<PointSprite>(context, num);
		pointSprite->Initialize();

#if 0
		ComPtr<ID3D11Device> device;
		context->GetDevice(&device);

		CS cs;
		cs.Create(device.Get(), context);
#endif
	}

	// 描画
	void Draw(CXMMATRIX view, CXMMATRIX proj)
	{
		pointSprite->SetMatrix(Matrix::Identity, view, proj, true);
		pointSprite->Applay();

		pointSprite->Draw();
	}

	void SetGravity(float gx, float gy, float gz)
	{
		this->gx = gx;
		this->gy = gy;
		this->gz = gz;
	}


	// エントリーのリセット
	void Reset()
	{
	}


#if 0
	// コンピュートシェーダテスト
	class CS
	{
	public:
		ComPtr<ID3D11ComputeShader> computeShader;
		ComPtr<ID3D11Buffer> particleBuffer;
		ComPtr<ID3D11ShaderResourceView> particleView;
		ComPtr<ID3D11UnorderedAccessView> particleUAV;
		ComPtr<ID3D11Buffer> pointSpriteBuffer;
		ComPtr<ID3D11UnorderedAccessView> pointSpriteUAV;

		void Create(ID3D11Device* device, ID3D11DeviceContext* context)
		{
			static const int num = 100000;
			{
				using Type = Info;
				D3D11_BUFFER_DESC desc = {};
				desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
				desc.ByteWidth = sizeof(Type) * num;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
				desc.StructureByteStride = sizeof(Type);
				desc.Usage = D3D11_USAGE_DEFAULT;
				device->CreateBuffer(&desc, nullptr, &particleBuffer);

				D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
				srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
				srvd.BufferEx.FirstElement = 0;
				srvd.BufferEx.NumElements = num;
				srvd.Format = DXGI_FORMAT_UNKNOWN;
				device->CreateShaderResourceView(particleBuffer.Get(), &srvd, &particleView);


				//CD3D11_UNORDERED_ACCESS_VIEW_DESC cuavd();
				D3D11_UNORDERED_ACCESS_VIEW_DESC uavd;
				uavd.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				uavd.Buffer.FirstElement = 0;
				uavd.Buffer.NumElements = num;
				uavd.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
				uavd.Format = DXGI_FORMAT_UNKNOWN;
				device->CreateUnorderedAccessView(particleBuffer.Get(), &uavd, &particleUAV);
			}

			HRESULT hr;
			{
				using Type = PointSprite::Vertex;
				D3D11_BUFFER_DESC desc = {};
				desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;
				desc.ByteWidth = sizeof(Type) * num;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
				//desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
				//desc.StructureByteStride = sizeof(Type)*4;
				desc.Usage = D3D11_USAGE_DEFAULT;
				hr = device->CreateBuffer(&desc, nullptr, &pointSpriteBuffer);

				D3D11_UNORDERED_ACCESS_VIEW_DESC uavd = {};
				uavd.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
				uavd.Buffer.FirstElement = 0;
				//				uavd.Buffer.NumElements = num;
				uavd.Buffer.NumElements = 1;
				//				uavd.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
				uavd.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
				//				uavd.Format = DXGI_FORMAT_UNKNOWN;
				uavd.Format = DXGI_FORMAT_R32_TYPELESS;
				hr = device->CreateUnorderedAccessView(pointSpriteBuffer.Get(), &uavd, &pointSpriteUAV);
			}

			{
				D3D11_MAPPED_SUBRESOURCE map = {};
				hr = context->Map(particleBuffer.Get(),
					0,
					D3D11_MAP_WRITE_DISCARD,
					0,
					&map);

				context->Unmap(particleBuffer.Get(), 0);

				//char aaa[1000];

				//D3D11_BOX box;
				//context->UpdateSubresource(
				//	particleBuffer.Get(),
				//	0,
				//	nullptr,
				//	aaa,
				//	4,
				//	4);

			}


		}

	};
#endif

};


ParticleManager::ParticleManager() : pImpl(make_unique<Impl>())
{
}

ParticleManager::ParticleManager(ID3D11DeviceContext * context) : pImpl(make_unique<Impl>(context, 100000))
{
}

ParticleManager::ParticleManager(ID3D11DeviceContext *context, uint32_t num) : pImpl(make_unique<Impl>(context, num))
{
}


ParticleManager::~ParticleManager()
{
}



void ParticleManager::Initialize()
{
	pImpl->Initialize();
}

void ParticleManager::Create(ID3D11DeviceContext * context)
{
	pImpl->Create(context, 100000);
}

void ParticleManager::Create(ID3D11DeviceContext * context, uint32_t num)
{
	pImpl->Create(context, num);
}


void ParticleManager::Update()
{
	pImpl->Update();
}


void ParticleManager::Entry(
	int frame,
	float x, float y, float z,
	float vx, float vy, float vz,
	float ax, float ay, float az,
	float startAngle, float endAngle,
	float startScale, float endScale,
	float startRed, float startGreen, float startBlue, float startAlpha,
	float endRed, float endGreen, float endBlue, float endAlpha
	)
{
	pImpl->Entry(frame, x, y, z, vx, vy, vz, ax, ay, az, startAngle, endAngle, startScale, endScale, startRed, startGreen, startBlue, startAlpha, endRed, endGreen, endBlue, endAlpha);
}


void ParticleManager::Draw(CXMMATRIX view, CXMMATRIX proj)
{
	pImpl->Draw(view, proj);
}


void ParticleManager::LoadTexture(const wchar_t * filename)
{
	pImpl->pointSprite->LoadTexture(filename);
}

void ParticleManager::SetTexture(ID3D11ShaderResourceView * texture)
{
	pImpl->pointSprite->SetTexture(texture);
}


void ParticleManager::SetGravity(float gx, float gy, float gz)
{
	pImpl->SetGravity(gx, gy, gz);
}

void ParticleManager::SetBillboard(DirectX::CXMMATRIX billboadTransform)
{
	pImpl->pointSprite->SetBillboard(billboadTransform);
}

void ParticleManager::SetAutoBillboard()
{
	pImpl->pointSprite->SetAutoBillboard();
}

void ParticleManager::Reset()
{
	pImpl->Clean();
}


std::wstring ParticleManager::ToString()
{
	wchar_t buf[1024];

	swprintf_s(buf, L"Particle Count(%8d)", pImpl->pointSprite->index);

	return buf;
}

