#include "RailCamera.h"

RailCamera::RailCamera() {
}

RailCamera::~RailCamera() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void RailCamera::Init() {
	BaseCamera::Init();

	cameraObj_ = std::make_unique<BaseGameObject>();
	cameraObj_->Init();
	cameraObj_->SetObject("camera.obj");

	offset_ = { 0.0f, 0.2f, 0.0f };
	eyeIndex_ = 0;
	forwardIndex_ = 0;

	isMove_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void RailCamera::Update() {
	if (!isMove_) {
		BaseCamera::Update();
		return;
	}

	frameCount_ += GameTimer::DeltaTime();

	if (frameCount_ > 0.1f) {
		RailMove();
		frameCount_ = 0.0f;
	}
	cameraObj_->GetTransform()->SetTranslaion(transform_.translate);
	cameraObj_->Update();
	BaseCamera::Update();

#ifdef _DEBUG
	if (Input::IsTriggerKey(DIK_L)) {
		isMove_ = &isMove_;
	}
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void RailCamera::Draw() const {
	cameraObj_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　レールに沿ってカメラが動くように
//////////////////////////////////////////////////////////////////////////////////////////////////

void RailCamera::InitRail() {
	eyeIndex_++;
	if (eyeIndex_ >= static_cast<uint32_t>(controlPoints_.size() - 1)) {
		return;
	}

	forwardIndex_ = eyeIndex_ + 5;
	if (controlPoints_.size() <= forwardIndex_) {
		forwardIndex_ = static_cast<uint32_t>(controlPoints_.size() - 1);
	}

	transform_.translate = controlPoints_[eyeIndex_] + offset_;
	// forwardの座標を求める
	Vector3 forwardPos = controlPoints_[forwardIndex_] + offset_;

	Vector3 diff = Normalize(forwardPos - transform_.translate);
	transform_.rotate.y = localRotate_.y + std::atan2f(diff.x, diff.z);
	float xzLenght = Length({ diff.x, 0, diff.z });
	transform_.rotate.x = localRotate_.x + std::atan2f(-diff.y, xzLenght);
}

void RailCamera::RailMove() {
	eyeIndex_++;
	if (eyeIndex_ >= static_cast<uint32_t>(controlPoints_.size() - 1)) {
		return;
	}

	forwardIndex_ = eyeIndex_ + 5;
	if (controlPoints_.size() <= forwardIndex_) {
		forwardIndex_ = static_cast<uint32_t>(controlPoints_.size() - 1);
	}

	transform_.translate = controlPoints_[eyeIndex_] + offset_;
	// forwardの座標を求める
	Vector3 forwardPos = controlPoints_[forwardIndex_] + offset_;

	Vector3 diff = Normalize(forwardPos - transform_.translate);
	transform_.rotate.y = localRotate_.y + std::atan2f(diff.x, diff.z);
	float xzLenght = Length({ diff.x, 0, diff.z });
	transform_.rotate.x = localRotate_.x + std::atan2f(-diff.y, xzLenght);

	//transform_.rotate.z = controlRotateZ_[eyeIndex_];
}

void RailCamera::SetControlPoints(const std::vector<Vector3>& points) {
	controlPoints_ = points;
}

void RailCamera::SetControlRotateZ(const std::vector<float>& points) {
	controlRotateZ_ = points;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　保存
//////////////////////////////////////////////////////////////////////////////////////////////////



#ifdef _DEBUG
void RailCamera::Debug_Gui() {
	ImGui::Begin("RailCamera");
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	if (ImGui::Button("Reset")) {
		eyeIndex_ = 0;
	}
	ImGui::Checkbox("isMove", &isMove_);

	ImGui::Text("eyeIndex : %d", eyeIndex_);
	ImGui::End();
}
#endif