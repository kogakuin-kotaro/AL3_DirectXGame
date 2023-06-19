#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

/// <summary>
/// レールカメラ
/// </summary>

class RailCamera {
public:
	void Initialize(Vector3 position,Vector3 rotation);
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	WorldTransform& GetWorldTransform() { return worldTransform_; };



		private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	ViewProjection viewProjection_;
};