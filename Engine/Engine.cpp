#include "Engine.h"

Engine::Engine() {
}

Engine::~Engine() {
	
}

void Engine::Initialize(uint32_t backBufferWidth, int32_t backBufferHeight) {
	kClientWidth_ = backBufferWidth;
	kClientHeight_ = backBufferHeight;

	lightKind_ = LightGroup::Directional;

	CoInitializeEx(0, COINIT_MULTITHREADED);

	// ↓インスタンスの生成
	winApp_ = WinApp::GetInstance();
	dxCommon_ = DirectXCommon::GetInstacne();
	imguiManager_ = ImGuiManager::GetInstacne();
	textureManager_ = TextureManager::GetInstance();
	input_ = Input::GetInstance();

	// ↓各初期化
	winApp_->CreateGameWindow();
	dxCommon_->Initialize(winApp_, kClientWidth_, kClientHeight_);
	dxDevice_ = std::make_unique<DirectXDevice>(dxCommon_->GetUseAdapter());

	assert(dxDevice_->GetDevice());

	dxCommands_ = std::make_unique<DirectXCommands>(dxDevice_->GetDevice());
	descriptorHeap_ = std::make_unique<DescriptorHeap>(dxDevice_->GetDevice());

	// dxcommon
	dxCommon_->Setting(dxDevice_->GetDevice(), dxCommands_.get(), descriptorHeap_.get());

	// ImGui
	imguiManager_->Init(winApp_->GetHwnd(), dxDevice_->GetDevice(), dxCommon_->GetSwapChainBfCount(), descriptorHeap_->GetSRVHeap());

	// texture
	textureManager_->Initialize(dxDevice_->GetDevice(), dxCommands_->GetCommandList(), descriptorHeap_->GetSRVHeap(), dxCommon_->GetDescriptorSize()->GetSRV());
	descriptorHeap_->SetUseSrvIndex(textureManager_->GetSRVDataIndex() + 1);

	// dxCompiler
	dxCompiler_ = std::make_unique<DirectXCompiler>();

	// shader
	shaders_.Load("Engine/HLSL/Object3d.VS.hlsl", "Engine/HLSL/Object3d.PS.hlsl", Shader::Normal);
	shaders_.Load("Engine/HLSL/Object3d.VS.hlsl", "Engine/HLSL/Textureless.PS.hlsl", Shader::TextureLess);
	shaders_.Load("Engine/HLSL/Primitive.VS.hlsl", "Engine/HLSL/Primitive.PS.hlsl", Shader::Primitive);
	shaders_.Load("Engine/HLSL/Object3d.VS.hlsl", "Engine/HLSL/Phong.Lighting.hlsl", Shader::Phong);
	shaders_.Load("Engine/HLSL/PBR.VS.hlsl", "Engine/HLSL/PBR.PS.hlsl", Shader::PBR);
	shaders_.Load("Engine/HLSL/Particle.VS.hlsl", "Engine/HLSL/Particle.PS.hlsl", Shader::Particle);

	// pipeline
	pipeline_ = std::make_unique<Pipeline>(dxDevice_->GetDevice(),dxCompiler_.get(),
				shaders_.GetShaderData(Shader::Normal), NormalPipeline);

	texturelessPipeline_ = std::make_unique<Pipeline>(dxDevice_->GetDevice(),dxCompiler_.get(),
						shaders_.GetShaderData(Shader::TextureLess), TextureLessPipeline);

	phongPipeline_ = std::make_unique<Pipeline>(dxDevice_->GetDevice(), dxCompiler_.get(),
					shaders_.GetShaderData(Shader::Phong), NormalPipeline);

	primitivePipeline_ = std::make_unique<PrimitivePipeline>(dxDevice_->GetDevice(), dxCompiler_.get(),
						shaders_.GetShaderData(Shader::Primitive));

	pbrPipeline_ = std::make_unique<Pipeline>(dxDevice_->GetDevice(), dxCompiler_.get(),
						shaders_.GetShaderData(Shader::PBR), NormalPipeline);

	particlePipeline_ = std::make_unique<Pipeline>(dxDevice_->GetDevice(), dxCompiler_.get(),
						shaders_.GetShaderData(Shader::Particle), ParticlePipeline);

	// light
	lightGroup_ = std::make_unique<LightGroup>();
	lightGroup_->Init(dxDevice_->GetDevice());

	// input
	input_->Initialize(winApp_->GetWNDCLASS(), winApp_->GetHwnd());

	// primitive
	primitiveDrawer_ = std::make_unique<PrimitiveDrawer>();
	primitiveDrawer_->Initialize(dxDevice_->GetDevice());

	Log("Clear!\n");

	lightKind_ = LightGroup::Directional;
}

void Engine::Finalize() {
	primitiveDrawer_->Finalize();
	lightGroup_->Finalize();

	pbrPipeline_->Finalize();
	primitivePipeline_->Finalize();
	phongPipeline_->Finalize();
	pipeline_->Finalize();
	texturelessPipeline_->Finalize();
	particlePipeline_->Finalize();

	dxCompiler_->Finalize();
	descriptorHeap_->Finalize();
	dxCommands_->Finalize();
	dxCommon_->Finalize();
	dxDevice_->Finalize();

	imguiManager_->Finalize();
	textureManager_->Finalize();
	winApp_->Finalize();

	CoUninitialize();
}

bool Engine::ProcessMessage() {
	return  winApp_->ProcessMessage();
}

void Engine::BeginFrame() {
	imguiManager_->Begin();
	dxCommon_->Begin();

	input_->Update();

	lightGroup_->Update();

	primitiveDrawer_->SetUseIndex(0);
}

void Engine::EndFrame() {
	imguiManager_->End();
	imguiManager_->Draw(dxCommands_->GetCommandList());
	dxCommon_->End();
}

//------------------------------------------------------------------------------------------------------
// 生成
//------------------------------------------------------------------------------------------------------
std::unique_ptr<Triangle> Engine::CreateTriangle(const Mesh::Vertices& vertex) {
	std::unique_ptr<Triangle> triangle = std::make_unique<Triangle>();
	triangle->Init(dxDevice_->GetDevice(), vertex);
	return triangle;
}

std::unique_ptr<Sprite> Engine::CreateSprite(const Mesh::RectVetices& rect) {
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	sprite->Init(dxDevice_->GetDevice(), rect);
	return sprite;
}

std::unique_ptr<Sphere> Engine::CreateSphere(const uint32_t& devision) {
	std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>();
	sphere->Init(dxDevice_->GetDevice(), devision);
	return sphere;
}

std::unique_ptr<Model> Engine::CreateModel(const std::string& filePath) {
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Init(dxDevice_->GetDevice(), "Resources", filePath);
	return model;
}

std::unique_ptr<BaseParticle> Engine::CreateBaseParticle(const std::string& fileName, const uint32_t& instanceNum) {
	std::unique_ptr<BaseParticle> particle = std::make_unique<BaseParticle>();
	particle->Init(dxDevice_->GetDevice(), "Resources", fileName, instanceNum);
	particle->CreateSRV(dxDevice_->GetDevice(), descriptorHeap_->GetSRVHeap(), dxCommon_->GetDescriptorSize()->GetSRV(), descriptorHeap_->GetUseSrvIndex());
	return particle;
}

//------------------------------------------------------------------------------------------------------
// 描画
//------------------------------------------------------------------------------------------------------
void Engine::DrawTriangle(Triangle* triangle) {
	pipeline_->Draw(dxCommands_->GetCommandList());
	lightGroup_->Draw(dxCommands_->GetCommandList(), 2, lightKind_);
	triangle->Draw(dxCommands_->GetCommandList());
}

void Engine::DrawSprite(Sprite* sprite) {
	pipeline_->Draw(dxCommands_->GetCommandList());
	lightGroup_->Draw(dxCommands_->GetCommandList(), 2, lightKind_);
	sprite->Draw(dxCommands_->GetCommandList());
}

void Engine::DrawSphere(Sphere* sphere) {
	if (lightKind_ == LightGroup::Directional) {
		//pipeline_->Draw(dxCommands_->GetCommandList());
		pipeline_->Draw(dxCommands_->GetCommandList());
	} else if(LightGroup::PBR) {
		pbrPipeline_->Draw(dxCommands_->GetCommandList());
	}
	lightGroup_->Draw(dxCommands_->GetCommandList(), 2, lightKind_);
	sphere->Draw(dxCommands_->GetCommandList());
}

void Engine::DrawModel(Model* model) {
	if (lightKind_ == LightGroup::Directional) {
		pipeline_->Draw(dxCommands_->GetCommandList());
	} else if (LightGroup::PBR) {
		pbrPipeline_->Draw(dxCommands_->GetCommandList());
	}
	
	lightGroup_->Draw(dxCommands_->GetCommandList(), 2, lightKind_);
	model->Draw(dxCommands_->GetCommandList());
}

void Engine::DrawLine(const Vector3& p1, const Vector3& p2, const Vector4& color, const Matrix4x4& vpMat) {
	primitivePipeline_->Draw(dxCommands_->GetCommandList());
	primitiveDrawer_->Draw(dxCommands_->GetCommandList(), p1, p2, color, vpMat);
}

void Engine::DrawParticle(BaseParticle* baseParticle, const uint32_t& numInstance) {
	particlePipeline_->Draw(dxCommands_->GetCommandList());
	lightGroup_->Draw(dxCommands_->GetCommandList(), 2, lightKind_);
	baseParticle->Draw(dxCommands_->GetCommandList(), numInstance);
}

//------------------------------------------------------------------------------------------------------
// ライトの設定
//------------------------------------------------------------------------------------------------------
void Engine::SetLightKind(const LightGroup::LightKind& kind) {
	lightKind_ = kind;
}

void Engine::SetEyePos(const Vector3& eyePos) {
	lightGroup_->SetEyePos(eyePos);
}
