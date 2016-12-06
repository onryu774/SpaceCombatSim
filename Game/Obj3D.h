#pragma once
#include<Model.h>
#include<Effects.h>
#include"Camera.h"
#include<map>
class Obj3D
{
private:
	// �ÓI�����o�ϐ��i�S�I�u�W�F�N�g�ŋ��L�j
	// �f�o�C�X�ւ̃|�C���^
	static ID3D11Device* s_pDevice;
	// �f�o�C�X�R���e�L�X�g�ւ̃|�C���^
	static ID3D11DeviceContext* Obj3D::s_pDeviceContext;
	// �`��X�e�[�g�ւ̃|�C���^
	static DirectX::CommonStates* Obj3D::s_pStates;
	// ���p�̃G�t�F�N�g�t�@�N�g��
	static DirectX::EffectFactory* s_pEffectFactory;
	// ���p�̃J�����i�`�掞�Ɏg�p�j
	static Camera* s_pCamera;
	// �ǂݍ��ݍς݃��f���R���e�i
	static std::map<std::wstring, std::unique_ptr<DirectX::Model>> s_modelarray;

	// ���f���f�[�^�ւ̃|�C���^
	const DirectX::Model* m_pModel;
	// ���s�ړ�
	DirectX::SimpleMath::Vector3 m_Trans;
	// ��]
	DirectX::SimpleMath::Vector3 m_Rot;
	// �X�P�[�����O
	DirectX::SimpleMath::Vector3 m_Scale;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_LocalWorld;
	// �e�̃��[���h�s��ւ̃|�C���^
	const DirectX::SimpleMath::Matrix* m_pParentMatrix;
	//�N�H�[�^�j�I��������
	bool m_UseQuaternion;

	DirectX::SimpleMath::Quaternion m_quaternipn;

public:
	static void SetDevice(ID3D11Device* pDevice) { s_pDevice = pDevice; }
	static void SetDeviceContext(ID3D11DeviceContext* pDeviceContext) { s_pDeviceContext = pDeviceContext; }
	static void SetStates(DirectX::CommonStates* pStates) { s_pStates = pStates; }
	static void SetEffectFactory(DirectX::EffectFactory* pfx) { s_pEffectFactory = pfx; }
	// �J������setter
	static void SetCamera(Camera* pCamera) {s_pCamera = pCamera; }

	Obj3D();
	virtual ~Obj3D();

	// �t�@�C�����烂�f����ǂݍ���
	void LoadModelFile(const wchar_t*filename = nullptr);
	// �s��̌v�Z
	void Calc();
	// �`��
	void Draw();
	// �I�u�W�F�N�g�̃��C�e�B���O�𖳌��ɂ���
	void DisableLighting();
	// �A�N�Z�b�T
	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_Trans = trans; }
	void SetRot(const DirectX::SimpleMath::Vector3& rot) { m_Rot = rot; }
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_Scale = scale; }
	void SetLocalWorld(const DirectX::SimpleMath::Matrix& mat) { m_LocalWorld = mat; }
	void SetParentMatrix(const DirectX::SimpleMath::Matrix* pParentMatrix) { m_pParentMatrix = pParentMatrix; }
	const DirectX::SimpleMath::Vector3& GetTrans() { return m_Trans; }
	const DirectX::SimpleMath::Vector3& GetRot() { return m_Rot; }
	const DirectX::SimpleMath::Vector3& GetScale() { return m_Scale; }
	const DirectX::SimpleMath::Matrix& GetLocalWorld() { return m_LocalWorld; }
	void SetRotQ(const DirectX::SimpleMath::Quaternion& rot) { m_quaternipn = rot; }
	DirectX::SimpleMath::Quaternion GetRotQ() {return m_quaternipn; }
	void OnQuaater() { m_UseQuaternion = true; }
	void OffQuaater() { m_UseQuaternion = false; }
};

