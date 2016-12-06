//////////////////////////////
//�q�b�g�G�t�F�N�g
//����@���c���o
///////////////////////////////
#include"HitEffect.h"
#include<SimpleMath.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;

float RandomRange(float min, float max);
//������
void HitEffect::Initialize(ParticleManager *argParticleManager, float x, float y, float z, int argCount)
{
	particleManager = argParticleManager;





	pos_x = x;
	pos_y = y;
	pos_z = z;

	

	count = argCount;


}
//���s����
bool HitEffect::Run()
{
	if (--count < 0) return false;

	

	particleManager->Entry(
		60,//�\���p�[�e�B�N����
		pos_x, pos_y, pos_z,//�p�[�e�B�N���n�܂�ʒu
		RandomRange(-0.2f,0.2f), RandomRange(-0.2f, 0.2f), RandomRange(-0.2f, 0.2f),//�p�[�e�B�N���̏I���ʒu
		0.0f, 0.0f, 0.0f,//�����x
		0.0f, 0.0f,//��]
		0.1f, 1.0f,//�傫��
		1.0f, 0.5f, 0.5f, 1.0f,//�n�܂�̐F
		1.0f, 1.0f, 1.0f, 1.0f//������Ƃ��̐F
		);


	return true;
}
