#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Sprite.h"

class GameScene;

/// <summary>
/// 天球
/// </summary>
class Skydome {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }


	private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Sprite* sprite_ = nullptr;

	GameScene* gameScene_ = nullptr;
};