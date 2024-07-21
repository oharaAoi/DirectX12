#include "TaskScene.h"

TaskScene::TaskScene() {
}

TaskScene::~TaskScene() {
}

void TaskScene::Init() {
	// カメラ -------------------------------------------------------------------
	camera_ = std::make_unique<Camera>();

	rect_ = {
		{0.0f, 0.0f, 0.0f, 1.0f},
		{640.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 360.0f, 0.0f, 1.0f},
		{640.0f, 360.0f, 0.0f, 1.0f}
	};

	vertices_ = {
		{-0.5f, -0.5f, 0.0f, 1.0f},
		{ 0.0f, 0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f }
	};

	// model -------------------------------------------------------------------

	modelMap_.emplace("plane", Engine::CreateModel("plane.obj"));
	modelMap_.emplace("teapot", Engine::CreateModel("teapot.obj"));
	modelMap_.emplace("bunny", Engine::CreateModel("bunny.obj"));
	modelMap_.emplace("multiMesh", Engine::CreateModel("multiMesh.obj"));
	modelMap_.emplace("multiMaterial", Engine::CreateModel("multiMaterial.obj"));
	modelMap_.emplace("suzanne", Engine::CreateModel("suzanne.obj"));

	objectKind_ = 0;
}

void TaskScene::Update() {
	// カメラの更新 -------------------------------------------------------------------
	camera_->Update();
	Engine::SetEyePos(camera_->GetWorldTranslate());
	Engine::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
	Engine::SetViewProjection2D(camera_->GetViewMatrix2D(), camera_->GetProjectionMatrix2D());

	AddObject();

	for (std::list<ObjectData>::iterator iter = gameObjectList_.begin(); iter != gameObjectList_.end();) {
		// ワールド座標の更新	
		iter->worldTransform.Update();
		// ImGuiの描画
		ImGui::Begin("Setting");
		if (ImGui::TreeNode(iter->objectName.c_str())) {
			iter->worldTransform.ImGuiDraw();
			iter->gameObject->ImGuiDraw("Material");

			// 削除
			if (ImGui::Button("Delete")) {
				iter = gameObjectList_.erase(iter);
				ImGui::TreePop();
				ImGui::End();
				continue;
			}

			ImGui::TreePop();
		}
		ImGui::End();
		iter++;
	}

	for (std::list<ObjectData>::iterator iter = textureLessList_.begin(); iter != textureLessList_.end();) {
		iter->worldTransform.Update();
		// ImGuiの描画
		ImGui::Begin("Setting");
		if (ImGui::TreeNode(iter->objectName.c_str())) {
			iter->worldTransform.ImGuiDraw();
			iter->gameObject->ImGuiDraw("Material");

			// 削除
			if (ImGui::Button("Delete")) {
				iter = gameObjectList_.erase(iter);
				ImGui::TreePop();
				ImGui::End();
				continue;
			}

			ImGui::TreePop();
		}
		ImGui::End();
		iter++;
	}

	uint32_t spriteNum = 0;
	for (std::list<std::unique_ptr<Sprite>> ::iterator iter = spriteList_.begin(); iter != spriteList_.end();) {
		ImGui::Begin("Setting");
		std::string name = "sprite" + spriteNum;
		if (ImGui::TreeNode(name.c_str())) {
			iter->get()->Update();

			// 削除
			if (ImGui::Button("Delete")) {
				iter = spriteList_.erase(iter);
				ImGui::TreePop();
				ImGui::End();
				continue;
			}
			ImGui::TreePop();
		}
		ImGui::End();
		spriteNum++;
		iter++;
	}
}

void TaskScene::Draw() {
#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);

	for (std::list<ObjectData>::iterator iter = gameObjectList_.begin(); iter != gameObjectList_.end();) {
		if (iter->modelObject == nullptr) {
			Engine::DrawBaseGameObject(iter->gameObject.get(), iter->worldTransform);
		} else {
			Engine::DrawBaseGameObject(iter->modelObject, iter->worldTransform);
		}
		iter++;
	}

#pragma endregion

#pragma region Textureless

	Engine::SetPipeline(PipelineKind::kTexturelessPipeline);
	for (std::list<ObjectData>::iterator iter = textureLessList_.begin(); iter != textureLessList_.end();) {
		Engine::DrawModel(modelMap_["suzanne"].get(), iter->worldTransform);
		iter++;
	}

#pragma endregion

#pragma region PBR

	Engine::SetPipeline(PipelineKind::kPBRPipeline);

#pragma endregion

#pragma region Particle

	Engine::SetPipeline(PipelineKind::kParticlePipeline);

#pragma endregion

#pragma region Spite

	Engine::SetPipeline(PipelineKind::kSpritePipeline);
	for (std::list<std::unique_ptr<Sprite>> ::iterator iter = spriteList_.begin(); iter != spriteList_.end();) {
		Engine::DrawSprite(iter->get());
		iter++;
	}

#pragma endregion
}

void TaskScene::AddObject() {
	ImGui::Begin("Setting");

	ImGui::Combo("object",&objectKind_, "triangle\0sphere\0plane\0teapot\0bunny\0multiMesh\0multiMaterial\0suzanne\0sprite");
	if (ImGui::Button("Create")) {
		ObjectData data{};
		std::string objectNum = std::to_string(gameObjectList_.size());

		switch (objectKind_) {
		case kTriangle:
			data.gameObject = Engine::CreateTriangle(vertices_);
			data.worldTransform = Engine::CreateWorldTransform();
			data.objectName = "triangle" + objectNum;
			gameObjectList_.push_back(std::move(data));
			break;

		case kSphere:
			data.gameObject = Engine::CreateSphere(16);
			data.worldTransform = Engine::CreateWorldTransform();
			data.objectName = "sphere" + objectNum;
			gameObjectList_.push_back(std::move(data));
			break;

		case kPlane:
			data.modelObject = modelMap_["plane"].get();
			data.worldTransform = Engine::CreateWorldTransform();
			data.objectName = "plane" + objectNum;
			gameObjectList_.push_back(std::move(data));
			break;
		case kTeapot:
			data.modelObject = modelMap_["teapot"].get();
			data.worldTransform = Engine::CreateWorldTransform();
			data.objectName = "teapot" + objectNum;
			gameObjectList_.push_back(std::move(data));
			break;
		case kBunny:
			data.modelObject = modelMap_["bunny"].get();
			data.worldTransform = Engine::CreateWorldTransform();
			data.objectName = "bunny" + objectNum;
			gameObjectList_.push_back(std::move(data));
			break;
		case kMultiMesh:
			data.modelObject = modelMap_["multiMesh"].get();
			data.worldTransform = Engine::CreateWorldTransform();
			data.objectName = "multiMesh" + objectNum;
			gameObjectList_.push_back(std::move(data));
			break;
		case kMultiMaterial:
			data.modelObject = modelMap_["multiMaterial"].get();
			data.worldTransform = Engine::CreateWorldTransform();
			data.objectName = "multiMaterial" + objectNum;
			gameObjectList_.push_back(std::move(data));
			break;

		case kSuzanne:
			data.modelObject = modelMap_["suzanne"].get();
			data.worldTransform = Engine::CreateWorldTransform();
			data.objectName = "suzanne" + objectNum;
			textureLessList_.push_back(std::move(data));
			break;

		case kSprite:
			spriteList_.push_back(Engine::CreateSprite(rect_));
			break;

		}
	}

	ImGui::End();
}
