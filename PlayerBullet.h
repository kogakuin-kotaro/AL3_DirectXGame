#pragma once
#include "Model.h"
#include "WorldTransform.h"


/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet{
public:

	void Initialize(Model* model, Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);


private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
};