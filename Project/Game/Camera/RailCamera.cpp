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
	if (Input::IsTriggerKey(DIK_L)) {
		isMove_ = !isMove_;
	}

	if (!isMove_) {
		const size_t segmentCount = controlPoints_.size();
		Vector3 pos = CatmullRomPosition(controlPoints_, t);
		Vector3 pos2 = CatmullRomPosition(controlPoints_, t2);

		if (t >= 1.0f) {
			t = 1.0f;
		}

		if (t2 >= 1.0f) {
			t2 = 1.0f;
		}

		transform_.translate = pos + offset_;
		// forwardの座標を求める
		Vector3 forwardPos = pos2 + offset_;

		Vector3 diff = Normalize(forwardPos - transform_.translate);
		transform_.rotate.y = localRotate_.y + std::atan2f(diff.x, diff.z);
		float xzLenght = Length({ diff.x, 0, diff.z });
		transform_.rotate.x = localRotate_.x + std::atan2f(-diff.y, xzLenght);


		BaseCamera::Update();
		return;
	}

	RailMove();

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

void RailCamera::RailMove() {

	frameCount_ += GameTimer::DeltaTime();
	if (frameCount_ > 0.5f) {
		eyeIndex_++;
		frameCount_ = 0;
	}

	const size_t segmentCount = controlPoints_.size();
	// 点からSpline曲線を引く
	t += (1.0f / static_cast<float>(segmentCount)) * (GameTimer::DeltaTime() * 2.0f);
	Vector3 pos = CatmullRomPosition(controlPoints_, t);

	t2 = t + (((1.0f / static_cast<float>(segmentCount)) * (GameTimer::DeltaTime() * 10.0f)));
	Vector3 pos2 = CatmullRomPosition(controlPoints_, t2);

	if (t >= 1.0f) {
		t = 1.0f;
	}

	if (t2 >= 1.0f) {
		t2 = 1.0f;
	}

	transform_.translate = pos + offset_;
	// forwardの座標を求める
	Vector3 forwardPos = pos2 + offset_;

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
		t = 0;
		t2 = 0;
		eyeIndex_ = 0.0f;
	}
	ImGui::Checkbox("isMove", &isMove_);

	ImGui::Text("eyeIndex : %d", eyeIndex_);

	ImGui::Text("pos.x");
	ImGui::SameLine();
	if (ImGui::ArrowButton("x##Left", ImGuiDir_Left)) {
		eyeIndex_--;
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("x##Right", ImGuiDir_Right)) {
		eyeIndex_++;
	}
	ImGui::End();
}
#endif