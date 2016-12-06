#include "LandShape.h"
#include<fstream>;
#include <algorithm>
#include"CollisionNode.h"
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

std::unique_ptr<LandShapeCommon>LandShape::s_pCommon;
std::map<std::wstring, std::unique_ptr<LandShapeDate>>LandShape::s_dataaray;


LandShapeCommon::LandShapeCommon(LandShapeCommonDef def)
{
	m_pCamera = def.pCamera;
	m_pStates.reset(new CommonStates(def.pDevice));
	m_pEffectFactory.reset(new EffectFactory(def.pDevice));
	m_pPrimitiveBatch.reset(new PrimitiveBatch<VertexPositionNormal>(def.pDeviceContext));
	m_pEffect.reset(new BasicEffect(def.pDevice));
	m_pEffect->SetLightingEnabled(true);
	m_pEffect->SetAmbientLightColor(Vector3(0, 0.0f, 0));
	m_pEffect->SetDiffuseColor(Vector3(1.0f, 1.0f,1.0f));
	m_pEffect->SetLightEnabled(0, true);
	m_pEffect->SetLightDiffuseColor(0,Vector3(0.2f,1.0f,0.2f));
	m_pEffect->SetLightDirection(0, Vector3(1.0f, -0.5f, 2.0f));
	m_pEffect->SetLightEnabled(1, true);
	m_pEffect->SetLightDiffuseColor(1, Vector3(0.5f, 0.2f, 0.3f));
	m_pEffect->SetLightDirection(1, Vector3(1.0f, -0.5f, 2.0f));

	void const*shaderByteCode;
	size_t byteCodeLength;
	m_pEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	def.pDevice->CreateInputLayout(VertexPositionNormal::InputElements, VertexPositionNormal::InputElementCount, shaderByteCode, byteCodeLength, &m_pInputLayout);
	m_pDeviceContext = def.pDeviceContext;

}

LandShapeCommon::~LandShapeCommon()
{
	if (m_pInputLayout)
	{
		m_pInputLayout->Release();
		m_pInputLayout = nullptr;
	}
}

void LandShape::InitializeCommon(LandShapeCommonDef def)
{
	if (s_pCommon)return;
	s_pCommon.reset(new LandShapeCommon(def));
	
}

LandShape::LandShape():m_Date(nullptr),m_Velocity( Vector3(0, 0, 0))
{
	
}

void LandShape::Initialize(const wchar_t * filename_mdl, const wchar_t * filename_cmo)
{
	if (filename_mdl)
	{
		std::map<std::wstring, std::unique_ptr<LandShapeDate>>::iterator it;
		it = s_dataaray.find(filename_mdl);
		if (s_dataaray.count(filename_mdl)==0)
		{
			s_dataaray[filename_mdl]=LandShapeDate::CreateFromMDL(filename_mdl);

		}
		m_Date = s_dataaray[filename_mdl].get();
	}
	if (filename_cmo)
	{
		m_obj.LoadModelFile(filename_cmo);
	}
}

void LandShape::Calc()
{
	Vector3 Trans = m_obj.GetTrans();
	Trans += m_Velocity;
	m_obj.SetTrans(Trans);
	m_obj.Calc();
	const Matrix&localworld = m_obj.GetLocalWorld();
	m_WorldLocal = localworld.Invert();
}

void LandShape::Draw()
{
	if (CollisionNode::GetDebugVisible() == false)
	{
		m_obj.Draw();
	}
	else if(m_Date)
	{
		const Matrix&view = s_pCommon->m_pCamera->GetView();
		const Matrix&projection = s_pCommon->m_pCamera->GetProj();

		s_pCommon->m_pEffect->SetWorld(m_obj.GetLocalWorld());
		s_pCommon->m_pEffect->SetView(view);
		s_pCommon->m_pEffect->SetProjection(projection);
		//エフェ
		s_pCommon->m_pEffect->Apply(s_pCommon->m_pDeviceContext);
		//深度
		s_pCommon->m_pDeviceContext->OMSetDepthStencilState(s_pCommon->m_pStates->DepthDefault(), 0);
		//ブレンド
		s_pCommon->m_pDeviceContext->OMSetBlendState(s_pCommon->m_pStates->NonPremultiplied(), nullptr,0xFFFFFFFF);
		//ラスタライズ
		s_pCommon->m_pDeviceContext->RSSetState(s_pCommon->m_pStates->CullClockwise());
		//サンプラー
		auto samplerState = s_pCommon->m_pStates->PointWrap();
		s_pCommon->m_pDeviceContext->VSSetSamplers(0, 1, &samplerState);
		 //入力
		s_pCommon->m_pDeviceContext->IASetInputLayout(s_pCommon->m_pInputLayout);
		
		s_pCommon->m_pPrimitiveBatch->Begin();

		const uint16_t*pIndex = &m_Date->m_Indices[0];
		int numIndex = m_Date->m_Indices.size();

		const VertexPositionNormal* pVertex = (VertexPositionNormal*)&m_Date->m_Vertex[0];
		int numVertex = m_Date->m_Vertex.size();

		s_pCommon->m_pPrimitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, pIndex, numIndex, pVertex, numVertex);

		s_pCommon->m_pPrimitiveBatch->End();


	}



}

bool LandShape::IntersectSphere(const Sphere & sphere, DirectX::SimpleMath::Vector3* reject)
{
	if (m_Date == nullptr)
	{
		return false;
	}

	int nTri = m_Date->m_Indices.size() / 3;
	
	bool hit=false;

	float distance=1.0e5;
	Vector3 l_inter;
	Vector3 l_normal;
	Vector3 l_down;

	float Scale=GetScale().x;

	Sphere localsphere = sphere;

	if (Scale <= 1.0e-10)
	{
		return false;
	}

	localsphere.center = Vector3::Transform(sphere.center, m_WorldLocal);

	localsphere.radius = sphere.radius / Scale;

	for (int i = 0; i < nTri; i++)
	{
		int index0 = m_Date->m_Indices[i * 3];
		int index1 = m_Date->m_Indices[i * 3 + 1];
		int index2 = m_Date->m_Indices[i * 3 + 2];
		Vector3 pos0 = m_Date->m_Vertex[index0].pos;
		Vector3 pos1 = m_Date->m_Vertex[index1].pos;
		Vector3 pos2 = m_Date->m_Vertex[index2].pos;

		Triangle tri;

		ComputeTriangle(pos0, pos1, pos2, &tri);

		float temp_distance;
		Vector3 temp_inter;
		if (CheckSphere2Triangle(localsphere, tri, &temp_inter))
		{
			hit = true;
			temp_distance = Vector3::Distance(localsphere.center, temp_inter);
			if (temp_distance < distance)
			{
				l_inter = temp_inter;
				l_normal = tri.Normal;
				distance = temp_distance;
			}
		}

	}


	if (hit)
	{
		distance *= Scale;

		const Matrix& localworld = m_obj.GetLocalWorld();

		if (reject != nullptr)
		{
			*reject = Vector3::TransformNormal(l_normal, localworld);
			reject->Normalize();
			*reject = (*reject)*(sphere.radius - distance);
		}

	}

	return hit;


}

bool LandShape::IntersectSegment(const Segment& segment, Vector3* inter)
{
	if (m_Date == nullptr) return false;

	// 三角形の数
	int nTri = m_Date->m_Indices.size() / 3;
	// ヒットフラグを初期化
	bool hit = false;
	// 大きい数字で初期化
	float distance = 1.0e5;
	// 角度判定用に地面とみなす角度の限界値<度>
	const float limit_angle = XMConvertToRadians(30.0f);
	Vector3 l_inter;

	// コピー
	Segment localSegment = segment;
	// 線分をワールド座標からモデル座標系に引き込む
	localSegment.start = Vector3::Transform(localSegment.start, m_WorldLocal);
	localSegment.end = Vector3::Transform(localSegment.end, m_WorldLocal);
	// 線分の方向ベクトルを取得
	Vector3 segmentNormal = localSegment.end - localSegment.start;
	segmentNormal.Normalize();

	for (int i = 0; i < nTri; i++)
	{
		// 三角形の各頂点のインデックスを取得
		int index0 = m_Date->m_Indices[i * 3];
		int index1 = m_Date->m_Indices[i * 3 + 1];
		int index2 = m_Date->m_Indices[i * 3 + 2];
		// 各頂点のローカル座標を取得
		Vector3 pos0 = m_Date->m_Vertex[index0].pos;
		Vector3 pos1 = m_Date->m_Vertex[index1].pos;
		Vector3 pos2 = m_Date->m_Vertex[index2].pos;

		Triangle tri;

		// 3点から三角形を構築（TODO:先に計算しておく）
		ComputeTriangle(pos0, pos1, pos2, &tri);

		float temp_distance;
		Vector3 temp_inter;

		// 上方向ベクトルと法線の内積
		float dot = -segmentNormal.Dot(tri.Normal);
		// 上方向との角度差を計算
		float angle = acosf(dot);
		// 上方向との角度が限界角より大きければ、地面とみなさず、スキップ
		if (angle > limit_angle) continue;

		// 線分と三角形（ポリゴン）の交差判定
		if (CheckSegment2Triangle(localSegment, tri, &temp_inter))
		{
			hit = true;
			// 衝突点までの距離を算出
			temp_distance = Vector3::Distance(localSegment.start, temp_inter);
			// 距離が一番近いものを記録
			if (temp_distance < distance)
			{
				l_inter = temp_inter;
				distance = temp_distance;
			}
		}
	}

	if (hit && inter != nullptr)
	{
		// 交点をモデル座標系からワールド座標系に変換
		const Matrix& localworld = m_obj.GetLocalWorld();
		*inter = Vector3::Transform(l_inter, localworld);
	}

	return hit;
}
