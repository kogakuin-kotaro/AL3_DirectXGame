#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player {
	public:

		void Initialize(Model* model, uint32_t textureHandle);
	    void Update();
	    void Draw(ViewProjection& viewProjection);

		void Rotate();

		void Attack();

	private:
	    WorldTransform worldTransform_;
	    Model* model_ = nullptr;
	    uint32_t textureHandle_ = 0u;

		Input* input_ = nullptr;

	    PlayerBullet* bullet_ = nullptr;
};