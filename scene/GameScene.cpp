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

	//x,y,z方向のスケーリングを設定
	/*for (WorldTransform& worldTransform : worldTransforms_)
	{
		worldTransform.Initialize();

		worldTransform.scale_ = { 1,1,1 };

		worldTransform.rotation_ = { RadDist(engine),RadDist(engine),RadDist(engine) };

		worldTransform.translation_ = { TraDist(engine),TraDist(engine),TraDist(engine) };



		worldTransform.matWorld_.IdentityMatrix4();

		worldTransform.matWorld_ *= ChengeScr(worldTransform.scale_);
		worldTransform.matWorld_ *= ChengeRot(worldTransform.rotation_);
		worldTransform.matWorld_ *= ChengePos(worldTransform.translation_);




	}*/

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

	
	

	

	viewProjection_.Initialize();
	

	
	
}

void GameScene::Update() 
{
	Vector3 move = Vector3();

	const float kEyeSpeed = 0.2f;

	//const float kUpRotSpeed = 0.05f;

	//if (input_->PushKey(DIK_SPACE))
	//{
	//	viewAngle += kUpRotSpeed;

	//	viewAngle = fmodf(viewAngle, XM_PI * 2.0f);
	//}

	///*if (input_->PushKey(DIK_W))
	//{
	//	move.z += kEyeSpeed;
	//}
	//if (input_->PushKey(DIK_S))
	//{
	//	move.z -= kEyeSpeed;
	//}*/
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kEyeSpeed;
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kEyeSpeed;
	}
	if (input_->PushKey(DIK_UP))
	{
		move.y += kEyeSpeed;
	}
	if (input_->PushKey(DIK_DOWN))
	{
		move.y -= kEyeSpeed;
	}
	worldTransforms_[kRoot].translation_ += move;

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
	/*for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}*/

	for (int i = 1; i < 9; i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
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

