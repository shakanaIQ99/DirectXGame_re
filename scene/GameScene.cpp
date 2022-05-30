#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include<random>

const float XM_PI = 3.14592654f;

float ConvertRad(float dosuu);

Matrix4 ChengeRot(Vector3 Rot);

Matrix4 ChengeScr(Vector3 Scr);

Matrix4 ChengePos(Vector3 Pos);

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
	textureHandle_ = TextureManager::Load("mario.jpg");
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
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		worldTransform.Initialize();

		worldTransform.scale_ = { 1,1,1 };

		worldTransform.rotation_ = { RadDist(engine),RadDist(engine),RadDist(engine) };

		worldTransform.translation_ = { TraDist(engine),TraDist(engine),TraDist(engine) };



		worldTransform.matWorld_.IdentityMatrix4();

		worldTransform.matWorld_ *= ChengeScr(worldTransform.scale_);
		worldTransform.matWorld_ *= ChengeRot(worldTransform.rotation_);
		worldTransform.matWorld_ *= ChengePos(worldTransform.translation_);

		worldTransform.TransferMatrix();



	}
	//viewProjection_.eye = { 0,0,-50 };
	viewProjection_.target = { 10,0,0 };
	viewProjection_.up = { cosf(ConvertRad(45.0f)),sinf(ConvertRad(45.0f)),0 };
	
	
	viewProjection_.Initialize();
	

	
	
}

void GameScene::Update() 
{
	Vector3 move = Vector3();

	const float kEyeSpeed = 0.2f;

	const float kUpRotSpeed = 0.05f;

	if (input_->PushKey(DIK_SPACE))
	{
		viewAngle += kUpRotSpeed;

		viewAngle = fmodf(viewAngle, XM_PI * 2.0f);
	}

	/*if (input_->PushKey(DIK_W))
	{
		move.z += kEyeSpeed;
	}
	if (input_->PushKey(DIK_S))
	{
		move.z -= kEyeSpeed;
	}*/
	if (input_->PushKey(DIK_LEFT))
	{
		move.x -= kEyeSpeed;
	}
	if (input_->PushKey(DIK_RIGHT))
	{
		move.x += kEyeSpeed;
	}

	viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0 };
	viewProjection_.target += move;
	viewProjection_.UpdateMatrix();
	
	debugCamera_->Update();
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
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
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

Matrix4 ChengeRot(Vector3 Rot)
{
	

	Matrix4 matRot;
	Matrix4 matRotX, matRotY, matRotZ;

	matRot.IdentityMatrix4();
	matRotX.IdentityMatrix4();
	matRotY.IdentityMatrix4();
	matRotZ.IdentityMatrix4();

	matRotX.m[1][1] = cos(Rot.x);
	matRotX.m[1][2] = sin(Rot.x);
	matRotX.m[2][1] = -sin(Rot.x);
	matRotX.m[2][2] = cos(Rot.x);

	matRotY.m[0][0] = cos(Rot.y);
	matRotY.m[0][2] = -sin(Rot.y);
	matRotY.m[2][0] = sin(Rot.y);
	matRotY.m[2][2] = cos(Rot.y);

	matRotZ.m[0][0] = cos(Rot.z);
	matRotZ.m[0][1] = sin(Rot.z);
	matRotZ.m[1][0] = -sin(Rot.z);
	matRotZ.m[1][1] = cos(Rot.z);

	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	return matRot;
}

Matrix4 ChengeScr(Vector3 Scr)
{
	Matrix4 matscale;
	matscale.IdentityMatrix4();

	matscale.m[0][0] = Scr.x;
	matscale.m[1][1] = Scr.y;
	matscale.m[2][2] = Scr.z;

	return matscale;
}

Matrix4 ChengePos(Vector3 Pos)
{
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	
	matTrans.m[3][0] += Pos.x;
	matTrans.m[3][1] += Pos.y;
	matTrans.m[3][2] += Pos.z;

	return matTrans;
}
