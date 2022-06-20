#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include<random>

const float XM_PI = 3.14592654f;

float ConvertRad(float dosuu);

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete model_;
	delete debugCamera_;
	
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("genba.jpg");
	model_ = Model::Create();

	std::random_device seed_gen;

	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> RadDist(0, ConvertRad(180.0f));
	std::uniform_real_distribution<float> TraDist(-10,10);

	


	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);

	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	for (size_t i = 0; i < _countof(worldTransform_2); i++) {
		// x,y,z方向のスケーリングを特定
		worldTransform_2[i].scale_ = { 5.0f, 1.0f, 1.0f };
		// x,y,z軸周りの回転角を設定
		//
		worldTransform_2[i].translation_ = { 0,0,float(i) * 10.0f };

		worldTransform_2[i].Initialize();
		worldTransform_2[i].UpdateMatrix();
		worldTransform_2[i].TransferMatrix();
	}

	

	worldTransforms_[kRoot].Initialize();

	worldTransforms_[kSpine].translation_ = { 0,4.5f,0 };
	worldTransforms_[kSpine].parent_ = &worldTransforms_[kRoot];
	worldTransforms_[kSpine].Initialize();

	worldTransforms_[kChest].translation_ = { 0,0,0 };
	worldTransforms_[kChest].parent_ = &worldTransforms_[kSpine];
	worldTransforms_[kChest].Initialize();

	worldTransforms_[kHead].translation_ = { 0,4.5f,0 };
	worldTransforms_[kHead].parent_ = &worldTransforms_[kChest];
	worldTransforms_[kHead].Initialize();


	worldTransforms_[kArmL].translation_ = { -4.5f,0,0 };
	worldTransforms_[kArmL].parent_ = &worldTransforms_[kChest];
	worldTransforms_[kArmL].Initialize();

	worldTransforms_[kArmR].translation_ = { 4.5f,0,0 };
	worldTransforms_[kArmR].parent_ = &worldTransforms_[kChest];
	worldTransforms_[kArmR].Initialize();

	worldTransforms_[kHip].translation_ = { 0,-4.5f,0 };
	worldTransforms_[kHip].parent_ = &worldTransforms_[kSpine];
	worldTransforms_[kHip].Initialize();
	
	worldTransforms_[kLegL].translation_ = { -4.5f,-4.5f,0 };
	worldTransforms_[kLegL].parent_ = &worldTransforms_[kHip];
	worldTransforms_[kLegL].Initialize();

	worldTransforms_[kLegR].translation_ = { 4.5f,-4.5f,0 };
	worldTransforms_[kLegR].parent_ = &worldTransforms_[kHip];
	worldTransforms_[kLegR].Initialize();

	
	
	viewProjection_.eye = { 0,50.0f,-50.0f };
	viewProjection_.target.z = viewProjection_.eye.z + 50;
	viewProjection_.target.x = viewProjection_.eye.x;

	viewProjection_.Initialize();
	

	mode = 0;


	
}

void GameScene::Update() 
{
	Vector3 move = Vector3();


	Vector3 rota = Vector3();

	Vector3 flont = Vector3(0, 0, 1);

	Vector3 result;

	const float kEyeSpeed = 0.2f;

	const float angle = 0.02f;
	Vector3 movepow = Vector3(kEyeSpeed, kEyeSpeed, kEyeSpeed);

	

	Vector3 OE = Vector3(viewProjection_.eye.x, 0, viewProjection_.eye.z);

	Vector3 OE_T = viewProjection_.target - OE;

	Vector3 OE_E = viewProjection_.eye - OE;

	OE_E = OE_E.cross(OE_T);

	OE_E.normalize();
	OE_T.normalize();

	if (input_->PushKey(DIK_Z))
	{
		viewAngle += ConvertRad(1.0f);

		viewProjection_.eye.x = -50 * sinf(viewAngle);
		viewProjection_.eye.z = -50 * cosf(viewAngle);

	}
	if (input_->PushKey(DIK_C))
	{
		viewAngle -= ConvertRad(1.0f);
		viewProjection_.eye.x = -50* sinf(viewAngle);
		viewProjection_.eye.z = -50 * cosf(viewAngle);
	}
	if (input_->PushKey(DIK_LEFT))
	{
		viewProjection_.eye.x -= kEyeSpeed;
		viewProjection_.target.x -= kEyeSpeed;
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		viewProjection_.eye.x += kEyeSpeed;
		viewProjection_.target.x += kEyeSpeed;
		
	}
	if (input_->PushKey(DIK_UP))
	{
		viewProjection_.eye.z += kEyeSpeed;
		viewProjection_.target.z += kEyeSpeed;
	}
	if (input_->PushKey(DIK_DOWN))
	{
		viewProjection_.eye.z -= kEyeSpeed;
		viewProjection_.target.z -= kEyeSpeed;
	}
	
	//const float kUpRotSpeed = 0.05f;

	//if (input_->PushKey(DIK_SPACE))
	//{
	//	viewAngle += kUpRotSpeed;

	//	viewAngle = fmodf(viewAngle, XM_PI * 2.0f);
	//}

	if (input_->TriggerKey(DIK_Q))
	{
		switch (mode)
		{
		case camera_wasd:
			mode = bio_wasd;
			break;
		case bio_wasd:
			mode = camera_wasd;
			break;
		}
	}


	if (input_->PushKey(DIK_W))
	{
		if (bio_wasd==mode) { move += movepow; }
		if (camera_wasd == mode) { worldTransforms_[kRoot].translation_ += OE_T * movepow; }
	}
	if (input_->PushKey(DIK_S))
	{
		if (bio_wasd == mode) { move -= movepow; }
		if (camera_wasd == mode) { worldTransforms_[kRoot].translation_ -= OE_T * movepow; }
	}
	if (input_->PushKey(DIK_D))
	{
		if (bio_wasd == mode) { rota.y += angle; }
		if (camera_wasd == mode) { worldTransforms_[kRoot].translation_ += OE_E * movepow; }
	}
	if (input_->PushKey(DIK_A))
	{
		if (bio_wasd == mode) { rota.y -= angle; }
		if (camera_wasd == mode) { worldTransforms_[kRoot].translation_ -= OE_E * movepow; }
	}
	worldTransforms_[kRoot].rotation_ += rota;

	result.x =
		cos(worldTransforms_[kRoot].rotation_.y) * flont.x +
		sin(worldTransforms_[kRoot].rotation_.y) * flont.z;
	result.z =
		-sin(worldTransforms_[kRoot].rotation_.y) * flont.x +
		cos(worldTransforms_[kRoot].rotation_.y) * flont.z;

	worldTransforms_[kRoot].translation_ +=result*move;
	


	for (int i = 0; i < 9; i++)
	{
		worldTransforms_[i].UpdateMatrix();
		worldTransforms_[i].TransferMatrix();
	}

	
	//viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0 };
	//viewProjection_.target += move;

	/*if (input_->PushKey(DIK_UP))
	{
		viewProjection_.nearZ += 1.0f;
	}
	if (input_->PushKey(DIK_DOWN))
	{
		viewProjection_.nearZ -= 1.0f;
	}*/



	

	viewProjection_.UpdateMatrix();
	
	debugCamera_->Update();
	debugText_->SetPos(150, 110);
	debugText_->Printf("%f", viewProjection_.fovAngleY);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//3Dモデル描画
	

	for (int i = 1; i < 9; i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}
	
	for (size_t i = 0; i < _countof(worldTransform_2); i++) {
		model_->Draw(worldTransform_2[i], viewProjection_, textureHandle_);
	}
	//PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(100.0f, 0, 0), Vector3(-100.0f, 0, 0), Vector4(1, 0, 0, 1));
	//PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0, 100.0f, 0), Vector3(0, -100.0f, 0), Vector4(0, 1, 0, 1));
	//PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0, 0, 100.0f), Vector3(0, 0, -100.0f), Vector4(0, 0, 1, 1));

	
	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

float ConvertRad(float dosuu)
{
	return dosuu * (XM_PI / 180.0f);
}

