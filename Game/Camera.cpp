#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera():m_FovY(XMConvertToRadians(60.0f)),m_Aspect(640.0f/480.0f),m_NClip(0.1f),m_FClip(1000.0f)
{
	m_view = Matrix::Identity;
	m_Eye = Vector3(0.0f, 6.0f, 5.0f);
	m_Tage= Vector3(0.0f, 2.0f, 0.0f);
	m_Up= Vector3(0.0f, 1.0f, 0.0f);
	m_Proj= Matrix::Identity;
}


Camera::~Camera()
{
}

void Camera::Update()
{
	m_view = Matrix::CreateLookAt(m_Eye, m_Tage, m_Up);
	m_Proj = Matrix::CreatePerspectiveFieldOfView(m_FovY, m_Aspect, m_NClip, m_FClip);
}
