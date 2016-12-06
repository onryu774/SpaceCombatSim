#pragma once
#include"Game\Obj3D.h"
#include"Game\LandShape.h"
#include"Game\CollisionNode.h"
#include"DirectXTK.h"
class EnemyShip
{
public:
	EnemyShip();
	~EnemyShip();
	//������
	void Initialaiz(const float x,const float y,const float z);
	//�X�V
	void Update(const DirectX::SimpleMath::Vector3 Tage_Pos);
	//���f���Ȃǂ̌v�Z
	void Calc();
	//�`��
	void Draw();
	//�ʒu�̎擾
	const DirectX::SimpleMath::Vector3& GetTrans();
	//��]�̎擾
	const DirectX::SimpleMath::Vector3& GetRot();
	//�ʒu�̐ݒ�
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	//�_���[�W���󂯂�
	void Damegy() { m_Lifes--;}
	//�����Ă��邩�ǂ���
	bool Livese();
	//���[�J�����[���h�s��̎擾
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	//�����蔻��̎擾
	const CapsuleNode& GetCollisionNodeBody() { return collisionNodeBody; }
	
protected:
	//3D���f��
	Obj3D obj;
private:
	//��]�s��̌v�Z
	DirectX::SimpleMath::Matrix LooKAtMatrix(const DirectX::SimpleMath::Vector3 Tage_Pos);
	//�ړI�n�̃x�N�g��
	DirectX::SimpleMath::Vector3 Ptage;
	//�i�ޕ����x�N�g��
	DirectX::SimpleMath::Vector3 move;
	//�v���C���[�������Ă��邩�ǂ�������
	bool EnemyView(const DirectX::SimpleMath::Vector3 Tage_Pos, const DirectX::SimpleMath::Vector3 EnemyMove);
	//�v���C���[�������Ă��Ȃ��Ƃ��̍s��
    DirectX::SimpleMath::Quaternion NoFinedMove();
	//�v���C���[�������Ă���Ƃ��̍s��
	DirectX::SimpleMath::Quaternion FinedMove(const DirectX::SimpleMath::Vector3 Tage_Pos);
	//�v���C���[�������Ă��邩�ǂ���
	bool insite;
	//�t���[��
	int m_frame;
	//����
	int m_time;
	//���C�t
	int m_Lifes;
	//�����蔻��
	CapsuleNode  collisionNodeBody;
};