#include "Emitter.h"
#include "BaseEffect.h"

Emitter::Emitter(BaseEffect* effect) : effect_(effect) {
	Init();
}

Emitter::~Emitter() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Init() {
	transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f,0.0f,0.0f},{0.0f, 0.0f, 0.0f} };
	count_ = 3;
	frequency_ = 1.0f;
	frequencyTime_ = 0.0f;
	size_ = { 0.1f, 0.1f, 0.1f };
	firstVelocity_ = { RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f) };
	lifeTime_ = RandomFloat(4.0f, 5.0f);
	emitterLifeTime_ = 100.0f;

	isDead_ = true;

	isRangeDraw_ = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Update() {
	frequencyTime_ += kDeltaTime_;				// 時刻を進める
	emitterLifeTime_ -= GameTimer::DeltaTime();	// Emitterの生存時間を短くする

	if (frequency_ <= frequencyTime_) { // 頻度より大きいなら発生
		CreateParticle();// 発生処理
		frequencyTime_ = 0;// 余計に過ぎた時間も紙した頻度計算する
	}

	// 生存時間が0になったら死亡フラグを立てる
	if (emitterLifeTime_ < 0.0f) {
		isDead_ = true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Draw(const Matrix4x4& viewProjection) {
	if (isRangeDraw_) {
		Vector3 center = transform_.translate;
		Vector3 size = size_;
		std::array<Vector3, 8> point = {
			Vector3{center.x - size.x,center.y + size.y, center.z - size.z }, // front_LT
			Vector3{center.x + size.x,center.y + size.y, center.z - size.z }, // front_RT
			Vector3{center.x + size.x,center.y - size.y, center.z - size.z }, // front_RB
			Vector3{center.x - size.x,center.y - size.y, center.z - size.z }, // front_LB
			Vector3{center.x - size.x,center.y + size.y, center.z + size.z }, // back_LT
			Vector3{center.x + size.x,center.y + size.y, center.z + size.z }, // back_RT
			Vector3{center.x + size.x,center.y - size.y, center.z + size.z }, // back_RB
			Vector3{center.x - size.x,center.y - size.y, center.z + size.z }, // back_LB
		};

		for (uint32_t oi = 0; oi < 4; oi++) {
			Render::DrawLine(point[oi], point[(oi + 1) % 4], { 1,0,0,1 }, viewProjection);
			uint32_t j = oi + 4;
			Render::DrawLine(point[j], point[(j + 1) % 4 + 4], { 1,0,0,1 }, viewProjection);
			Render::DrawLine(point[oi], point[j], { 1,0,0,1 }, viewProjection);
		}
	}
}


void Emitter::CreateParticle() {
	// 生成時に必要なデータを生成する
	BaseEffect::ParticleCreateData data(transform_, size_, count_, Normalize(firstVelocity_), lifeTime_);
	// リストにパーティクルを追加する
	effect_->AddParticleList(data);
}

void Emitter::Load(const std::string& fileName) {
	// 読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath_ + fileName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + fileName + ".json";
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	for (auto& [topKey, nestedData] : root.items()) {
		if (nestedData.contains("scale")) {
			transform_.scale = { nestedData["scale"][0], nestedData["scale"][1], nestedData["scale"][2] };
		}
		if (nestedData.contains("rotate")) {
			transform_.rotate = { nestedData["rotate"][0], nestedData["rotate"][1], nestedData["rotate"][2] };
		}
		if (nestedData.contains("position")) {
			transform_.translate = { nestedData["position"][0], nestedData["position"][1], nestedData["position"][2] };
		}
		if (nestedData.contains("size")) {
			size_ = { nestedData["size"][0], nestedData["size"][1], nestedData["size"][2] };
		}
		if (nestedData.contains("firstVelocity")) {
			firstVelocity_ = { nestedData["firstVelocity"][0], nestedData["firstVelocity"][1], nestedData["firstVelocity"][2] };
		}
		if (nestedData.contains("acceleration")) {
			acceleration_ = { nestedData["acceleration"][0], nestedData["acceleration"][1], nestedData["acceleration"][2] };
		}
		if (nestedData.contains("emitCount")) {
			count_ = nestedData["emitCount"];
		}
		if (nestedData.contains("frequency")) {
			frequency_ = nestedData["frequency"];
		}
		if (nestedData.contains("lifeTime")) {
			lifeTime_ = nestedData["lifeTime"];
		}
		if (nestedData.contains("emitteLifeTime")) {
			emitterLifeTime_ = nestedData["emitteLifeTime"];
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug時のみ使用
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void Emitter::Debug_Gui() {
	if (ImGui::TreeNode("Emitter")) {
		// -------------------------------------------------
		// ↓ Emitterに関する情報
		// -------------------------------------------------
		if (ImGui::Button("emit")) {
			CreateParticle();
		}
		if (ImGui::TreeNode("transform")) {
			ImGui::DragFloat3("scale", &transform_.scale.x, 0.01f);
			ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.01f);
			ImGui::DragFloat3("translation", &transform_.translate.x, 0.01f);
			ImGui::TreePop();
		}
		ImGui::DragFloat3("size", &size_.x, 0.01f);
		ImGui::DragScalar("count", ImGuiDataType_U32, &count_, 1);
		ImGui::DragFloat("frequency", &frequency_, 0.1f);
		ImGui::DragFloat("lifeTime", &lifeTime_, 0.1f);
		ImGui::DragFloat("emitterLifeTime", &emitterLifeTime_, 0.1f);

		ImGui::Checkbox("RangeDraw", &isRangeDraw_);
		ImGui::Separator();

		// -------------------------------------------------
		// ↓ Emitterの読み込み
		// -------------------------------------------------
		if (ImGui::Button("Load")) {
			Load(loadFileName_);
		}
		ImGui::SameLine();
		ImGui::InputText(".json##LoadFineName", &loadFileName_[0], sizeof(char) * 64);

		ImGui::Separator();
		// -------------------------------------------------
		// ↓ Emitterの保存を行う
		// -------------------------------------------------
		if (ImGui::Button("save")) {
			Save();
		}
		ImGui::SameLine();
		ImGui::InputText(".json##SaveFineName", &exportFileName_[0], sizeof(char) * 64);

		ImGui::TreePop();
	}
}

// EmitterのSave
void Emitter::Save() {
	json root;

	json pos = json::array({ transform_.translate.x,  transform_.translate.y,  transform_.translate.z });
	json rotateJoson = json::array({ transform_.rotate.x, transform_.rotate.y, transform_.rotate.z });
	json scaleJson = json::array({ transform_.scale.x, transform_.scale.y, transform_.scale.z });
	json size = json::array({ size_.x,  size_.y, size_.z });
	json firstVelocity = json::array({ firstVelocity_.x,  firstVelocity_.y, firstVelocity_.z });
	json acceleration = json::array({ acceleration_.x, acceleration_.y, acceleration_.z });

	root[exportFileName_] = {
		{"position",pos },
		{"rotate", rotateJoson},
		{"scale", scaleJson},
		{"size",size},
		{"firstVelocity", firstVelocity},
		{"acceleration", acceleration},
		{"emitCount", count_ },
		{"frequency", frequency_},
		{"lifeTime", lifeTime_},
		{"emitteLifeTime", emitterLifeTime_}
	};

	// -------------------------------------------------
	// ↓ 実際にファイルを保存する
	// -------------------------------------------------
	std::string filePath = kDirectoryPath_ + exportFileName_ + ".json";
	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープンが出来ているか
	if (!ofs.fail()) {
		assert("not open File");
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl; // rootにあるデータをjson文字列に変換してファイルへ
	// 閉じる
	ofs.close();
}
#endif
