#include "TestObject.h"
#include "Engine/Utilities/AdjustmentItem.h"

TestObject::TestObject() {
}

TestObject::~TestObject() {
}

void TestObject::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TestObject::Init() {
	BaseGameObject::Init();
	SetObject("skin.obj");

	SetMeshCollider("testObj");

	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other);});
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other);});
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other);});

	transitionAnimationTime_ = 0.0f;
	transitionAnimationTimeLimit_ = 1.0f;

	animationTime_ = 0.0f;

	test_.FromJson(AdjustmentItem::GetData("Player", "testParame"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TestObject::Update() {
	/*if (animetor_ != nullptr) {
		if (!animetor_->GetIsAnimationChange()) {
			animationTime_ += GameTimer::DeltaTime();
		}
		animetor_->UpdateScript(animationTime_, transitionAnimationTimeLimit_);
		animationTime_ = std::fmod(animationTime_, animetor_->GetAnimationDuration());
	}*/

	BaseGameObject::Update();
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////
void TestObject::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TestObject::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "testObj") {
		isCollision_ = true;
	}
}

void TestObject::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
	
}

void TestObject::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "testObj") {
		isCollision_ = true;
	}
}


#ifdef _DEBUG
void TestObject::Debug_Gui() {
	BaseGameObject::Debug_Gui();

	if (animetor_ != nullptr) {
		ImGui::SliderFloat("animationTime", &animationTime_, 0.0, animetor_->GetAnimationDuration());
		ImGui::SliderFloat("transitionAnimationTime", &transitionAnimationTime_, 0.0, transitionAnimationTimeLimit_);

		ImGui::SliderFloat("transitionAnimationTimeLimit", &transitionAnimationTimeLimit_, 0.0, 10.0f);
	}

	if (ImGui::Button("lerp1")) {
		isLerp_ = true;
		animetor_->SetTransitionAnimation("walkRun", "jump");
	}

	if (ImGui::Button("lerp2")) {
		isLerp_ = true;
		animetor_->SetTransitionAnimation("jump", "walkRun");
	}
}
#endif