#pragma once
#include "Model.h"
#include "WorldTransform.h"


/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet{
public:

	void Initialize(Model* model, Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;

	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
};