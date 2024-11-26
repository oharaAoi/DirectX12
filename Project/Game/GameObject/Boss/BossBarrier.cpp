#include "BossBarrier.h"

BossBarrier::BossBarrier() {}
BossBarrier::~BossBarrier() {}


void BossBarrier::Finalize() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossBarrier::Init() {
	BaseGameObject::Init();
	SetObject("barrier.obj");
	SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f));

	SetMeshCollider("boss_barrier");
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other); });
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other); });
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other); });

	// バッファの作成
	timeBuffer_ = CreateBufferResource(Engine::GetDevice(), sizeof(TimeCount));
	timeBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&timeData_));

	SetIsLighting(false);

	// -------------------------------------------------
	// ↓ 調整項目系
	// -------------------------------------------------
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "expand_.timeLimit", expand_.timeLimit);
	adjust->AddItem(groupName_, "expand_.easingType", (uint32_t)expand_.easingType);
	adjust->AddItem(groupName_, "break_.timeLimit", break_.timeLimit);
	adjust->AddItem(groupName_, "break_.easingType", (uint32_t)break_.easingType);

	// 調整項目の適応
	AdaptAdjustment();

	expand_.isAct = false;
	expand_.time = 0.0f;
	
	break_.isAct = false;
	break_.time = 0.0f;

	hp_ = Durability_;
	transform_->SetTranslationY(-3.2f);

	enableFunction_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossBarrier::Update() {
	if (!enableFunction_) {
		return;
	}

	timeData_->totalTime += GameTimer::DeltaTime() * 10.0f;
	timeData_->cameraPos = Render::GetEyePos();

	if (expand_.isAct && !break_.isAct) {
		Expand();
	} else if (break_.isAct) {
		Break();
	}

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossBarrier::Draw() const {
	if (!enableFunction_) {
		return;
	}

	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
	Engine::SetPipeline(PipelineType::TriangleSeparation);
	Render::DrawLightGroup(4);
	for (uint32_t oi = 0; oi < model_->GetMeshs().size(); oi++) {
		// meshとmaterial
		model_->GetMeshs()[oi]->Draw(commandList);
		materials[oi]->Draw(commandList);
		// worldTransformとviewTransform
		transform_->Draw(commandList);
		Render::Draw(commandList);
		// Texture
		std::string textureName = materials[oi]->GetUseTexture();
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 3);
		// 必要に応じて追加したいもの
		commandList->SetGraphicsRootConstantBufferView(7, timeBuffer_->GetGPUVirtualAddress());

		commandList->DrawIndexedInstanced(model_->GetMeshs()[oi]->GetIndexNum(), 1, 0, 0, 0);
	}
}

void BossBarrier::AdaptAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	expand_.timeLimit = adjust->GetValue<float>(groupName_, "expand_.timeLimit");
	expand_.easingType = (int)adjust->GetValue<uint32_t>(groupName_, "expand_.easingType");
	break_.timeLimit = adjust->GetValue<float>(groupName_, "break_.timeLimit");
	break_.easingType = (int)adjust->GetValue<uint32_t>(groupName_, "break_.easingType");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　バリアを展開する処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossBarrier::Expand() {
	expand_.time += (GameTimer::DeltaTime());
	float t = expand_.time / expand_.timeLimit;
	t = std::clamp(t, 0.0f, 1.0f);
	timeData_->time = std::lerp(6.0f, 0.0f, CallEasingFunc(expand_.easingType, t));
	isBreak_ = false;

	for (uint32_t oi = 0; oi < materials.size(); oi++) {
		t = std::clamp(t, 0.0f, 1.0f);
		materials[oi]->SetColor(Vector4(1, 1, 1, std::lerp(0.0f, 0.7f, CallEasingFunc(expand_.easingType, t))));
	}

	if (expand_.time >= expand_.timeLimit) {
		expand_.isAct = false;
		hp_ = Durability_;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　バリアを割る処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossBarrier::Break() {
	break_.time += (GameTimer::DeltaTime());
	float t = break_.time / break_.timeLimit;
	t = std::clamp(t, 0.0f, 1.0f);
	timeData_->time = std::lerp(0.0f, 6.0f, CallEasingFunc(break_.easingType, t));
	isBreak_ = true;

	for (uint32_t oi = 0; oi < materials.size(); oi++) {
		t = std::clamp(t, 0.0f, 1.0f);
		materials[oi]->SetColor(Vector4(1, 1, 1, std::lerp(0.7f, 0.0f, CallEasingFunc(break_.easingType, t))));
	}

	if (break_.time >= break_.timeLimit) {
		break_.isAct = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　当たり判定処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void BossBarrier::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "throwMissile") {
		hp_--;
		// hpが0になったら
		if (hp_ == 0) {
			break_.isAct = true;
		}
	}
	if (other.GetTag() == "fallStone") {
		break_.isAct = true;
	}
}

void BossBarrier::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
}

void BossBarrier::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void BossBarrier::Debug_Gui() {
	ImGui::Begin("Boss_Barrier");
	ImGui::SliderFloat("time", &timeData_->time, 0.0f, 6.0f);
	transform_->Debug_Gui();

	if (ImGui::Button("expand")) {
		expand_.isAct = true;
		expand_.time = 0.0f;
	}

	if (ImGui::Button("break")) {
		break_.isAct = true;
		break_.time = 0.0f;
	}

	if (ImGui::CollapsingHeader("Expand")) {
		ShowEasingDebug(expand_.easingType);
	} else if(ImGui::CollapsingHeader("Break")) {
		ShowEasingDebug(break_.easingType);
	}

	ImGui::End();
}
void BossBarrier::Debug_Draw() {
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	meshCollider_->Draw();
}

#endif

const bool BossBarrier::GetIsExpand() const {

	bool is = (expand_.time < 0.1f) && expand_.isAct;

	return is;
}