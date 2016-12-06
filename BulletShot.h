#pragma once
#include"Game\Obj3D.h"
#include"Game\LandShape.h"
#include"Game\CollisionNode.h"
#include"DirectXTK.h"
///////
//�e�̃N���X
///////
class BulletShot
{
private:
	//���f��
	Obj3D obj;
	//�����蔻��
	CapsuleNode collisionNodeBullet;
	//�e�̐i�ރx�N�g��
	DirectX::SimpleMath::Vector3 m_BulletVel;
	bool m_flag;
public:
	BulletShot();
	~BulletShot();
	//������
	void Initialaiz(const DirectX::SimpleMath::Matrix*Parent);
	//�X�V
	void Update();
	///�v�Z
	void Calc();
	//�`��
	void Draw();
	//�e�̍đ��U
	void ResetBullet(const DirectX::SimpleMath::Matrix*Parent);
	//�N�H�[�^�j�I�����g�p����
	void ONQ() { obj.OnQuaater(); };
	//�N�H�[�^�j�I�����g�p���Ȃ�
	void OFFQ() { obj.OffQuaater(); };
	//�e�����������ǂ���
	bool flage() { return m_flag; };
	const DirectX::SimpleMath::Vector3& GetTrans();
	const DirectX::SimpleMath::Vector3& BulletVel() { return m_BulletVel; };
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	//�����蔻��̎擾
	const CapsuleNode& GetCollisionNodeBullet() { return collisionNodeBullet; }
	void BulletUpdate() { collisionNodeBullet.Update(); };

};

