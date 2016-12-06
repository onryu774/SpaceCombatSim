#include "Obj3D.h"
#include "VertexTypes.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
ID3D11Device* Obj3D::s_pDevice;
ID3D11DeviceContext* Obj3D::s_pDeviceContext;
DirectX::CommonStates* Obj3D::s_pStates;
DirectX::EffectFactory* Obj3D::s_pEffectFactory;
Camera* Obj3D::s_pCamera;
std::map<std::wstring, std::unique_ptr<DirectX::Model>> Obj3D::s_modelarray;
Obj3D::Obj3D() : m_pParentMatrix(nullptr)
, m_pModel(nullptr),m_UseQuaternion(false)
{
	m_Scale = Vector3(1, 1, 1);
}


Obj3D::~Obj3D()
{
}

void Obj3D::LoadModelFile(const wchar_t * filename)
{
	assert(s_pEffectFactory);
	if (s_modelarray.count(filename) == 0)
	{
		s_modelarray[filename] = Model::CreateFromCMO(s_pDevice, filename, *s_pEffectFactory);
	}
	m_pModel = s_modelarray[filename].get();
}

void Obj3D::DisableLighting()
{
	if (m_pModel)
	{
		ModelMesh::Collection::const_iterator it_mesh = m_pModel->meshes.begin();
		for (; it_mesh != m_pModel->meshes.end(); it_mesh++)
		{
			ModelMesh*modelmesh=it_mesh->get();
			assert(modelmesh);
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart*meshpart = it_meshpart->get();
				assert(meshpart);
				BasicEffect*eff= static_cast<BasicEffect*>(meshpart->effect.get());
				{
					XMVECTOR emissive;
					emissive.m128_f32[0] = 1.0f;
					emissive.m128_f32[1] = 1.0f;
					emissive.m128_f32[2] = 1.0f;
					emissive.m128_f32[3] = 1.0f;
					eff->SetEmissiveColor(emissive);
					// エフェクトの含む全ての平行光源分回す
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}

void Obj3D::Calc()
{
	Matrix scalem;
	Matrix rotm;
	Matrix transm;
	scalem = Matrix::CreateScale(m_Scale);
	
	if (m_UseQuaternion)
	{
		rotm = Matrix::CreateFromQuaternion(m_quaternipn);
	}
	else
	{
		rotm = Matrix::CreateFromYawPitchRoll(m_Rot.y, m_Rot.x, m_Rot.z);
	}
	transm = Matrix::CreateTranslation(m_Trans);
	// ワールド行列をSRTの順に合成
	m_LocalWorld = Matrix::Identity;
	m_LocalWorld *= scalem;
	m_LocalWorld *= rotm;
	m_LocalWorld *= transm;
	// 親行列があれば
	if (m_pParentMatrix)
	{
		// 親行列を掛ける
		m_LocalWorld = m_LocalWorld * (*m_pParentMatrix);
	}
}

void Obj3D::Draw()
{
	if (m_pModel)
	{
		assert(s_pCamera);
		const Matrix& view = s_pCamera->GetView();
		const Matrix& projection = s_pCamera->GetProj();
		assert(s_pDeviceContext);
		assert(s_pStates);
		m_pModel->Draw(s_pDeviceContext, *s_pStates, m_LocalWorld, view, projection);
	}
}
