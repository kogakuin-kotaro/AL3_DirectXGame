#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>

/// <summary>
/// 自キャラ
/// </summary>
class Player {
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Player();

		void Initialize(Model* model, uint32_t textureHandle);
	    void Update();
	    void Draw(ViewProjection& viewProjection);

		void Rotate();

		void Attack();

		Vector3 GetWorldPosition();

		//衝突を検出したら呼び出されるコールバック関数
	    void OnCollision();

		//弾リストを取得
		const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

		//半径
	    const float Range = 1.0f;


	private:
	    WorldTransform worldTransform_;
	    Model* model_ = nullptr;
	    uint32_t textureHandle_ = 0u;

		Input* input_ = nullptr;

	    std::list<PlayerBullet*> bullets_;
};