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

		worldTransform.UpdateMatrix();
		worldTransform.TransferMatrix();
	}*/

	for (int i = 0; i < num; i++)
	{
		worldTransforms_[i].Initialize();
		worldTransforms_[i].scale_ = { 1,1,1 };
		worldTransforms_[i].rotation_ = { 0,0,0 };
		if (num / 2 > i)
		{
			worldTransforms_[i].translation_ = { 2.0f * (float)i-20.0f,4,-40 };
		}
		if (num / 2 <= i)
		{
			worldTransforms_[i].translation_ = { 2.0f * ((float)i-(num/2)) - 20.0f,-4,-40 };
		}
		worldTransforms_[i].UpdateMatrix();
		worldTransforms_[i].TransferMatrix();

	}
	

	viewProjection_.Initialize();
	

	
	
}

void GameScene::Update() 
{
	
	

	viewProjection_.UpdateMatrix();
	
	
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

