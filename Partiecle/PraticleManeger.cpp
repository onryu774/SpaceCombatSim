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
	//	char* VertexShader;   // �_�~�[
	//	char* GeometryShader; // �_�~�[
	//	char* PixelShader;    // �_�~�[
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

// �|�C���g�X�v���C�g�̒�`
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

		ComPtr<ID3D11DeviceContext> context; // �R���e�L�X�g
		ComPtr<ID3D11Device> device; // �f�o�C�X

									 // Vertex buffer
		ComPtr<ID3D11InputLayout> inputLayout; // �C���v�b�g���C�A�E�g
		ComPtr<ID3D11Buffer> vertexBuffer; // ���_�o�b�t�@
		ComPtr<ID3D11Buffer> indexBuffer; // �C���f�b�N�X�o�t�@

										  // shader
		ComPtr<ID3D11VertexShader> vertexShader;     // ���_�V�F�[�_
		ComPtr<ID3D11GeometryShader> geometryShader; // �W�I���g���V�F�[�_
		ComPtr<ID3D11PixelShader> pixelShader;       // �s�N�Z���V�F�[�_

													 // constant buffer
		ComPtr<ID3D11Buffer> matrixesBuffer; // �s��֘A�̃R���X�^���g�o�b�t�@

											 // 
		ComPtr<ID3D11RasterizerState> rasterizerState;      // ���X�^���C�U�[�X�e�[�g
		ComPtr<ID3D11BlendState> blendState;                // �u�����h�X�e�[�g
		ComPtr<ID3D11DepthStencilState> depthStencilState;  // �[�x�X�e���V���X�e�[�g

															// �e�N�X�`��
		ComPtr<ID3D11ShaderResourceView> textureView;  // �e�N�X�`��
		ComPtr<ID3D11SamplerState>       samplerState; // �T���v���[


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

		ComPtr<ID3D11Buffer> gsVertexInfoBuffer; // ���
		ComPtr<ID3D11Buffer> gsVertexsBuffer;     // �\����
		ComPtr<ID3D11ShaderResourceView> gsVertexsView;


		bool isBillboardAutoUpdate;   // �r���{�[�h�̎����X�V

									  // �R���X�^���g�o�b�t�@�ɑ���
		struct MatrixesBufferType
		{
			Matrix world;
			Matrix view;
			Matrix proj;
			Matrix billboard;
		};
		MatrixesBufferType matrixes;

		// �v���~�e�B�u�̍ő吔
		//static const int num = 100000;

		// ���_�f�[�^
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

			// �e�N�X�`���̓ǂݍ���
			// �����ł̃G���[�́A�e�N�X�`���t�@�C���̊i�[�ꏊ��g���q���`�F�b�N���Ă��������B
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

			// ���_���C�A�E�g�̍쐬
			D3D11_INPUT_ELEMENT_DESC elements[] =
			{
				{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 1, DXGI_FORMAT_R32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			DX::ThrowIfFailed(device->CreateInputLayout(elements, ARRAYSIZE(elements), VertexShader, sizeof(VertexShader), &inputLayout));

			// �o�[�e�b�N�X�V�F�[�_�̍쐬
			DX::ThrowIfFailed(device->CreateVertexShader(VertexShader, sizeof(VertexShader), nullptr, &vertexShader));

			// �W�I���g���V�F�[�_�̍쐬
			DX::ThrowIfFailed(device->CreateGeometryShader(GeometryShader, sizeof(GeometryShader), nullptr, &geometryShader));

			// �s�N�Z���V�F�[�_�̍쐬
			DX::ThrowIfFailed(device->CreatePixelShader(PixelShader, sizeof(PixelShader), nullptr, &pixelShader));


			// �R���X�^���g�o�b�t�@���쐬
			// �ݒ荀�ځF�R���X�^���g�o�b�t�@�A�T�C�Y��MatrixesBufferType
			D3D11_BUFFER_DESC matrixesBufferDesc = {};
			matrixesBufferDesc.ByteWidth = sizeof(MatrixesBufferType);
			matrixesBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			matrixesBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			matrixesBufferDesc.CPUAccessFlags = 0;
			matrixesBufferDesc.MiscFlags = 0;
			matrixesBufferDesc.StructureByteStride = sizeof(DWORD);
			DX::ThrowIfFailed(device->CreateBuffer(&matrixesBufferDesc, nullptr, &matrixesBuffer));

			// �W�I���g���V�F�[�_�̌`��f�[�^�����߂�o�b�t�@
			D3D11_BUFFER_DESC gsVertexBufferDesc = {};
			gsVertexBufferDesc.ByteWidth = sizeof(GSVertexType) * 4; // �f�[�^�T�C�Y
			gsVertexBufferDesc.BindFlags =
				D3D11_BIND_UNORDERED_ACCESS |  // �����_���A�N�Z�X�Ƀo�C���h
				D3D11_BIND_SHADER_RESOURCE;    // �V�F�[�_���\�[�X�Ƀo�C���h
			gsVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			gsVertexBufferDesc.CPUAccessFlags = 0;
			gsVertexBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // �\���̃o�b�t�@�Ƃ��č쐬
			gsVertexBufferDesc.StructureByteStride = sizeof(GSVertexType);

			// �`��f�[�^
			gsVertexs[0].pos = Vector4(-0.5f, 0.5f, 0.0f, 0.0f);
			gsVertexs[0].uv = Vector2(0.0f, 0.0f);
			gsVertexs[1].pos = Vector4(0.5f, 0.5f, 0.0f, 0.0f);
			gsVertexs[1].uv = Vector2(1.0f, 0.0f);
			gsVertexs[2].pos = Vector4(-0.5f, -0.5f, 0.0f, 0.0f);
			gsVertexs[2].uv = Vector2(0.0f, 1.0f);
			gsVertexs[3].pos = Vector4(0.5f, -0.5f, 0.0f, 0.0f);
			gsVertexs[3].uv = Vector2(1.0f, 1.0f);
			D3D11_SUBRESOURCE_DATA srd = {};
			srd.pSysMem = gsVertexs.data();     // �f�[�^�|�C���^
			srd.SysMemPitch = gsVertexs.size(); // �o�C�g��
			srd.SysMemSlicePitch = 0;
			// �o�b�t�@�̍쐬
			DX::ThrowIfFailed(device->CreateBuffer(&gsVertexBufferDesc, &srd, &gsVertexsBuffer));


			// �o�b�t�@����r���[���쐬����
			D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
			srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX; // �g���o�b�t�@
			srvd.BufferEx.FirstElement = 0;
			srvd.BufferEx.NumElements = 4; // �v�f��
			srvd.Format = DXGI_FORMAT_UNKNOWN;
			DX::ThrowIfFailed(device->CreateShaderResourceView(gsVertexsBuffer.Get(), &srvd, &gsVertexsView));

			// ���X�^���C�U�̍쐬
			//  �ݒ荀�ځF�ʕ`��A�J�����O�Ȃ��A�[�x�N���b�v����A�}���`�A���v�����O����
			D3D11_RASTERIZER_DESC rasterizerDesc = {};
			rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
			rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
			rasterizerDesc.DepthClipEnable = true;
			rasterizerDesc.MultisampleEnable = true;
			DX::ThrowIfFailed(device->CreateRasterizerState(&rasterizerDesc, &rasterizerState));


			// �[�x�X�e���V���쐬
			// �ݒ荀�ځF�[�x�o�b�t�@����A�[�x�������݂Ȃ��AZ�l�����������`��A�X�e���V�����g��Ȃ�
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

			// �u�����h�쐬(���Z����)
			//  �ݒ荀�ځF�A���t�@�u�����h�g���A�����_�[�^�[�Q�b�g�ɂ��ׂĂ̏����������ށA���Z����
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


			// ���_�o�b�t�@���쐬
			// �ݒ荀�ځF���_�̃o�b�t�@�A�T�C�Y�͍ő吔�Afloat�P�ʂ̃A�N�Z�X
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
			samplerDesc.MipLODBias = 0;                            // �v�Z���ꂽ�~�b�v�}�b�v ���x������̃o�C�A�X
			samplerDesc.MaxAnisotropy = 16;                        // �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // ��r�I�v�V�����B
			samplerDesc.BorderColor[0] = 0.0f; // ���E�F
			samplerDesc.BorderColor[1] = 0.0f; // ���E�F
			samplerDesc.BorderColor[2] = 0.0f; // ���E�F
			samplerDesc.BorderColor[3] = 0.0f; // ���E�F
			samplerDesc.MinLOD = 0;                                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̉����l
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̏���l
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
			// �R���X�^���g�o�b�t�@�̃A�b�v�f�[�g
			context->UpdateSubresource(matrixesBuffer.Get(), 0, nullptr, &matrixes, 0, 0);

			// �`��̐ݒ�
			context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

			// �C���v�b�g���C�A�E�g��ݒ�
			context->IASetInputLayout(inputLayout.Get());

			// ���_�o�b�t�@�̓���
			ID3D11Buffer *vertexBuffers[] = { vertexBuffer.Get() };
			UINT strides[] = { sizeof(Vertex) };
			UINT offsets[] = { 0 };
			context->IASetVertexBuffers(0, 1, vertexBuffers, strides, offsets);

			// ���_�o�b�t�@��ݒ�
			context->VSSetShader(vertexShader.Get(), nullptr, 0);

			// ���_�o�b�t�@�̃R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer *cbuffers[] = { matrixesBuffer.Get() };
			context->VSSetConstantBuffers(0, 1, cbuffers);

			// �W�I���g���V�F�[�_�ݒ�
			context->GSSetShader(geometryShader.Get(), nullptr, 0);

			// �W�I���g���V�F�[�_�̃R���X�^���g�o�b�t�@��ݒ�
			context->GSSetConstantBuffers(0, 1, cbuffers);

			// �W�I���g���V�F�[�_
			ID3D11ShaderResourceView *gsStructherds[] = { gsVertexsView.Get() };
			context->GSSetShaderResources(1, 1, gsStructherds);

			// �s�N�Z���V�F�[�_�ݒ�
			context->PSSetShader(pixelShader.Get(), nullptr, 0);

			// �s�N�Z���V�F�[�_�Ƀe�N�X�`���ݒ�
			ID3D11ShaderResourceView *tex[] = { textureView.Get() };
			context->PSSetShaderResources(0, 1, tex);


			ID3D11SamplerState * sampler[] = { samplerState.Get() };
			context->PSSetSamplers(0, 1, sampler);

			// ���X�^���C�U�[�̐ݒ�
			context->RSSetState(rasterizerState.Get());

			// �u�����h�̐ݒ�
			float blendFactor[] = { 1, 1, 1, 1 };
			context->OMSetBlendState(blendState.Get(), blendFactor, 0xFFFFFFFF);

			// �[�x�X�e���V����ݒ�
			context->OMSetDepthStencilState(depthStencilState.Get(), 0);


			// �`��
			context->Draw(index, 0);


			// ���̕`��ɉe����^����ݒ�������Ă���
			// �u�����h�̐ݒ�
			context->OMSetBlendState(nullptr, blendFactor, 0xFFFFFFFF);

			// �[�x�X�e���V����ݒ�
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
	// �p�[�e�B�N���֘A


	// �G���g���[���
	struct Info
	{
		float x, y, z;    // ���݂̍��W
		float vx, vy, vz; // ���x
		float ax, ay, az; // �����x


		float angle;   // ���݂̊p�x
		float s_angle; // �J�n�p�x
		float e_angle; // �I���p�x


		float scale;   // ���݂̃X�P�[��
		float s_scale; // �J�n�X�P�[��
		float e_scale; // �I���X�P�[��


		float r, g, b, a;         // ���݂̐F
		float s_r, s_g, s_b, s_a; // �J�n�̐F
		float e_r, e_g, e_b, e_a; // �I���̐F


		int   frame;     // ���݂̃t���[���J�E���g
		int   num_frame; // �I���̃t���[���J�E���g


		Info* next; // ���̃f�[�^�|�C���^
		Info* prev; // �O�̃f�[�^�|�C���^
	};


	const int tableNum;// �G���g���[�̌��E��
	vector<Info> infoTable;


	Info freeList;// �t���[�ȃG���g���[���
	Info activeList;// �A�N�e�B�u�ȃG���g���[���


	float gx, gy, gz;

	// �A�N�e�B�u���X�g�ɃG���g���[����ǉ�����
	Info* AddInfo()
	{
		// �t���[�̃G���g���[�����擾
		Info *info = freeList.next;

		// �t���[�̃G���g���[��񂪂Ȃ��ꍇNULL��Ԃ�
		if (info->next == &freeList) return nullptr;

		// �擾�����G���g���[�����A�t���[���X�g����؂藣��
		freeList.next = freeList.next->next;
		freeList.next->prev = &freeList;

		// �G���g���[�����A�N�e�B�u���X�g�ɐڑ�
		info->prev = &activeList;
		info->next = activeList.next;
		info->prev->next = info;
		info->next->prev = info;

		// �擾�����G���g���[���̃|�C���^��Ԃ�
		return info;

	}

	// �w��̃G���g���[�����A�N�e�B�u���X�g����A�t���[���X�g�ֈڂ�
	Info* Free(Info *info)
	{
		// �G���g���[���̎��̃G���g���[�����擾����
		Info *temp = info->next;

		// �A�N�e�B�u���X�g����A�G���g���[����؂藣��
		info->next->prev = info->prev;
		info->prev->next = info->next;

		// �G���g���[�����t���[���X�g�ɐڑ�����
		info->prev = &freeList;
		info->next = freeList.next;
		info->next->prev = info;
		info->prev->next = info;

		// �؂藣�����A���̃G���g���[����Ԃ�
		return temp;
	}

	// ������
	void Clean()
	{
		// �A�N�e�B�u���X�g�̏����ݒ�i��ɂ���j
		activeList.next = &activeList;
		activeList.prev = &activeList;

		// �t���[���X�g�̏����ݒ�i��ɂ���j
		freeList.next = &freeList;
		freeList.next = &freeList;

		// �G���g���[���̔z������ׂăt���[���X�g�ɐڑ�����
		//  �e�[�u���̐������J��Ԃ�
		for (int i = 0; i < tableNum; i++)
		{
			// �t���[���X�g�ɐڑ�
			Info* p = &infoTable[i];
			p->prev = &freeList;
			p->next = freeList.next;
			p->prev->next = p;
			p->next->prev = p;
		}
	}

	void Entry(
		int frame,// �t���[����
		float x, float y, float z,// ���W
		float vx, float vy, float vz,// ���x
		float ax, float ay, float az,// �����x
		float startAngle, float endAngle,// �g�k�E�J�n�{���ƏI���{��
		float startScale, float endScale,// ��]�E�J�n�p�x�ƏI���p�x
		float startRed, float startGreen, float startBlue, float startAlpha,// �J�n�J���[
		float endRed, float endGreen, float endBlue, float endAlpha// �I���J���[
		)
	{
		// �G���g���[�����擾����
		Impl::Info* info = AddInfo();

		// �G���g���[�����擾�ł��Ȃ������ꍇ�A�G���g���[���Ȃ�
		if (info == nullptr)  return;

		// �������W�ݒ�
		info->x = x;
		info->y = y;
		info->z = z;

		// �����x�ݒ�
		info->vx = vx;
		info->vy = vy;
		info->vz = vz;

		// �����x�ݒ�
		info->ax = ax;
		info->ay = ay;
		info->az = az;


		// �J�n�X�P�[���ƁA�I���X�P�[����ݒ�
		info->scale = startScale;
		info->s_scale = startScale;
		info->e_scale = endScale;

		// �J�n�A���O���ƁA�I���A���O���̐ݒ�
		info->angle = startAngle;
		info->s_angle = startAngle;
		info->e_angle = endAngle;

		// �J�n�J���[�̐ݒ�i���݂̃J���[�j
		info->a = startAlpha;
		info->r = startRed;
		info->g = startGreen;
		info->b = startBlue;

		// �J�n�J���[�̐ݒ�
		info->s_a = startAlpha;
		info->s_r = startRed;
		info->s_g = startGreen;
		info->s_b = startBlue;

		// �I������̐ݒ�
		info->e_a = endAlpha;
		info->e_r = endRed;
		info->e_g = endGreen;
		info->e_b = endBlue;

		// �J�n�t���[���̐ݒ�
		info->frame = 0;
		// �I���t���[���̐ݒ�
		info->num_frame = frame;

		// �|�C���g�X�v���C�g��o�^
		pointSprite->Add(info->x, info->y, info->z,
			info->angle, info->scale,
			info->r, info->g, info->b, info->a);
	}

	// ���`���
	float Lerp(float t, float s, float e)
	{
		return t * (e - s) + s;
	}

	// �G���g���[���̃A�b�v�f�[�g
	bool Update(Info *info)
	{
		// �t���[���J�E���g��i�߂�(���Ԍo��)
		info->frame++;

		// �I���t���[���J�E���g�ɒB���Ă�����A�I������
		if (info->frame > info->num_frame) return false;

		// ���Ԍo�߂��W��(0.0f�`1.0f)�ɕϊ�
		float f = static_cast<float>(info->frame) / info->num_frame;

		// ���x�ɉ����x��������
		info->vx += info->ax;
		info->vy += info->ay;
		info->vz += info->az;

		// ���x�ɏd�͉����x��������
		info->vx += gx;
		info->vy += gy;
		info->vz += gz;

		// ���݂̍��W�ɑ��x��������
		info->x += info->vx;
		info->y += info->vy;
		info->z += info->vz;

		// �o�ߎ��Ԃ̊p�x�����߂�i���`��ԁj
		info->angle = Lerp(f, info->s_angle, info->e_angle);
		// �o�ߎ��Ԃ̃X�P�[�������߂�i���`��ԁj
		info->scale = Lerp(f, info->s_scale, info->e_scale);

		// �o�ߎ��Ԃ̐F�����߂�i���`��ԁj
		info->r = Lerp(f, info->s_r, info->e_r);
		info->g = Lerp(f, info->s_g, info->e_g);
		info->b = Lerp(f, info->s_b, info->e_b);
		info->a = Lerp(f, info->s_a, info->e_a);

		// �|�C���g�X�v���C�g��o�^
		pointSprite->Add(info->x, info->y, info->z,
			info->angle, info->scale,
			info->r, info->g, info->b, info->a);

		// �p������
		return true;
	}

	// ���ׂẴA�N�e�B�u�ȃG���g���[�����X�V����
	void Update()
	{
		// �|�C���g�X�v���C�g���N���A
		pointSprite->Reset();

		// �A�N�e�B�u�ȃG���g���[���̐擪�|�C���^���擾����
		auto *i = activeList.next;

		// ���X�g�̏I�[�܂ŌJ��Ԃ�
		while (i != &activeList)
		{
			// �X�V�̎��s
			if (Update(i))
			{
				// �������s(true)�̏ꍇ
				// ���̃G���g���[����
				i = i->next;
			}
			else
			{
				// �����I��(false)�̏ꍇ
				// �A�N�e�B�u���X�g����؂藣��
				i = Free(i);
			}
		}
	}


	// ������
	void Initialize()
	{
		// �p�[�e�B�N�����X�g�̃N���A
		Clean();

		// �|�C���g�X�v���C�g�̏�����
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

	// �`��
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


	// �G���g���[�̃��Z�b�g
	void Reset()
	{
	}


#if 0
	// �R���s���[�g�V�F�[�_�e�X�g
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

