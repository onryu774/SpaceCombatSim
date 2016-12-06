/////////////////////////////////
//�G�̍s���Ȃǂ̐ݒ�
//
//����@���c���o
////////////////////////////////

#include"EnemyShip.h"
#include<SimpleMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

float RandomRange(float min, float max);


EnemyShip::EnemyShip()
{
}

EnemyShip::~EnemyShip()
{
}
//����������
void EnemyShip::Initialaiz(const float x, const float y, const float z)
{
	//���f���̓ǂݍ���
	obj.LoadModelFile(L"Resources/cModels/Enemyfighter.cmo");
	//�N�H�[�^�j�I���̎g�p
	obj.OnQuaater();
	//���f���̃T�C�Y�ύX
	obj.SetScale(Vector3(1.0f, 1.0f, 1.0f));
	int m_x = x;
	int m_y = y;
	int m_z = z;
	//���f���̏����ʒu�̐ݒ�
	obj.SetTrans(Vector3(m_x,m_y,m_z));
	//�ŏ��̖ړI�n�̐ݒ�
	Ptage = Vector3(RandomRange(-200, 200), RandomRange(-200, 200), RandomRange(-200, 200));
	//�����蔻��̏�����
	collisionNodeBody.Initialize();
	//�e�s��̐ݒ�
	collisionNodeBody.SetParentMatrix(&obj.GetLocalWorld());
	//�����蔻��̉�]
	collisionNodeBody.SetRot(Vector3(XMConvertToRadians(90),0.0f,00.0f));
	//�����蔻��̔��a
	collisionNodeBody.SetLocalRadius(0.2f);
	//�����蔻��̒���
	collisionNodeBody.SetLocalLength(1.5f);
	//�����蔻��̈ʒu�ݒ�
	collisionNodeBody.SetTrans(Vector3(0.0f, 0.0f, -0.7f));
	//���C�t�̐ݒ�
	m_Lifes = 2;
	
	insite = false;
}
//�X�V����
void EnemyShip::Update(const Vector3 Tage_Pos)
{
	//���f���̈ʒu���擾
	Vector3 trans = obj.GetTrans();

	Quaternion RotQ;
	//�i�ރx�N�g����ݒ�
	move = Vector3(0.0f, 0.0f, 0.1f);
	//���@�������Ă��邩�ǂ���
	insite = EnemyView(Tage_Pos, move);
	m_frame++;
	if (m_frame >= 60)
	{
		m_time++;
		m_frame = 0;
	}
	

	//�s���p�^�[���̕ύX
	if (insite == true)
	{
		RotQ=FinedMove(Tage_Pos);
	}
	else if(insite==false)
	{
		RotQ=NoFinedMove();
	}
	//�ړ��̌v�Z
	trans = trans + move;
	obj.SetTrans(trans);
	obj.SetRotQ(RotQ);
	Calc();
	//�����蔻��̍X�V
	collisionNodeBody.Update();
	
}
//�ʒu�Ȃǂ̌v�Z
void EnemyShip::Calc()
{
	obj.Calc();
}
//�`��
void EnemyShip::Draw()
{
	obj.Draw();
	collisionNodeBody.Draw();
}
//�ʒu�̎擾
const DirectX::SimpleMath::Vector3 & EnemyShip::GetTrans()
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return obj.GetTrans();
}
//��]�̎擾
const DirectX::SimpleMath::Vector3 & EnemyShip::GetRot()
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return obj.GetRot();
}
//�ʒu�̐ݒ�
void EnemyShip::SetTrans(const DirectX::SimpleMath::Vector3 & trans)
{
	obj.SetTrans(trans);
}
//�����Ă��邩�ǂ���
//true:�����Ă���
//false:�����Ă��Ȃ�
bool EnemyShip::Livese()
{
	if (m_Lifes<=0)
	{
		return false;
	}
	else
	{
		return true;
	}
	/*return true;*/
}
//���[�J�����[���h�s��̎擾
const DirectX::SimpleMath::Matrix & EnemyShip::GetLocalWorld()
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return obj.GetLocalWorld();
}
//����
//true:�v���C���[��������Ă���
//false�F�v���C���[�������Ă��Ȃ�
bool EnemyShip::EnemyView(const Vector3 Tage_Pos,const Vector3 Enemymove)
{
	//����͈�
	int halfradians;
	halfradians = 10;
	//�^�[�Q�b�g�ւ̃x�N�g��
	Vector3 Score;
	Score = Tage_Pos - obj.GetTrans();
	//�^�[�Q�b�g�ւ̃x�N�g���̒���
	float Scorelength;
	Scorelength = Score.Length();
	//�G�̐i��ł���x�N�g���̒���
	float Enemymovelength;
	Enemymovelength = Enemymove.Length();
	//��̃x�N�g���̂Ȃ��p�����߂�icosin�j
	float cos = Score.Dot(Enemymove)/(Scorelength*Enemymovelength);
	//cosin�����W�A���p�ɕϊ�
	float radians = acos(cos);
	//�v���C���[�������Ă��邩�ǂ����̔���
	if ((radians<=90)&&((obj.GetTrans().x-Tage_Pos.x)*(obj.GetTrans().x - Tage_Pos.x)+ (obj.GetTrans().z - Tage_Pos.z)*(obj.GetTrans().z - Tage_Pos.z)<=halfradians*halfradians)
		&& ((obj.GetTrans().y - Tage_Pos.y)*(obj.GetTrans().x - Tage_Pos.x) + (obj.GetTrans().z - Tage_Pos.z)*(obj.GetTrans().z - Tage_Pos.z) <= halfradians*halfradians))
	{
		return true;
	}
	return false;
}
//�v���C���[��������Ă��Ȃ��Ƃ��̍s��
DirectX::SimpleMath::Quaternion EnemyShip::NoFinedMove()
{
	
	//2�b���ƂɖړI�n�̐ݒ�
	if (m_time>=2)
	{
		Ptage = Vector3(RandomRange(-200, 200), RandomRange(-200, 200), RandomRange(-200, 200));
		m_time = 0;
	}
	//�ړI�n�ւ̉�]�s��̐ݒ�
	Matrix rote =LooKAtMatrix(Ptage);
	Quaternion RotQ;
	//�}�g���b�N�X���N�H�[�^�j�I���ɕϊ�
	RotQ = Quaternion::CreateFromRotationMatrix(rote);
	//�}�g���b�N�X��Vector3�ɕϊ�
	move = Vector3::TransformNormal(move, rote);
	
	return RotQ;
	
}
//�v���C���[�������Ă���Ƃ��̍s��
DirectX::SimpleMath::Quaternion EnemyShip::FinedMove(const Vector3 Tage_Pos)
{

	Matrix roteS;
	//�v���C���[�ւ̉�]�s��
	roteS = LooKAtMatrix(Tage_Pos);
	Quaternion RotQ;
	// �}�g���b�N�X���N�H�[�^�j�I���ɕϊ�
	RotQ = Quaternion::CreateFromRotationMatrix(roteS);
	//�}�g���b�N�X��Vector3�ɕϊ�
	move = Vector3::TransformNormal(move, roteS);
	return RotQ;
}


//��]�s��̌v�Z
DirectX::SimpleMath::Matrix EnemyShip::LooKAtMatrix(const DirectX::SimpleMath::Vector3 Tage_Pos)
{
	//��]�s��̊i�[
	Matrix Point;
	//���ݒn
	Vector3 pos; 
	pos = obj.GetTrans();;
	//�ړI�n
	Vector3 tage;
	tage = Tage_Pos ;
	Vector3 up; 
	up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 X;
	Vector3 Y;
	Vector3 V;
	Vector3 Z;
	//���̌v�Z
	V = tage - pos;
	V.Normalize();
	X = up.Cross(V);
	X.Normalize();
	Y = V.Cross(X);
	Y.Normalize();
	Z = X.Cross(Y);
	Point._11 = X.x;  Point._12 = X.y;	 Point._13 = X.z;  Point._14 = 0;
	Point._21 = Y.x;  Point._22 = Y.y;	 Point._23 = Y.z;  Point._24 = 0;
	Point._31 = V.x;  Point._32 = V.y;	 Point._33 = V.z;  Point._34 = 0;
	Point._41 = pos.x;  Point._42 = pos.y;	 Point._43 = pos.z;  Point._44 = 1.0f;
	return Point;
}

