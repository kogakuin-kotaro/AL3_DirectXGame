#include "RailCamera.h"
#include "MathUtility.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Vector3 position, Vector3 rotation) {

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	viewProjection_.Initialize();

}

void RailCamera::Update() {

	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	const float kRotSpead = 0.00f;
	worldTransform_.rotation_.y -= kRotSpead;

	worldTransform_.UpdateMatrix(); 

	viewProjection_.matView = Inverce(worldTransform_.matWorld_);

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Transform", &worldTransform_.translation_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("Rotation", &worldTransform_.rotation_.x, -360.0f, 360.0f);
	ImGui::End();

}

