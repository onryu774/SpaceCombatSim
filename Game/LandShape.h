#pragma once
#include<vector>
#include<memory>
#include<SimpleMath.h>
#include"LandShapeDate.h"
#include"Camera.h"
#include"Obj3D.h"
#include"Collision.h"
struct LandShapeCommonDef
{
	ID3D11Device*pDevice;
	ID3D11DeviceContext*pDeviceContext;
	Camera*pCamera;
};


class LandShapeCommon
{
public:
	friend class LandShape;
	LandShapeCommon(LandShapeCommonDef def);
	~LandShapeCommon();
protected:
	Camera*m_pCamera;

	std::unique_ptr<DirectX::CommonStates>m_pStates;
	std::unique_ptr<DirectX::EffectFactory>m_pEffectFactory;
	std::unique_ptr<DirectX::BasicEffect>m_pEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>>m_pPrimitiveBatch;
	ID3D11InputLayout*m_pInputLayout;
	ID3D11DeviceContext*m_pDeviceContext;
};

class LandShape
{
public:
	static void InitializeCommon(LandShapeCommonDef def);
	LandShape();
	void Initialize(const wchar_t*filename_mdl,const wchar_t*filename_cmo=nullptr);
	void Calc();
	void Draw();

	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_obj.SetTrans(trans); }
	void SetRot(const DirectX::SimpleMath::Vector3& rot) { m_obj.SetRot(rot); }
	void SetScale(const float scale) { m_obj.SetScale(DirectX::SimpleMath::Vector3(scale)); }
	void SetLocalWorld(const DirectX::SimpleMath::Matrix& mat) { m_obj.SetLocalWorld(mat); }
	void SetVelocity(const DirectX::SimpleMath::Vector3& Velocity){ m_Velocity=Velocity; }
	const DirectX::SimpleMath::Matrix& GetLocalWorld() { return m_obj.GetLocalWorld(); }
	const DirectX::SimpleMath::Vector3& GetTrans() { return m_obj.GetTrans(); }
	const DirectX::SimpleMath::Vector3& GetRot() { return m_obj.GetRot();}
	const DirectX::SimpleMath::Vector3& GetScale() { return m_obj.GetScale();}
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_Velocity;}

	bool IntersectSphere(const Sphere&sphere, DirectX::SimpleMath::Vector3* reject);
	bool IntersectSegment(const Segment& segment, DirectX::SimpleMath::Vector3* inter);
protected:
	static std::unique_ptr<LandShapeCommon>s_pCommon;
	static std::map<std::wstring, std::unique_ptr<LandShapeDate>>s_dataaray;
	Obj3D m_obj;
	const LandShapeDate*m_Date;
	DirectX::SimpleMath::Vector3 m_Velocity;
	DirectX::SimpleMath::Matrix m_WorldLocal;

};

