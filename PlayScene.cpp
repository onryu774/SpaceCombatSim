#include "PlayScene.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

float RandomRange(float min, float max);
PlayScene::PlayScene()
{
}
//�Q�[���v���C�V�[���̏���������
void PlayScene::PlaySceneInit()
{
	static const int Limit_Time = 120;
	//�J�����̐���
	camera = new Camera;
	//�G�̐���
	for (int i = 0; i < ENEMYS_NUM; i++)
	{
		Enemys[i] = new EnemyShip;
	}
	//�v���C���[�̐���
	player = new Player;
	//�w�i�̐���
	ten = new Obj3D;
	//�Q�[���̐�������
	Timelimit = Limit_Time;
	frem = 0;
	factory = new EffectFactory(g_pd3dDevice.Get());

	//�^�X�N�������p�[�e�B�N���̎g�p���邽�߂̏�����
	{
		//�^�X�N�}�l�[�W���̏�����
		taskManager.reset(new TaskManager());
		//�p�[�e�B�N���̏�����
		particleManager.reset(new ParticleManager(g_pImmediateContext.Get()));
		particleManager->LoadTexture(L"Resources\\Media\\bit");
		particleManager->SetAutoBillboard();
		/*particleManager2.reset(new ParticleManager(g_pImmediateContext.Get()));
		particleManager2->LoadTexture(L"Resources\\Media\\break");
		particleManager2->SetAutoBillboard();*/
	}

	//obj3D�̏�����
	{
		Obj3D::SetDevice(g_pd3dDevice.Get());
		Obj3D::SetDeviceContext(g_pImmediateContext.Get());
		Obj3D::SetStates(g_state.get());
		Obj3D::SetEffectFactory(factory);
		Obj3D::SetCamera(camera);
	}
	///��Q���̏�����
	/*def.pCamera = camera;
	def.pDevice = g_pd3dDevice.Get();
	def.pDeviceContext = g_pImmediateContext.Get();
	LandShape::InitializeCommon(def);
	for (int i = 0; i < OBJCT_NUM; i++)
	{
		s_LandshapeTable[i] = { Vector3(RandomRange(-50,50),RandomRange(-50,50),RandomRange(-50,50)),Vector3(0, 0, 0),0.05f,L"Resources/LandShape/lokc.MDL",L"Resources/cModels/lokc.cmo" };
	}
	landShape_tablenum = sizeof(s_LandshapeTable) / sizeof(LandshapeTable);
	landShapeArray.resize(landShape_tablenum);
	for (int i = 0; i < landShape_tablenum; i++)
	{
		LandshapeTable*pTable = &s_LandshapeTable[i];
		LandShape* land = new LandShape;
		wchar_t*filename_MDL = nullptr;
		wchar_t*filename_CMO = nullptr;
		if (lstrcmpW(pTable->filename_MDL, L"") != 0)filename_MDL = pTable->filename_MDL;
		if (lstrcmpW(pTable->filename_CMO, L"") != 0)filename_CMO = pTable->filename_CMO;
		Vector3 Rot = pTable->rot;
		Rot.x = XMConvertToRadians(pTable->rot.x);
		Rot.y = XMConvertToRadians(pTable->rot.y);
		Rot.z = XMConvertToRadians(pTable->rot.z);

		land->Initialize(filename_MDL, filename_CMO);
		land->SetTrans(pTable->trans);
		land->SetRot(Rot);
		land->SetScale(pTable->scale);

		landShapeArray[i] = land;

	}*/

	    //�w�i�̃��f�����[�h
		ten->LoadModelFile(L"Resources/cModels/Utyu.cmo");
		//�v���C���[�̏�����
		player->Initialaiz();
		//�G�l�~�[�̏�����
		for (int i = 0; i < ENEMYS_NUM; i++)
		{

			Enemys[i]->Initialaiz(-25 + 5 * i, 0, -30);
		}
	
	debugflag = false;
}

bool PlayScene::PlaySceneRun()
{
	//�J�����̍X�V
	camera->Update();
	//�t���[���̒ǉ�
	frem++;
	//�t���[����60�ȏ�ɂȂ����琧�����Ԃ���1�b�����ăt���[����0�ɂ���
	if (frem >= 60)
	{
		frem = 0;
		Timelimit--;
	}
	//�������Ԃ��I�������false��Ԃ�
	if (Timelimit == 0)
	{
		return false;
	}
	//�����蔻��̃f�o�b�N����
	if (g_keyTracker->pressed.D1)
	{
		CollisionNode::SetDebugVisible(!debugflag);
	}

	//�v���C���[�̍X�V
	player->Update();
	//�G�̍X�V
	for (int i = 0; i < ENEMYS_NUM; i++)
	{
		//�G�������Ă���Ȃ�X�V�����s����
		if (Enemys[i]->Livese() == true) 
		{
			Enemys[i]->Update(player->GetTrans());
		}
	}
	//�����蔻��̃`�F�b�N
	HitCheck();
	
	//�p�[�e�B�N���̍X�V
	particleManager->Update();
	/*particleManager2->Update();*/
	//�^�X�N�̎��s
	taskManager->Run();

	
	return true;
}

void PlayScene::HitCheck()
{

	//�e�ƓG�̂����蔻��
	///////////////////////////////////////////////////////////////////////////////////
	//�e�̂����蔻����擾����
	const Capsule&bulletcapsule = player->GetCollisionNodeBullet();
	for (size_t i = 0; i < ENEMYS_NUM; i++)
	{
		//�G�������Ă��邩�ǂ���
		if (Enemys[i]->Livese() == false)
		{
			continue;
		}
		//�G�̂����蔻����擾����
		const Capsule& enemyLegSphere = Enemys[i]->GetCollisionNodeBody();
		//���������W���i�[����
		Vector3 inter;
		//�����蔻��
		if (CheckCapsule2Capsule(enemyLegSphere, bulletcapsule, &inter) == true)
		{
			//�^�X�N�}�l�[�W���ɂ��q�b�g���̃p�[�e�B�N���G�t�F�N�g�̎��s
			taskManager->Add<HitEffect>()->Initialize(particleManager.get(), inter.x, inter.y, inter.z, 5);
			//�_���[�W�̎��s
			Enemys[i]->Damegy();
			//�G���j�󂳂ꂽ��^�X�N�}�l�[�W���ɂ��j�󎞂̃p�[�e�B�N���G�t�F�N�g�̎��s
			if (Enemys[i]->Livese() == false)
			{
				taskManager->Add<Contrail>()->Initialize(particleManager.get(), Enemys[i]->GetTrans().x, Enemys[i]->GetTrans().y, Enemys[i]->GetTrans().z, 5);
			}
			//�v���C���[�̒e�̍đ��U
			player->ResetBullet();


		}
	}
	//////////////////////////////////////////////////////////////////////////////

	//�v���C���[�ƒn�`���Q���̂����蔻��
	///////////////////////////////////////////////////////////////////////////////////////////////
	/*Sphere sphere = player->GetCollisionNodeBody();
	Vector3 trans = player->GetTrans();
	Vector3 sphere2player = trans - sphere.center;
	for (std::vector<LandShape*>::iterator it = landShapeArray.begin(); it != landShapeArray.end(); it++)
	{
		LandShape* pLandShape = *it;
		Vector3 reject;
		if (pLandShape->IntersectSphere(sphere, &reject))
		{
			sphere.center = sphere.center + reject;

		}

	}
	player->SetTrans(sphere.center + sphere2player);
	player->Calc();*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
}


void PlayScene::PlaySceneRender()
{
	//���[���h�s���ݒ�
	Matrix world = Matrix::Identity;
	//�J�����ƃv���C���[�̋���
	const float CAMERA_DISTANS = 2.0f/*0.001f*/;
	Vector3 Eyes, Refs, Up;
	Refs = player->GetTrans() + Vector3(0.0f, 0.1f, 0.0f);
	Vector3  CameraV(0.0f, 0.5f, CAMERA_DISTANS);
	CameraV = Vector3::TransformNormal(CameraV, player->GetLocalWorld());
	Eyes = Refs + CameraV;

	Vector3 rote = player->GetRot();
	Vector3 dir(0, 1, 0);
	//������]������
	Matrix rotZ = Matrix::CreateRotationZ(rote.z);
	Matrix rotX = Matrix::CreateRotationX(rote.x);
	Matrix rotY = Matrix::CreateRotationY(rote.y);
	Matrix rot = rotX*rotY*rotZ;
	//����x�N�g���̐ݒ�
	dir = Vector3::Transform(dir, rot);
	Up = dir;

	Eyes = camera->GetEye() + (Eyes - camera->GetEye()) * 0.25f;
	Refs = camera->GetTage() + (Refs - camera->GetTage())*0.25f;
	//�J�����̐ݒ�
	camera->SetEye(Eyes);
	camera->SetTage(Refs);
	camera->SetUp(Up);
	//�r���[�s���ݒ�
	Matrix view = camera->GetView();
	//�v���W�F�N�V�����s���ݒ肷��
	Matrix proj = camera->GetProj();
	//�w�i�̕`��
	ten->Draw();



	/*LandShape*landShape = landShapeArray[0];
	landShape->SetVelocity(Vector3(0, 0, 0));

	for (std::vector<LandShape*>::iterator it = landShapeArray.begin(); it != landShapeArray.end(); it++)
	{
		LandShape*landShape = *it;
		if (landShape == nullptr)continue;
		landShape->Calc();

	}
	for (std::vector<LandShape*>::iterator it = landShapeArray.begin(); it != landShapeArray.end(); it++)
	{
		LandShape*landShape = *it;
		if (landShape == nullptr)continue;
		landShape->Draw();
	}*/
	//�v���C���[�̕`��
	player->Draw();
	//�p�[�e�B�N���̕`��
	particleManager->Draw(view, proj);
	/*particleManager2->Draw(view, proj);*/
	//�G�̕`��
	for (int i = 0; i < ENEMYS_NUM; i++)
	{
		if (Enemys[i]->Livese() == true)
			Enemys[i]->Draw();
	}
}


PlayScene::~PlayScene()
{
	delete[] Enemys;
	delete camera;
	delete player;
	delete ten;
}
