#include "GpuEmitter.h"
#include "Engine/Engine.h"

void GpuEmitter::Init() {
	// Resourceの作成
	commonBuffer_ = CreateUAVResource(Engine::GetDevice(), sizeof(CommonEmitter));

	commonBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(CommonEmitter));
	commonBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&commonEmitter_));

	perFrameBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(PerFrame));
	perFrameBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&perFrame_));

	rotate_ = Quaternion();

	commonEmitter_->rotate = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	commonEmitter_->translate = Vector3::ZERO();
	commonEmitter_->frequency = 0.5f;
	commonEmitter_->frequencyTime = 0.0f;
	commonEmitter_->count = 5;
	commonEmitter_->emit = 0;
	commonEmitter_->speed = 1.0f;
	commonEmitter_->color = Vector4(1, 1, 1, 1);

	emitterParameter_.velocity = Vector3::ZERO();
	emitterParameter_.speed = 1.0f;
	emitterParameter_.lifeTime = 10.0f;

	lifeTime_ = emitterParameter_.lifeTime;
	isMove_ = false;
	isDead_ = false;
}


void GpuEmitter::Update() {

	Move();

	perFrame_->deltaTime = GameTimer::DeltaTime();
	perFrame_->time = GameTimer::TotalTime();

	commonEmitter_->frequencyTime += GameTimer::DeltaTime();
	// 射出時間を超えたら射出許可
	if (commonEmitter_->frequencyTime >= commonEmitter_->frequency) {
		commonEmitter_->frequencyTime -= commonEmitter_->frequency;
		commonEmitter_->emit = 1;
	} else {
		commonEmitter_->emit = 0;
	}

	// 回転の更新
	rotate_ = deltaRotate_.Normalize() * rotate_;
	commonEmitter_->rotate = rotate_.Normalize();
}

void GpuEmitter::BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) {
	commandList->SetComputeRootConstantBufferView(rootParameterIndex, perFrameBuffer_->GetGPUVirtualAddress());
	commandList->SetComputeRootConstantBufferView(rootParameterIndex + 1, commonBuffer_->GetGPUVirtualAddress());
}

void GpuEmitter::SetEmitter(const std::string& name) {
	label_ = name;
	Load();
}

void GpuEmitter::Move() {
	if (isMove_) {
		lifeTime_ -= GameTimer::DeltaTime();
		commonEmitter_->translate += (emitterParameter_.velocity.Normalize() * emitterParameter_.speed) * GameTimer::DeltaTime();

		if (lifeTime_ <= 0.0f) {
			isDead_ = true;
		}
	}
}

void GpuEmitter::Save() {
	EffectPersistence* persistence = EffectPersistence::GetInstance();
	persistence->CreateGroup(label_);
	// commonのSave
	persistence->AddItem(label_, "translate", commonEmitter_->translate);
	persistence->AddItem(label_, "rotate", commonEmitter_->rotate);
	persistence->AddItem(label_, "shape", commonEmitter_->shape);
	persistence->AddItem(label_, "count", commonEmitter_->count);
	persistence->AddItem(label_, "frequency", commonEmitter_->frequency);
	persistence->AddItem(label_, "frequencyTime", commonEmitter_->frequencyTime);
	persistence->AddItem(label_, "color", commonEmitter_->color);
	persistence->AddItem(label_, "speed", commonEmitter_->speed);
	// parameter
	persistence->AddItem(label_, "velocity", emitterParameter_.velocity);
	persistence->AddItem(label_, "speed", emitterParameter_.speed);
	persistence->AddItem(label_, "lifeTime", emitterParameter_.lifeTime);
}

void GpuEmitter::Load() {
	EffectPersistence* persistence = EffectPersistence::GetInstance();
	// commonのLoad
	commonEmitter_->translate = persistence->GetValue<Vector3>(label_, "translate");
	commonEmitter_->rotate = persistence->GetValue<Vector4>(label_, "rotate");
	commonEmitter_->shape = persistence->GetValue<uint32_t>(label_, "shape");
	commonEmitter_->count = persistence->GetValue<uint32_t>(label_, "count");
	commonEmitter_->frequency = persistence->GetValue<float>(label_, "frequency");
	commonEmitter_->frequencyTime = persistence->GetValue<float>(label_, "frequencyTime");
	commonEmitter_->color = persistence->GetValue<Vector4>(label_, "color");
	commonEmitter_->speed = persistence->GetValue<float>(label_, "speed");
	// commonのSave
	emitterParameter_.velocity = persistence->GetValue<Vector3>(label_, "velocity");
	emitterParameter_.speed = persistence->GetValue<float>(label_, "speed");
	emitterParameter_.lifeTime = persistence->GetValue<float>(label_, "lifeTime");
}

#ifdef _DEBUG
#include "Engine/System/Manager/ImGuiManager.h"
void GpuEmitter::Debug_Gui() {
	deltaRotate_ = Quaternion();

	// 共通部
	if (ImGui::TreeNode("Common")) {
		ImGui::DragFloat3("rotate", &rotate_.x, 0.01f);
		ImGui::DragFloat3("deltaRotate", &deltaRotate_.x, 0.01f);
		ImGui::DragFloat3("translate", &commonEmitter_->translate.x, 0.1f);
		ImGui::DragFloat("frequency", &commonEmitter_->frequency, 0.1f);
		ImGui::DragFloat("frequencyTime", &commonEmitter_->frequencyTime, 0.1f);
		ImGui::DragScalar("count", ImGuiDataType_U32, &commonEmitter_->count);
		ImGui::SliderInt("emit", &commonEmitter_->emit, 0, 1);
		ImGui::DragFloat("speed", &commonEmitter_->speed, 0.1f);
		ImGui::ColorEdit4("color", &commonEmitter_->color.x);

		ImGui::TreePop();
	}

	// Emitter
	if (ImGui::TreeNode("Parameter")) {
		if (ImGui::Button("play")) {
			isMove_ = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("stop")) {
			isMove_ = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("reset")) {
			commonEmitter_->translate = Vector3::ZERO();
			lifeTime_ = emitterParameter_.lifeTime;
			isDead_ = true;
		}

		ImGui::DragFloat3("velocity", &emitterParameter_.velocity.x, 0.01f);
		ImGui::DragFloat("lifeTime", &emitterParameter_.lifeTime, 0.1f);
		ImGui::DragFloat("speed", &emitterParameter_.speed, 0.1f);

		ImGui::TreePop();
	}
}
#endif