#pragma once
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>


class ParticleManager
{
public:
	ParticleManager();
	ParticleManager(ID3D11DeviceContext *context);
	ParticleManager(ID3D11DeviceContext *context, uint32_t num);
	virtual ~ParticleManager();

	// ������
	void Initialize();
	void Create(ID3D11DeviceContext *context);
	void Create(ID3D11DeviceContext *context, uint32_t num);

	// �X�V
	void Update();

	// �G���g���[
	void Entry(
		int frame,
		float sx, float sy, float sz,
		float vx, float vy, float vz,
		float ax, float ay, float az,
		float startAngle, float endAngle,
		float startScale, float endScale,
		float startRed, float startGreen, float startBlue, float startAlpha,
		float endRed, float endGreen, float endBlue, float endAlpha
		);

	// �`��
	void Draw(DirectX::CXMMATRIX view, DirectX::CXMMATRIX proj);

	// �e�N�X�`�����[�h
	void LoadTexture(const wchar_t* filename);

	// �e�N�X�`���̐ݒ�
	void SetTexture(ID3D11ShaderResourceView* texture);

	// �d�͂̐ݒ�
	void SetGravity(float gx, float gy, float gz);

	// �r���{�[�h�s��̐ݒ�
	void SetBillboard(DirectX::CXMMATRIX billboadTransform);

	// �r���{�[�h�̎����ݒ�
	void SetAutoBillboard();

	// �G���g���[�̃��Z�b�g
	void Reset();

	// ���
	std::wstring ToString();

private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};