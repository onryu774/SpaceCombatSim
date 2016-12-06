#pragma once
#include "..\DirectXTK.h"
#include <SimpleMath.h>
class Camera
{
private:
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Vector3 m_Eye;
	DirectX::SimpleMath::Vector3 m_Tage;
	DirectX::SimpleMath::Vector3 m_Up;

	DirectX::SimpleMath::Matrix m_Proj;

	float m_FovY;
	float m_Aspect;
	float m_NClip;
	float m_FClip;
public:
	Camera();
	~Camera();
	void Update();
	void SetEye(const DirectX::SimpleMath::Vector3 pos) { m_Eye = pos; }
	void SetTage(const DirectX::SimpleMath::Vector3 pos) { m_Tage = pos; }
	void SetUp(const DirectX::SimpleMath::Vector3 vec) { m_Up = vec; }

	void SetFovY(const float FovY) { m_FovY = FovY; }
	void SetAspect(const float Aspect) { m_Aspect = Aspect; }
	void SetNClip(const float NClip) { m_NClip = NClip; }
	void SetFClip(const float FClip) { m_FClip = FClip; }

	const DirectX::SimpleMath::Vector3& GetEye()const { return m_Eye; }
	const DirectX::SimpleMath::Vector3& GetTage()const { return m_Tage; }
	const DirectX::SimpleMath::Vector3& GetUp()const { return m_Up; }
	const DirectX::SimpleMath::Matrix& GetView()const { return m_view; }
	const DirectX::SimpleMath::Matrix& GetProj()const { return m_Proj; }

};

