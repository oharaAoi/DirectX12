#include "Render.h"

Render::Render() {}
Render::~Render() {}

void Render::Finalize() {
	viewProjection2D_->Finalize();
	viewProjection_->Finalize();
	lightGroup_->Finalize();
	primitiveDrawer_->Finalize();
}

Render* Render::GetInstacne() {
	static Render instance;
	return &instance;
}

void Render::Init(ID3D12GraphicsCommandList* commandList, ID3D12Device* device, PrimitivePipeline* primitive) {
	assert(commandList);
	commandList_ = commandList;
	primitivePipelines_ = primitive;

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection2D_ = std::make_unique<ViewProjection>();
	primitiveDrawer_ = std::make_unique<PrimitiveDrawer>();
	lightGroup_ = std::make_unique<LightGroup>();

	viewProjection_->Init(device);
	viewProjection2D_->Init(device);

	// light
	lightGroup_->Init(device);

	primitiveDrawer_->Init(device);
}

void Render::Update() {
	lightGroup_->Update();
	primitiveDrawer_->SetUseIndex(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　三角形の描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void Render::DrawTriangle(Triangle* triangle, const WorldTransform& worldTransform) {
	lightGroup_->Draw(commandList_, 4);
	triangle->Draw(commandList_, worldTransform, viewProjection_.get());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Spriteの描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void Render::DrawSprite(Sprite* sprite) {
	sprite->Draw(commandList_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　球体の描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void Render::DrawSphere(Sphere* sphere, const WorldTransform& worldTransform) {
	lightGroup_->Draw(commandList_, 4);
	sphere->Draw(commandList_, worldTransform, viewProjection_.get());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　モデルの描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void Render::DrawModel(Model* model, const WorldTransform& worldTransform) {
	if (model->GetHasTexture()) {
		lightGroup_->Draw(commandList_, 4);
	} else {
		lightGroup_->Draw(commandList_, 3);
	}
	model->Draw(commandList_, worldTransform, viewProjection_.get());
}

void Render::DrawAnimationModel(Model* model, const Skinning* skinning, const WorldTransform& worldTransform) {
	lightGroup_->Draw(commandList_, 5);
	model->DrawSkinning(commandList_, skinning, worldTransform, viewProjection_.get());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Particleの描画
//////////////////////////////////////////////////////////////////////////////////////////////////
void Render::DrawParticle(BaseParticle* baseParticle, const uint32_t& numInstance) {
	lightGroup_->DrawLi(commandList_, 3);
	baseParticle->Draw(commandList_, numInstance);
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　線の描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void Render::DrawLine(const Vector3& p1, const Vector3& p2, const Vector4& color, const Matrix4x4& vpMat) {
	primitivePipelines_->Draw(commandList_);
	primitiveDrawer_->Draw(commandList_, p1, p2, color, vpMat);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　設定系の関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Render::SetViewProjection(const Matrix4x4& view, const Matrix4x4& projection) {
	viewProjection_->SetViewProjection(view, projection);
}

void Render::SetViewProjection2D(const Matrix4x4& view, const Matrix4x4& projection) {
	viewProjection2D_->SetViewProjection(view, projection);
}

void Render::SetEyePos(const Vector3& eyePos) {
	lightGroup_->SetEyePos(eyePos);
}
