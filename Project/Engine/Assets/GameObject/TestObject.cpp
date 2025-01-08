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
	SetObject("cube.obj");
	//SetAnimater("./Engine/Resources/Animation/", "player.gltf", true, true, false);

	SetMeshCollider("testObj");
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other);});
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other);});
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other);});

	transitionAnimationTime_ = 0.0f;
	transitionAnimationTimeLimit_ = 1.0f;

	animationTime_ = 0.0f;

	/*sword_ = std::make_unique<BaseGameObject>();
	sword_->Init();
	sword_->SetObject("sword.obj");
	swordMat_ = animetor_->GetSkeleton()->GetSkeltonSpaceMat("mixamorig:RightHand") * transform_->GetWorldMatrix();
	sword_->GetTransform()->SetScale(Vector3(100, 100, 100));
	sword_->GetTransform()->SetParent(swordMat_);*/

	test_.ToJson("testParame");
	test_.FromJson(AdjustmentItem::GetData("Player", "testParame"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TestObject::Update() {
	/*Matrix4x4 jointMat = animetor_->GetSkeleton()->GetSkeltonSpaceMat("mixamorig:RightHand");
	swordMat_ = animetor_->GetSkeleton()->GetSkeltonSpaceMat("mixamorig:RightHand") * transform_->GetWorldMatrix();
	sword_->Update();*/
	BaseGameObject::Update();
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////
void TestObject::Draw() const {
	//sword_->Draw();
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
}
#endif