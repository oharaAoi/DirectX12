#include "Engine.h"
#include "Engine/System/ParticleSystem/EffectSystem.h"

Engine::Engine() {}

Engine::~Engine() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::Initialize(uint32_t backBufferWidth, int32_t backBufferHeight) {
	kClientWidth_ = backBufferWidth;
	kClientHeight_ = backBufferHeight;

	CoInitializeEx(0, COINIT_MULTITHREADED);

	// -------------------------------------------------
	// ↓ インスタンスの作成
	// -------------------------------------------------
	winApp_ = WinApp::GetInstance();
	dxCommon_ = DirectXCommon::GetInstacne();
	textureManager_ = TextureManager::GetInstance();
	input_ = Input::GetInstance();
	render_ = Render::GetInstacne();
	effectSystem_ = EffectSystem::GetInstacne();
	editerWindows_ = EditerWindows::GetInstance();

	winApp_->CreateGameWindow();
	dxCommon_->Initialize(winApp_, kClientWidth_, kClientHeight_);
	dxDevice_ = std::make_shared<DirectXDevice>(dxCommon_->GetUseAdapter());

	dxCommands_ = std::make_unique<DirectXCommands>(dxDevice_->GetDevice());
	descriptorHeap_ = std::make_shared<DescriptorHeap>(dxDevice_->GetDevice());
	renderTarget_ = std::make_unique<RenderTarget>();
	dxCompiler_ = std::make_unique<DirectXCompiler>();
	shaders_ = std::make_unique<Shader>();

	graphicsPipelines_ = std::make_unique<GraphicsPipelines>();
	primitivePipeline_ = std::make_unique<PrimitivePipeline>();
	computeShader_ = std::make_unique<ComputeShader>();

	renderTexture_ = std::make_unique<RenderTexture>();
	audio_ = std::make_unique<Audio>();

#ifdef _DEBUG
	editerWindows_->Init();
#endif

	// -------------------------------------------------
	// ↓ 各初期化
	// -------------------------------------------------
	shaders_->Init();
	dxCommon_->Setting(dxDevice_->GetDevice(), dxCommands_.get(), descriptorHeap_.get(), renderTarget_.get());
	renderTarget_->Init(dxDevice_->GetDevice(), descriptorHeap_.get(), dxCommon_->GetSwapChain().Get());
	textureManager_->Init(dxDevice_, dxCommands_->GetCommandList(), descriptorHeap_);
	graphicsPipelines_->Init(dxDevice_->GetDevice(), dxCompiler_.get(), shaders_.get());
	primitivePipeline_->Init(dxDevice_->GetDevice(), dxCompiler_.get(), shaders_->GetShaderData(Shader::Primitive));
	computeShader_->Init(dxDevice_->GetDevice(), dxCompiler_.get(), descriptorHeap_.get(), renderTarget_->GetRenderTargetSRVHandle(RenderTargetType::Object3D_RenderTarget), shaders_.get());
	input_->Init(winApp_->GetWNDCLASS(), winApp_->GetHwnd());
	render_->Init(dxCommands_->GetCommandList(), dxDevice_->GetDevice(), primitivePipeline_.get(), renderTarget_.get());
	renderTexture_->Init(dxDevice_->GetDevice(), descriptorHeap_.get());
	audio_->Init();
	effectSystem_->Init();

#ifdef _DEBUG
	imguiManager_ = ImGuiManager::GetInstacne();
	imguiManager_->Init(winApp_->GetHwnd(), dxDevice_->GetDevice(), dxCommon_->GetSwapChainBfCount(), descriptorHeap_->GetSRVHeap());
	EffectSystem::GetInstacne()->EditerInit(renderTarget_.get(), descriptorHeap_.get(), dxCommands_.get(), dxDevice_->GetDevice());
#endif

	// -------------------------------------------------
	// ↓ その他初期化
	// -------------------------------------------------
	isFullScreen_ = false;
	isEffectEditer_ = true;
	runGame_ = true;

	Render::SetRenderTarget(RenderTargetType::Object3D_RenderTarget);

	Log("Engine Initialize compulete!\n");
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　終了処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::Finalize() {
	audio_->Finalize();

	renderTexture_->Finalize();

	computeShader_->Finalize();
	primitivePipeline_->Finalize();
	graphicsPipelines_->Finalize();

	render_->Finalize();

	renderTarget_->Finalize();
	dxCompiler_->Finalize();
	descriptorHeap_->Finalize();
	dxCommands_->Finalize();
	dxCommon_->Finalize();
	dxDevice_->Finalize();
#ifdef _DEBUG
	imguiManager_->Finalize();
#endif
	textureManager_->Finalize();
	winApp_->Finalize();

	CoUninitialize();
}

bool Engine::ProcessMessage() {
	return  winApp_->ProcessMessage();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　フレーム開始時の処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::BeginFrame() {
	dxCommon_->Begin();
	input_->Update();

	if (Input::IsTriggerKey(DIK_F11)) {
		isFullScreen_ = !isFullScreen_;
		WinApp::GetInstance()->SetFullScreen(isFullScreen_);
	}
	Render::SetRenderTarget(RenderTargetType::Object3D_RenderTarget);

#ifdef _DEBUG
	imguiManager_->Begin();
	editerWindows_->Begine();

	if (ImGui::Begin("EffectSystem", nullptr)) {
		runGame_ = false;
	}
	ImGui::End();

	if (ImGui::Begin("Game Window", nullptr)) {
		runGame_ = true;
	}
	ImGui::End();
#endif // 
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　フレーム終了時の処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::EndFrame() {
	Engine::RenderFrame();
#ifdef _DEBUG
	editerWindows_->End();
	imguiManager_->End();
	imguiManager_->Draw(dxCommands_->GetCommandList());

	if (!runGame_) {
		effectSystem_->EndEditer();
	}
#endif

	dxCommon_->End();
	descriptorHeap_->FreeList();
	audio_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　1frameの結果を描画する
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::RenderFrame() {
	// gameで使用したlineの描画を開始する
	primitivePipeline_->Draw(dxCommands_->GetCommandList());
	Render::PrimitiveDrawCall();

	// 最終Textureの作成
	BlendFinalTexture();

#ifdef _DEBUG
	if (runGame_) {
		if (ImGui::Begin("Game Window", nullptr, ImGuiWindowFlags_MenuBar)) {
			editerWindows_->Update();
			renderTexture_->DrawGui();
		}
		ImGui::End();
	} else {
		if (ImGui::Begin("EffectSystem", nullptr)) {
			effectSystem_->EditerUpdate();
			effectSystem_->Debug_Gui();
		}
		ImGui::End();
	}
#endif

	// swapChainの変更
	dxCommon_->SetSwapChain();
	graphicsPipelines_->SetPipeline(PipelineType::SpritePipeline, dxCommands_->GetCommandList());

	renderTexture_->Draw(dxCommands_->GetCommandList());

	renderTarget_->TransitionResource(dxCommands_->GetCommandList(), Object3D_RenderTarget, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　最終的に描画するTextureを合成する
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::BlendFinalTexture() {
	// -------------------------------------------------
	// ↓ Resourceの状態を切り替える(obj3D, sprite2D, renderTexture)
	// -------------------------------------------------
	renderTarget_->TransitionResource(
		dxCommands_->GetCommandList(),
		Object3D_RenderTarget,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE
	);

	// 最終描画のTextureを書き込み可能状態にする
	renderTexture_->TransitionResource(dxCommands_->GetCommandList(),
									   D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
									   D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// -------------------------------------------------
	// ↓ object3Dと最終描画のTextureを合成する
	// -------------------------------------------------
	computeShader_->BlendRenderTarget(dxCommands_->GetCommandList(), renderTarget_->GetRenderTargetSRVHandle(Object3D_RenderTarget).handleGPU, renderTexture_->GetUAV());

	// -------------------------------------------------
	// ↓ 映すTextureをpixeslShaderで使えるようにする
	// -------------------------------------------------
	renderTexture_->TransitionResource(dxCommands_->GetCommandList(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　生成する関数群
//////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Triangle> Engine::CreateTriangle(const Mesh::Vertices& vertex, const std::string& textureName) {
	std::unique_ptr<Triangle> triangle = std::make_unique<Triangle>();
	triangle->Init(dxDevice_->GetDevice(), vertex, textureName);
	return triangle;
}

std::unique_ptr<Sprite> Engine::CreateSprite(const std::string& fileName) {
	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	sprite->Init(dxDevice_->GetDevice(), fileName);
	return sprite;
}

std::unique_ptr<Model> Engine::CreateModel(const std::string& directoryPath, const std::string& filePath) {
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Init(dxDevice_->GetDevice(), directoryPath, filePath);
	return model;
}

std::unique_ptr<BaseParticle> Engine::CreateBaseParticle(const std::string& directoryPath, const std::string& fileName, const uint32_t& instanceNum) {
	std::unique_ptr<BaseParticle> particle = std::make_unique<BaseParticle>();
	particle->Init(dxDevice_->GetDevice(), directoryPath, fileName, instanceNum);
	particle->CreateSRV(dxDevice_->GetDevice(), descriptorHeap_.get(), instanceNum);
	return particle;
}

std::unique_ptr<WorldTransform> Engine::CreateWorldTransform() {
	std::unique_ptr<WorldTransform> result = std::make_unique<WorldTransform>();
	result->Init(dxDevice_->GetDevice());
	return result;
}

std::unique_ptr<Skinning> Engine::CreateSkinning(Skeleton* skeleton, Model* model, uint32_t index) {
	std::unique_ptr<Skinning> result = std::make_unique<Skinning>();
	result->CreateSkinCluster(dxDevice_->GetDevice(), skeleton, model->GetMesh(index), descriptorHeap_.get(), model->GetSkinClustersData(index));
	return result;
}

std::unique_ptr<Material> Engine::CreateMaterial(const Model::ModelMaterialData data) {
	std::unique_ptr<Material> material = std::make_unique<Material>();
	material->Init(dxDevice_->GetDevice(), data);
	return material;
}

std::unique_ptr<PBRMaterial> Engine::CreatePBRMaterial(const Model::ModelMaterialData data) {
	std::unique_ptr<PBRMaterial> material = std::make_unique<PBRMaterial>();
	material->Init(dxDevice_->GetDevice());
	material->SetMaterialData(data);
	return material;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　CSの設定
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::RunCS() {
	renderTarget_->TransitionResource(dxCommands_->GetCommandList(), Object3D_RenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	computeShader_->RunComputeShader(dxCommands_->GetCommandList());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　深度バッファをリセットする
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::ClearDepth() {
	renderTarget_->ClearDepth(dxCommands_->GetCommandList());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　パイプラインの設定
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::SetPipeline(const PipelineType& kind) {
	switch (kind) {
	case PipelineType::NormalPipeline:
		graphicsPipelines_->SetPipeline(PipelineType::NormalPipeline, dxCommands_->GetCommandList());
		break;
	case PipelineType::AddPipeline:
		graphicsPipelines_->SetPipeline(PipelineType::AddPipeline, dxCommands_->GetCommandList());
		break;
	case PipelineType::TextureLessPipeline:
		graphicsPipelines_->SetPipeline(PipelineType::TextureLessPipeline, dxCommands_->GetCommandList());
		break;
	case PipelineType::PrimitivePipeline:
		primitivePipeline_->Draw(dxCommands_->GetCommandList());
		break;
	case PipelineType::PBRPipeline:
		graphicsPipelines_->SetPipeline(PipelineType::PBRPipeline, dxCommands_->GetCommandList());
		break;
	case PipelineType::ParticlePipeline:
		graphicsPipelines_->SetPipeline(PipelineType::ParticlePipeline, dxCommands_->GetCommandList());
		break;
	case PipelineType::SpritePipeline:
		graphicsPipelines_->SetPipeline(PipelineType::SpritePipeline, dxCommands_->GetCommandList());
		break;
	case PipelineType::SpriteNormalBlendPipeline:
		graphicsPipelines_->SetPipeline(PipelineType::SpriteNormalBlendPipeline, dxCommands_->GetCommandList());
		break;
	case PipelineType::SpriteAddBlendPipeline:
		graphicsPipelines_->SetPipeline(PipelineType::SpriteAddBlendPipeline, dxCommands_->GetCommandList());
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　CSPipelineの設定
//////////////////////////////////////////////////////////////////////////////////////////////////

void Engine::SetCsPipeline(const CsPipelineType& kind) {
	computeShader_->SetCsPipeline(kind, dxCommands_->GetCommandList());
}

void Engine::SetSkinning(Skinning* skinning, Mesh* mesh) {
	computeShader_->SetCsPipeline(CsPipelineType::Skinning_Pipeline, dxCommands_->GetCommandList());

	mesh->SetInitVertex();
	skinning->RunCs(dxCommands_->GetCommandList());
	skinning->EndCS(dxCommands_->GetCommandList(), mesh);
}

void Engine::ResetComputeShader() {
	computeShader_->ResetComputeShader();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Sound系
//////////////////////////////////////////////////////////////////////////////////////////////////

SoundData Engine::LoadAudio(const std::string filePath) {
	return audio_->SoundLoad(filePath.c_str());
}

AudioData Engine::LoadAudio(const SoundData& loadAudioData) {
	return audio_->LoadAudio(loadAudioData);
}

void Engine::PlayAudio(const AudioData& soundData, bool isLoop, float volume, bool checkPlaying) {
	audio_->PlayAudio(soundData, isLoop, volume, checkPlaying);
}

void Engine::Pause(const AudioData& soundData) {
	audio_->PauseAudio(soundData.pSourceVoice);
}

void Engine::ReStart(const AudioData& soundData) {
	audio_->ReStartAudio(soundData.pSourceVoice);
}

void Engine::Stop(const AudioData& soundData) {
	audio_->StopAudio(soundData.pSourceVoice);
}

void Engine::SetVolume(const AudioData& soundData, float volume) {
	audio_->SetVolume(soundData.pSourceVoice, volume);
}

bool Engine::GetIsPlaying(const AudioData& soundData) {
	return audio_->IsPlaying(soundData.pSourceVoice);
}

void Engine::SingleShotPlay(const SoundData& loadAudioData, float volume) {
	audio_->SinglShotPlay(loadAudioData, volume);
}

ID3D12Device* Engine::GetDevice() {
	// TODO: return ステートメントをここに挿入します
	return dxDevice_->GetDevice();
}

ID3D12GraphicsCommandList* Engine::GetCommandList() {
	// TODO: return ステートメントをここに挿入します
	return dxCommands_->GetCommandList();
}

DescriptorHeap* Engine::GetDxHeap() {
	return descriptorHeap_.get();
}

bool Engine::GetIsOpenEffectEditer() {
	return isEffectEditer_;
}

bool Engine::GetRunGame() {
	return runGame_;
}
