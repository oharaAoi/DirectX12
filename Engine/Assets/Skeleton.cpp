#include "Skeleton.h"
#include "Engine/GameObject/BaseGameObject.h"

Skeleton::Skeleton() {}
Skeleton::~Skeleton() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Skeleton::Init() {
	for (size_t oi = 0; oi < joints_.size(); ++oi) {
		auto& instance = obj_.emplace_back(std::make_unique<BaseGameObject>());
		instance->Init();
		instance->SetObject("SquarePyramid.obj");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Skeleton::Update() {
	uint32_t oi = 0;
	for (Joint& joint : joints_) {
		joint.localMat = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);

		if (joint.parent) {
			joint.skeltonSpaceMat = joint.localMat * joints_[*joint.parent].skeltonSpaceMat;
		} else {
			joint.skeltonSpaceMat = joint.localMat;
		}

		obj_[oi]->GetTransform().Update(joint.skeltonSpaceMat);
		oi++;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Skeleton::Draw() const {
	for (int32_t oi = 0; oi < joints_.size(); ++oi) {
		obj_[oi]->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　skeletonの作成
//////////////////////////////////////////////////////////////////////////////////////////////////

void Skeleton::CreateSkeleton(const Model::Node& node) {
	root_ = CreateJoint(node, {}, joints_);

	// 名前からIndexを検索可能に
	for (const Joint& joint : joints_) {
		jointMap_.emplace(joint.name, joint.index);
	}

	//Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　再帰的にbornの情報を取り込む
//////////////////////////////////////////////////////////////////////////////////////////////////

int32_t Skeleton::CreateJoint(const Model::Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints) {
	Joint joint;
	joint.name = node.name;
	joint.localMat = node.localMatrix;
	joint.skeltonSpaceMat = MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints_.size());	// 登録されている数
	joint.parent = parent;
	joints.push_back(std::move(joint));	// skeltonのjoint列に追加
	for (const Model::Node& child : node.children) {
		// 子のjointを作成し、そのIndexを登録
		int32_t chileIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(chileIndex);
	}

	return joint.index;
}
