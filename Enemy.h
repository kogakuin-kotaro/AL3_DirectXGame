#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Enemy {
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);

	void ApproachUpdate();
	void LeaveUpdate();

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_ = {0, 0, -0.2f};

	enum class Phase {
		Approach,
		Leave,
	};

	Phase phase_ = Phase::Approach;

};