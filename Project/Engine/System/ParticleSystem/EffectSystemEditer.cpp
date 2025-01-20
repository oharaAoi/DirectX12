#include "EffectSystemEditer.h"
#include "Engine/Utilities/DrawUtils.h"
#include "Engine/System/ParticleSystem/EffectPersistence.h"
#include "Engine/System/ParticleSystem/Emitter/SphereEmitter.h"
#ifdef _DEBUG
#include "Engine/System/Manager/ImGuiManager.h"

EffectSystemEditer::EffectSystemEditer(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device) {
	Init(renderTarget, descriptorHeaps, dxCommands, device);
}

EffectSystemEditer::~EffectSystemEditer() {}

void EffectSystemEditer::Finalize() {
	depthStencilResource_.Reset();
	gpuParticles_->Finalize();
	gpuEmitterList_.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Init(RenderTarget* renderTarget, DescriptorHeap* descriptorHeaps, DirectXCommands* dxCommands, ID3D12Device* device) {

	renderTarget_ = renderTarget;
	descriptorHeaps_ = descriptorHeaps;
	dxCommands_ = dxCommands;

	// -------------------------------------------------
	// ↓ 深度バッファの作成
	// -------------------------------------------------
	depthStencilResource_ = CreateDepthStencilTextureResource(device, kClientWidth_, kClientHeight_);
	// DSVの生成
	D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	device->CreateDepthStencilView(depthStencilResource_.Get(), &desc, descriptorHeaps_->GetDescriptorHandle(TYPE_DSV).handleCPU);

	emitterFiles_ = EffectPersistence::GetInstance()->GetEmitterNames();
	effectFiles_ = EffectPersistence::GetInstance()->GetEffectNames();

	// -------------------------------------------------
	// ↓ カメラの初期化
	// -------------------------------------------------
	effectSystemCamera_ = std::make_unique<EffectSystemCamera>();
	effectSystemCamera_->Init();

	particleField_ = std::make_unique<ParticleField>();
	particleField_->Init();

	gpuParticles_ = std::make_unique<GpuParticles>();
	gpuParticles_->Init(1024);

	auto& newEmitter = gpuEmitterList_.emplace_back(std::make_unique<SphereEmitter>());
	newEmitter->Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Update() {
	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();

	// カメラの更新
	effectSystemCamera_->Update();

	gpuParticles_->SetViewProjection(effectSystemCamera_->GetViewMatrix() * effectSystemCamera_->GetProjectionMatrix());

	Engine::SetCsPipeline(CsPipelineType::EmitGpuParticle);
	for (std::list<std::unique_ptr<GpuEmitter>>::iterator it = gpuEmitterList_.begin(); it != gpuEmitterList_.end();) {
		(*it)->Update();

		gpuParticles_->EmitBindCmdList(commandList, 0);
		(*it)->BindCmdList(commandList, 2);
		commandList->Dispatch(1, 1, 1);

		++it;
	}


	particleField_->Update();

	if (effectSystemCamera_->GetIsFocused()) {
		isFocused_ = true;
	} else {
		isFocused_ = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Draw() const {
	ImGui::Begin("EffectSystem", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus);

	// Grid線描画
	DrawGrid(effectSystemCamera_->GetViewMatrix(), effectSystemCamera_->GetProjectionMatrix());

	// 実際にEffectを描画する
	Engine::SetPipeline(PipelineType::AddPipeline);
	gpuParticles_->Draw(Engine::GetCommandList());

	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	Render::PrimitiveDrawCall();

	// 最後にImGui上でEffectを描画する
	renderTarget_->TransitionResource(dxCommands_->GetCommandList(), EffectSystem_RenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	ImTextureID textureID2 = reinterpret_cast<ImTextureID>(static_cast<uint64_t>(renderTarget_->GetRenderTargetSRVHandle(RenderTargetType::EffectSystem_RenderTarget).handleGPU.ptr));
	ImGui::SetCursorPos(ImVec2(20, 30)); // 描画位置を設定
	ImGui::Image((void*)textureID2, ImVec2(640.0f, 360.0f)); // サイズは適宜調整

	// windowの判定
	if (ImGui::IsWindowFocused()) {
		effectSystemCamera_->SetIsFocused(true);
	} else {
		effectSystemCamera_->SetIsFocused(false);
	}

	ImGui::End();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　開始
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Begin() {
	// ここでゲーム描画のRenderTargetからEffect用のRenderTargetに変更する
	ID3D12GraphicsCommandList* commandList = dxCommands_->GetCommandList();
	// dsvのポインターを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descriptorHeaps_->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
	dsvHandle.ptr += size_t(descriptorHeaps_->GetDescriptorSize()->GetDSV());
	// RenderTargetを指定する
	renderTarget_->SetRenderTarget(commandList, RenderTargetType::EffectSystem_RenderTarget);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	// RenderTargetをクリアする
	commandList->ClearRenderTargetView(renderTarget_->GetRenderTargetRTVHandle(RenderTargetType::EffectSystem_RenderTarget).handleCPU, clearColor, 0, nullptr);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　終了
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::End() {
	renderTarget_->TransitionResource(dxCommands_->GetCommandList(), EffectSystem_RenderTarget, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Effectの作成をする
//////////////////////////////////////////////////////////////////////////////////////////////////

//void EffectSystemEditer::CreateEffect(const std::string& newName) {
//	auto& newEffect = effectList_.emplace_back(std::make_unique<GpuEffect>());
//	newEffect->Init(static_cast<EmitterShape>(createShape_));
//	newEffect->SetEffectName(newName);
//}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Emitterを取り出す
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Import() {
	//EffectPersistence* persistence = EffectPersistence::GetInstance();
	//uint32_t shape = persistence->GetValue<uint32_t>(importFileName_, "shape");

	/*auto& newEffect = effectList_.emplace_back(std::make_unique<GpuEffect>());
	newEffect->Init(static_cast<EmitterShape>(shape));
	newEffect->SetEmitter(importFileName_);*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::Debug_Gui() {
	EditEmitter();

	EditEffect();

	//if (ImGui::BeginTabBar("EmitterTabs")) {
	//	// リストを反復してタブを作成
	//	for (auto it = effectList_.begin(); it != effectList_.end(); ++it) {
	//		// タブのラベルを生成（インデックスではなくポインタアドレスを利用）
	//		std::string tabLabel = (*it)->GetEmitterLabel() + std::to_string(std::distance(effectList_.begin(), it));

	//		if (ImGui::BeginTabItem(tabLabel.c_str())) {
	//			(*it)->Debug_Gui();

	//			ImGui::EndTabItem();
	//		}
	//	}
	//	ImGui::EndTabBar();
	//}

	ImGui::Begin("Emitter List");
	//static int selectedEffectIndex = -1; // -1 means no selection
	//int index = 0;
	//for (auto it = effectList_.begin(); it != effectList_.end(); ++it, ++index) {
	//	std::string name = (*it)->GetEmitterLabel();
	//	std::string label = "Effect_" + name + std::to_string(index);

	//	if (ImGui::Selectable(label.c_str(), selectedEffectIndex == index)) {
	//		selectedEffectIndex = index; // Update the selected index
	//	}
	//}

	//if (selectedEffectIndex >= 0 && selectedEffectIndex < effectList_.size()) {
	//	auto it = effectList_.begin();
	//	std::advance(it, selectedEffectIndex); // Move iterator to the selected index

	//	ImGui::Begin("EmitterSetting");
	//	(*it)->Debug_Gui();

	//	if (ImGui::Button("Delete")) {
	//		(*it)->SetIsAlive(false);
	//	}
	//	ImGui::End();
	//}
	ImGui::End();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Emitterを編集する
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::EditEmitter() {
	ImGui::Begin("Create Emitter");
	ImGui::RadioButton("Sphere", &createShape_, (int)EmitterShape::Sphere);
	ImGui::SameLine();
	ImGui::RadioButton("Cone", &createShape_, (int)EmitterShape::Cone);
	ImGui::SameLine();
	ImGui::RadioButton("Box", &createShape_, (int)EmitterShape::Box);

	/*if (ImGui::Button("CreateEmitter")) {
		uint32_t effectNum = static_cast<uint32_t>(effectList_.size()) + 1;
		std::string defalutName = "effect" + std::to_string(effectNum);
		CreateEffect(defalutName);
	}*/
	ImGui::Separator();
	if (ImGui::Button("Inport")) {
		Import();
	}
	ImGui::SameLine();
	if (!emitterFiles_.empty()) {
		importFileName_ = emitterFiles_[importIndex_];
	}
	if (ImGui::BeginCombo("##InportFileName", &importFileName_[0], ImGuiComboFlags_HeightLargest)) {
		for (uint32_t i = 0; i < emitterFiles_.size(); i++) {
			const bool isSelected = (importIndex_ == i);
			if (ImGui::Selectable(emitterFiles_[i].c_str(), isSelected)) {
				importIndex_ = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::End();

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Effectを選択する
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::EditEffect() {
	ImGui::Begin("Create Effect");

	if (ImGui::Button("hotReload")) {
		emitterFiles_.clear();
		effectFiles_.clear();
		emitterFiles_ = EffectPersistence::GetInstance()->GetEmitterNames();
		effectFiles_ = EffectPersistence::GetInstance()->GetEffectNames();
		EffectPersistence::GetInstance()->LoadAllFile();
	}

	if (ImGui::BeginCombo("##InportFileName", &importFileName_[0], ImGuiComboFlags_HeightLargest)) {
		for (uint32_t i = 0; i < emitterFiles_.size(); i++) {
			const bool isSelected = (importIndex_ == i);
			if (ImGui::Selectable(emitterFiles_[i].c_str(), isSelected)) {
				importIndex_ = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Add")) {
		effectEmitterNames_.push_back(importFileName_);
	}

	ImGui::BulletText("Emitters");
	static int selectedIndex = -1; // 選択された項目のインデックス
	std::unordered_map<std::string, int> nameCount; // 各名前の出現回数を記録

	for (int i = 0; i < effectEmitterNames_.size(); ++i) {
		std::string displayName = effectEmitterNames_[i];

		// 重複がある場合は末尾に番号を追加
		if (nameCount[displayName] > 0) {
			displayName += " (" + std::to_string(nameCount[displayName]) + ")";
		}

		// 出現回数を更新
		nameCount[effectEmitterNames_[i]]++;

		// ImGui で選択可能項目を表示
		if (ImGui::Selectable(displayName.c_str(), selectedIndex == i)) {
			selectedIndex = i; // 選択を更新
		}
	}

	if (selectedIndex != -1) {
		if (ImGui::Button("Delete")) {
			effectEmitterNames_.erase(effectEmitterNames_.begin() + selectedIndex);
		}
	}

	strncpy_s(inputNewEffectNameBuffer_, newEffectName_.c_str(), sizeof(inputNewEffectNameBuffer_));
	inputNewEffectNameBuffer_[sizeof(inputNewEffectNameBuffer_) - 1] = '\0'; // null終端を確実に設定
	if (ImGui::InputText("##effectName", inputNewEffectNameBuffer_, sizeof(inputNewEffectNameBuffer_))) {
		newEffectName_ = inputNewEffectNameBuffer_; // 入力が変更された場合に更新
	}

	if (ImGui::Button("Save")) {
		SaveEffect();
	}

	LoadEffect();

	ImGui::End();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Effectを保存する
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemEditer::SaveEffect() {
	EffectPersistence* persistence = EffectPersistence::GetInstance();
	uint32_t emitterNum = 1;
	for (const auto& emitterName : effectEmitterNames_) {
		std::string key = "emitter" + std::to_string(emitterNum);
		persistence->AddItem(newEffectName_, key.c_str(), emitterName);
		emitterNum++;
	}
	persistence->Save(true, newEffectName_);

}
void EffectSystemEditer::LoadEffect() {
	ImGui::BulletText("Load");

	if (!EffectPersistence::GetInstance()->GetEffectNames().empty()) {
		importEffectName_ = EffectPersistence::GetInstance()->GetEffectNames()[importEffectIndex_];
	}
	if (ImGui::BeginCombo("##InportEffectName", &importEffectName_[0], ImGuiComboFlags_HeightLargest)) {
		for (uint32_t i = 0; i < effectFiles_.size(); i++) {
			const bool isSelected = (importEffectIndex_ == i);
			if (ImGui::Selectable(effectFiles_[i].c_str(), isSelected)) {
				importEffectIndex_ = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Load")) {
		for (uint32_t oi = 0; oi < EffectPersistence::GetInstance()->GetItemsSize(importEffectName_); ++oi) {
			std::string key = "emitter" + std::to_string(oi + 1);
			importFileName_ = EffectPersistence::GetInstance()->GetValue<std::string>(importEffectName_, key.c_str());
			Import();
		}
	}
}
#endif // _DEBUG