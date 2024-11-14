#include "TestObject.h"

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
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TestObject::Update() {
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
}
#endif