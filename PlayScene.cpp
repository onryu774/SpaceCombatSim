#include "PlayScene.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

float RandomRange(float min, float max);
PlayScene::PlayScene()
{
}
//ゲームプレイシーンの初期化処理
void PlayScene::PlaySceneInit()
{
	static const int Limit_Time = 120;
	//カメラの製作
	camera = new Camera;
	//敵の製作
	for (int i = 0; i < ENEMYS_NUM; i++)
	{
		Enemys[i] = new EnemyShip;
	}
	//プレイヤーの製作
	player = new Player;
	//背景の製作
	ten = new Obj3D;
	//ゲームの制限時間
	Timelimit = Limit_Time;
	frem = 0;
	factory = new EffectFactory(g_pd3dDevice.Get());

	//タスク処理＆パーティクルの使用するための初期化
	{
		//タスクマネージャの初期化
		taskManager.reset(new TaskManager());
		//パーティクルの初期化
		particleManager.reset(new ParticleManager(g_pImmediateContext.Get()));
		particleManager->LoadTexture(L"Resources\\Media\\bit");
		particleManager->SetAutoBillboard();
		/*particleManager2.reset(new ParticleManager(g_pImmediateContext.Get()));
		particleManager2->LoadTexture(L"Resources\\Media\\break");
		particleManager2->SetAutoBillboard();*/
	}

	//obj3Dの初期化
	{
		Obj3D::SetDevice(g_pd3dDevice.Get());
		Obj3D::SetDeviceContext(g_pImmediateContext.Get());
		Obj3D::SetStates(g_state.get());
		Obj3D::SetEffectFactory(factory);
		Obj3D::SetCamera(camera);
	}
	///障害物の初期化
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

	    //背景のモデルロード
		ten->LoadModelFile(L"Resources/cModels/Utyu.cmo");
		//プレイヤーの初期化
		player->Initialaiz();
		//エネミーの初期化
		for (int i = 0; i < ENEMYS_NUM; i++)
		{

			Enemys[i]->Initialaiz(-25 + 5 * i, 0, -30);
		}
	
	debugflag = false;
}

bool PlayScene::PlaySceneRun()
{
	//カメラの更新
	camera->Update();
	//フレームの追加
	frem++;
	//フレームが60以上になったら制限時間から1秒引いてフレームを0にする
	if (frem >= 60)
	{
		frem = 0;
		Timelimit--;
	}
	//制限時間が終わったらfalseを返す
	if (Timelimit == 0)
	{
		return false;
	}
	//あたり判定のデバック処理
	if (g_keyTracker->pressed.D1)
	{
		CollisionNode::SetDebugVisible(!debugflag);
	}

	//プレイヤーの更新
	player->Update();
	//敵の更新
	for (int i = 0; i < ENEMYS_NUM; i++)
	{
		//敵が生きているなら更新を実行する
		if (Enemys[i]->Livese() == true) 
		{
			Enemys[i]->Update(player->GetTrans());
		}
	}
	//当たり判定のチェック
	HitCheck();
	
	//パーティクルの更新
	particleManager->Update();
	/*particleManager2->Update();*/
	//タスクの実行
	taskManager->Run();

	
	return true;
}

void PlayScene::HitCheck()
{

	//弾と敵のあたり判定
	///////////////////////////////////////////////////////////////////////////////////
	//弾のあたり判定を取得する
	const Capsule&bulletcapsule = player->GetCollisionNodeBullet();
	for (size_t i = 0; i < ENEMYS_NUM; i++)
	{
		//敵が生きているかどうか
		if (Enemys[i]->Livese() == false)
		{
			continue;
		}
		//敵のあたり判定を取得する
		const Capsule& enemyLegSphere = Enemys[i]->GetCollisionNodeBody();
		//当った座標を格納する
		Vector3 inter;
		//当たり判定
		if (CheckCapsule2Capsule(enemyLegSphere, bulletcapsule, &inter) == true)
		{
			//タスクマネージャによるヒット時のパーティクルエフェクトの実行
			taskManager->Add<HitEffect>()->Initialize(particleManager.get(), inter.x, inter.y, inter.z, 5);
			//ダメージの実行
			Enemys[i]->Damegy();
			//敵が破壊されたらタスクマネージャによる破壊時のパーティクルエフェクトの実行
			if (Enemys[i]->Livese() == false)
			{
				taskManager->Add<Contrail>()->Initialize(particleManager.get(), Enemys[i]->GetTrans().x, Enemys[i]->GetTrans().y, Enemys[i]->GetTrans().z, 5);
			}
			//プレイヤーの弾の再装填
			player->ResetBullet();


		}
	}
	//////////////////////////////////////////////////////////////////////////////

	//プレイヤーと地形や障害物のあたり判定
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
	//ワールド行列を設定
	Matrix world = Matrix::Identity;
	//カメラとプレイヤーの距離
	const float CAMERA_DISTANS = 2.0f/*0.001f*/;
	Vector3 Eyes, Refs, Up;
	Refs = player->GetTrans() + Vector3(0.0f, 0.1f, 0.0f);
	Vector3  CameraV(0.0f, 0.5f, CAMERA_DISTANS);
	CameraV = Vector3::TransformNormal(CameraV, player->GetLocalWorld());
	Eyes = Refs + CameraV;

	Vector3 rote = player->GetRot();
	Vector3 dir(0, 1, 0);
	//軸を回転させる
	Matrix rotZ = Matrix::CreateRotationZ(rote.z);
	Matrix rotX = Matrix::CreateRotationX(rote.x);
	Matrix rotY = Matrix::CreateRotationY(rote.y);
	Matrix rot = rotX*rotY*rotZ;
	//上方ベクトルの設定
	dir = Vector3::Transform(dir, rot);
	Up = dir;

	Eyes = camera->GetEye() + (Eyes - camera->GetEye()) * 0.25f;
	Refs = camera->GetTage() + (Refs - camera->GetTage())*0.25f;
	//カメラの設定
	camera->SetEye(Eyes);
	camera->SetTage(Refs);
	camera->SetUp(Up);
	//ビュー行列を設定
	Matrix view = camera->GetView();
	//プロジェクション行列を設定する
	Matrix proj = camera->GetProj();
	//背景の描写
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
	//プレイヤーの描写
	player->Draw();
	//パーティクルの描写
	particleManager->Draw(view, proj);
	/*particleManager2->Draw(view, proj);*/
	//敵の描写
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
