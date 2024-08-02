#include "Engine.h"

Engine::Engine() {}

Engine::~Engine() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

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
	renderTarget_ = std::make_unique<RenderTarget>();
	dxCompiler_ = std::make_unique<DirectXCompiler>();
	shaders_ = std::make_unique<Shader>();

	graphicsPipelines_ = std::make_unique<GraphicsPipelines>();
	computeShader_ = std::make_unique<ComputeShader>();
	primitiveDrawer_ = std::make_unique<PrimitiveDrawer>();

	lightGroup_ = std::make_unique<LightGroup>();

	viewProjection_ = std::make_unique<ViewProjection>();
	viewProjection2D_ = std::make_unique<ViewProjection>();

	renderTexture_ = std::make_unique<RenderTexture>();

	audio_ = std::make_unique<Audio>();


	// dxcommon
	dxCommon_->Setting(dxDevice_->GetDevice(), dxCommands_.get(), descriptorHeap_.get(), renderTarget_.get());
	// ImGui
	imguiManager_->Init(winApp_->GetHwnd(), dxDevice_->GetDevice(), dxCommon_->GetSwapChainBfCount(), descriptorHeap_->GetSRVHeap());
	// renderTarget
	renderTarget_->Init(dxDevice_->GetDevice(), descriptorHeap_.get(), dxCommon_->GetSwapChain().Get());
	// texture
	textureManager_->Init(dxDevice_->GetDevice(), dxCommands_->GetCommandList(), descriptorHeap_.get());
	// shader
	shaders_->Init();
	// pipeline
	graphicsPipelines_->Init(dxDevice_->GetDevice(), dxCompiler_.get(), shaders_.get());
	primitivePipeline_ = std::make_unique<PrimitivePipeline>(dxDevice_->GetDevice(), dxCompiler_.get(), shaders_->GetShaderData(Shader::Primitive));
	// CS
	computeShader_->Init(dxDevice_->GetDevice(), dxCompiler_.get(), descriptorHeap_.get(),renderTarget_->GetOffScreenSRVHandle(RenderTargetType::OffScreen_RenderTarget), shaders_.get());
	// light
	lightGroup_->Init(dxDevice_->GetDevice());
	// input
	input_->Init(winApp_->GetWNDCLASS(), winApp_->GetHwnd());
	// audio
	audio_->Init();
	// primitive
	primitiveDrawer_->Init(dxDevice_->GetDevice());

	viewProjection_->Init(dxDevice_->GetDevice());
	viewProjection2D_->Init(dxDevice_->GetDevice());

	renderTexture_->Init(dxDevice_->GetDevice());

	Log("Clear!\n");

	lightKind_ = LightGroup::Directional;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　終了処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::Finalize() {
	renderTexture_->Finalize();

	viewProjection2D_->Finalize();
	viewProjection_->Finalize();

	computeShader_->Finalize();

	primitivePipeline_->Finalize();
	graphicsPipelines_->Finalize();
	primitiveDrawer_->Finalize();
	lightGroup_->Finalize();

	renderTarget_->Finalize();
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


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ImGuiを描画する
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::DrawImGui() {
	ImGui::Begin("Engine");

	ImGui::Checkbox("RunCS", &isRunCS_);

	ImGui::End();
}

bool Engine::ProcessMessage() {
	return  winApp_->ProcessMessage();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　フレーム開始時の処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::BeginFrame() {
	imguiManager_->Begin();
	dxCommon_->Begin();

	input_->Update();

	lightGroup_->Update();

	primitiveDrawer_->SetUseIndex(0);

	DrawImGui();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　フレーム終了時の処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::EndFrame() {
	imguiManager_->End();
	imguiManager_->Draw(dxCommands_->GetCommandList());
	dxCommon_->End();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　offScreenRenderingの処理をこの関数内で行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::EndRenderTexture() {
	// offScreenRenderingのResourceの状態を変更する
	dxCommon_->ChangeOffScreenResource();
	// これから書き込む画面をバックバッファに変更する
	dxCommon_->SetSwapChain();

	if (isRunCS_) {
		//----------------------------------------------------------------
		// computerShaderを実行する
		computeShader_->RunComputeShader(dxCommands_->GetCommandList());
		//----------------------------------------------------------------

		// スプライト用のパイプラインの設定
		graphicsPipelines_->SetPipeline(PipelineType::SpritePipeline, dxCommands_->GetCommandList());
		// computeShaderで加工したTextureを描画する
		renderTexture_->Draw(dxCommands_->GetCommandList(), computeShader_->GetShaderResourceHandleGPU());
		//----------------------------------------------------------------
		
		// リソースの状態をShaderResourceから書き込める状態にする(SR→UA)
		computeShader_->TransitionUAVResource(dxCommands_->GetCommandList(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	} else {
		// スプライト用のパイプラインの設定
		graphicsPipelines_->SetPipeline(PipelineType::SpritePipeline, dxCommands_->GetCommandList());
		// offScreenRenderingで書き込んだTextureを描画する
		renderTexture_->Draw(dxCommands_->GetCommandList(), renderTarget_->GetOffScreenSRVHandle(RenderTargetType::OffScreen_RenderTarget).handleGPU);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　生成する関数群
//////////////////////////////////////////////////////////////////////////////////////////////////

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
	particle->CreateSRV(dxDevice_->GetDevice(), descriptorHeap_.get(), instanceNum);
	return particle;
}

WorldTransform Engine::CreateWorldTransform() {
	WorldTransform result;
	result.Init(dxDevice_->GetDevice());
	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画する関数群
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::DrawTriangle(Triangle* triangle, const WorldTransform& worldTransform) {
	lightGroup_->Draw(dxCommands_->GetCommandList(), 4);
	triangle->Draw(dxCommands_->GetCommandList(), worldTransform, viewProjection_.get());
}

void Engine::DrawSprite(Sprite* sprite) {
	sprite->Draw(dxCommands_->GetCommandList());
}

void Engine::DrawSphere(Sphere* sphere, const WorldTransform& worldTransform) {
	lightGroup_->Draw(dxCommands_->GetCommandList(), 4);
	sphere->Draw(dxCommands_->GetCommandList(), worldTransform, viewProjection_.get());
}

void Engine::DrawModel(Model* model, const WorldTransform& worldTransform) {
	if (model->GetHasTexture()) {
		lightGroup_->Draw(dxCommands_->GetCommandList(), 4);
	} else {
		lightGroup_->Draw(dxCommands_->GetCommandList(), 3);
	}
	model->Draw(dxCommands_->GetCommandList(), worldTransform, viewProjection_.get());
}

void Engine::DrawLine(const Vector3& p1, const Vector3& p2, const Vector4& color, const Matrix4x4& vpMat) {
	primitivePipeline_->Draw(dxCommands_->GetCommandList());
	primitiveDrawer_->Draw(dxCommands_->GetCommandList(), p1, p2, color, vpMat);
}

void Engine::DrawParticle(BaseParticle* baseParticle, const uint32_t& numInstance) {
	lightGroup_->DrawLi(dxCommands_->GetCommandList(), 3);
	baseParticle->Draw(dxCommands_->GetCommandList(), numInstance);
}

void Engine::DrawBaseGameObject(BaseGameObject* gameObject, const WorldTransform& worldTransform) {
	lightGroup_->Draw(dxCommands_->GetCommandList(), 5);
	gameObject->Draw(dxCommands_->GetCommandList(), worldTransform, viewProjection_.get());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ライトの設定
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::SetLightKind(const LightGroup::LightKind& kind) {
	lightKind_ = kind;
}

void Engine::SetEyePos(const Vector3& eyePos) {
	lightGroup_->SetEyePos(eyePos);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　パイプラインの設定
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::SetPipeline(const PipelineKind& kind) {
	switch (kind) {
	case PipelineKind::kNormalPipeline:
		graphicsPipelines_->SetPipeline(PipelineType::NormalPipeline, dxCommands_->GetCommandList());
		break;
	case PipelineKind::kTexturelessPipeline:
		graphicsPipelines_->SetPipeline(PipelineType::TextureLessPipeline, dxCommands_->GetCommandList());
		break;
	case PipelineKind::kPBRPipeline:
		graphicsPipelines_->SetPipeline(PipelineType::PBRPipeline, dxCommands_->GetCommandList());
		break;
	case PipelineKind::kParticlePipeline:
		graphicsPipelines_->SetPipeline(PipelineType::ParticlePipeline, dxCommands_->GetCommandList());
		break;
	case PipelineKind::kSpritePipeline:
		graphicsPipelines_->SetPipeline(PipelineType::SpritePipeline, dxCommands_->GetCommandList());
		
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ Viewの設定
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::SetViewProjection(const Matrix4x4& view, const Matrix4x4& projection) {
	viewProjection_->SetViewProjection(view, projection);
}

void Engine::SetViewProjection2D(const Matrix4x4& view, const Matrix4x4& projection) {
	viewProjection2D_->SetViewProjection(view, projection);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Sound系
//////////////////////////////////////////////////////////////////////////////////////////////////

SoundData Engine::LoadAudio(const std::string& fileName) {
	return audio_->SoundLoadWave(fileName.c_str());
}

BgmData Engine::LoadBGM(const std::string& fileName) {
	return audio_->LoadBGM(fileName.c_str());
}

SeData Engine::LoadSE(const std::string& fileName) {
	return audio_->LoadSE(fileName.c_str());
}

void Engine::PlayAudio(const SoundData& soundData) {
	audio_->SoundPlayWave(soundData);
}

void Engine::PlayBGM(const BgmData& soundData, const bool& isLoop) {
	audio_->PlayBGM(soundData, isLoop);
}

void Engine::PlaySE(const SeData& soundData, const bool& isLoop) {
	audio_->PlaySE(soundData, isLoop);
}

void Engine::PauseBGM(const BgmData& soundData) {
	audio_->PauseAudio(soundData.pSourceVoice);
}

void Engine::ReStartBGM(const BgmData& soundData) {
	audio_->ReStartAudio(soundData.pSourceVoice);
}

void Engine::StopBGM(const BgmData& soundData) {
	audio_->StopAudio(soundData.pSourceVoice);
}
