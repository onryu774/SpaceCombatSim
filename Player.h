#pragma once
#include"Game\Obj3D.h"
#include"Game\LandShape.h"
#include"Game\CollisionNode.h"
#include"BulletShot.h"
#include"DirectXTK.h"
class Player
{
public:
	Player();
	~Player();
	//������
	void Initialaiz();
	//�X�V����
	void Update();
	//�v�Z
	void Calc();
	//�`��
	void Draw();
	//�ʒu�̎擾
	const DirectX::SimpleMath::Vector3& GetTrans();
	//�e�̈ʒu�̎擾
	const DirectX::SimpleMath::Vector3& GetBulletTrans();
	//��]�̎擾
	const DirectX::SimpleMath::Vector3& GetRot();
	//�ʒu�̐ݒ�
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	//�e�̍đ��U
	void ResetBullet() { Bullet->ResetBullet(&obj.GetLocalWorld()); }
	//���[�J�����[���h�s��̎擾
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	//���@�̂����蔻��̎擾
	const CapsuleNode& GetCollisionNodeBullet() { return collisionNodeBullet; }
	//�e�̂����蔻��̎擾
	const SphereNode& GetCollisionNodeBody() { return collisionNodeBody; }
protected:
	//�v���C���[�̃��f��
	Obj3D obj;
	//�^�[�Q�b�g�T�C�g�̃��f��
	Obj3D Tage;
private:
	//�e�̃N���X
	BulletShot* Bullet;
	//�e���ł��ꂽ���ǂ���
	bool m_timeflag;
	//�e�̐i�ރx�N�g��
	DirectX::SimpleMath::Vector3 m_BulletVel;
	//�e�����˂���Ă���̃t���[��
	int m_frame;
	//�e�����˂���Ă���̕b��
	int m_time;
	//�e�̂����蔻��
	CapsuleNode collisionNodeBullet;
	//���@�̂����蔻��
	SphereNode  collisionNodeBody;


};

