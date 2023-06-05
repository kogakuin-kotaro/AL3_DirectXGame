#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Enemy {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_;

};