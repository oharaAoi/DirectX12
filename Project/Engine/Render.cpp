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

void Render::Init(ID3D12GraphicsCommandList* commandList, ID3D12Device* device, PrimitivePipeline* primitive, RenderTarget* renderTarget) {
	assert(commandList);
	commandList_ = commandList;
	primitivePipelines_ = primitive;
	GetInstacne()->renderTarget_ = renderTarget;

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection2D_ = std::make_unique<ViewProjection>();
	primitiveDrawer_ = std::make_unique<PrimitiveDrawer>();
	lightGroup_ = std::make_unique<LightGroup>();

	viewProjection_->Init(device);
	viewProjection2D_->Init(device);

	// light
	lightGroup_->Init(device);

	primitiveDrawer_->Init(device);


	nearClip_ = 1.0f;
	farClip_ = 10000.0f;

	nearClip2D_ = 0.0f;
	farClip2D_ = 100.0f;
}

void Render::Update() {
	lightGroup_->Update();
	primitiveDrawer_->SetUseIndex(0);
}

void Render::Begin() {
	GetInstacne()->renderTarget_->SetRenderTarget(commandList_, RenderTargetType::Object3D_RenderTarget);
}

void Render::SetRenderTarget(const RenderTargetType& type) {
	GetInstacne()->renderTarget_->SetRenderTarget(commandList_, type);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　三角形の描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void Render::DrawTriangle(Triangle* triangle) {
	lightGroup_->Draw(commandList_, 4);
	triangle->Draw(commandList_, viewProjection_.get());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Spriteの描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void Render::DrawSprite(Sprite* sprite) {
	sprite->PostDraw(commandList_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　球体の描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void Render::DrawSphere(Sphere* sphere, const WorldTransform* worldTransform) {
	lightGroup_->Draw(commandList_, 4);
	sphere->Draw(commandList_, worldTransform, viewProjection_.get());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　モデルの描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void Render::DrawModel(Model* model, const WorldTransform* worldTransform, const std::vector<std::unique_ptr<Material>>& materials) {
	if (model->GetHasTexture()) {
		lightGroup_->Draw(commandList_, 4);
	} else {
		lightGroup_->Draw(commandList_, 3);
	}
	model->Draw(commandList_, worldTransform, viewProjection_.get(), materials);
}

void Render::DrawAnimationModel(Model* model, const Skinning* skinning,
								const WorldTransform* worldTransform,
								const std::vector<std::unique_ptr<Material>>& materials) {
	lightGroup_->Draw(commandList_, 5);
	model->DrawSkinning(commandList_, skinning, worldTransform, viewProjection_.get(), materials);
}

void Render::DrawAnimationModels(Model* model, const std::vector<std::unique_ptr<Skinning>>& skinning, const WorldTransform* worldTransform, const std::vector<std::unique_ptr<Material>>& materials) {
	lightGroup_->Draw(commandList_, 5);
	model->DrawSkinnings(commandList_, skinning, worldTransform, viewProjection_.get(), materials);
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
	primitiveDrawer_->Draw(commandList_, p1, p2, color, vpMat);
}

void Render::DrawLightGroup(const int& startIndex) {
	lightGroup_->DrawLi(commandList_, startIndex);
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

Matrix4x4 Render::GetViewport2D() {
	return  viewProjection2D_->GetViewMatrix();
}

Matrix4x4 Render::GetViewport3D() {
	return viewProjection_->GetViewMatrix();
}

Matrix4x4 Render::GetProjection2D() {
	return  viewProjection2D_->GetProjectionMatrix();
}

Matrix4x4 Render::GetProjection3D() {
	return viewProjection_->GetProjectionMatrix();
}

float Render::GetNearClip() {
	return nearClip_;
}

float Render::GetNearClip2D() {
	return nearClip2D_;
}

float Render::GetFarClip() {
	return farClip_;
}

float Render::GetFarClip2D() {
	return farClip2D_;
}

void Render::SetEyePos(const Vector3& eyePos) {
	lightGroup_->SetEyePos(eyePos);
}

const ViewProjection* Render::GetViewProjection(){
	return viewProjection_.get();
}
