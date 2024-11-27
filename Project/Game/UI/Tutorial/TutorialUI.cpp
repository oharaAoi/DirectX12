#include "TutorialUI.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Math/Easing.h"

TutorialUI::TutorialUI() {
}

TutorialUI::~TutorialUI() {
}

void TutorialUI::Init() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	for (uint32_t oi = 0; oi < kGuideMax_; ++oi) {
		gaide_[oi] = std::make_unique<BaseGameObject>();
		gaide_[oi]->Init();
		gaide_[oi]->SetObject("plane.obj");
		gaide_[oi]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3::UP()));
		gaide_[oi]->GetTransform()->SetScale(Vector3(0.0f, 0.0f, 0.0f));

		std::string id = "pos" + std::to_string(oi);
		adjust->AddItem("TutorialUI", id.c_str(), gaide_[oi]->GetTransform()->GetTranslation());
	}

	gaide_[0]->SetTexture("tutorial_01.png");
	gaide_[1]->SetTexture("tutorial_02.png");
	gaide_[2]->SetTexture("tutorial_03.png");
	gaide_[3]->SetTexture("tutorial_04.png");

	gaide_[0]->GetTransform()->SetScale(Vector3(3.0f, 3.0f, 1.0f));

	AdaptItem();

	nearIndex_ = 0;

	isChange_ = false;
}

void TutorialUI::Update(const Vector3& playerPos) {
	float minLength = 9999.0f;
	for (uint32_t oi = 0; oi < kGuideMax_; ++oi) {

		float length = (gaide_[oi]->GetWorldPos() - playerPos).Length();
		if (length < minLength) {
			nearIndex_ = oi;
			minLength = length;
		}

		gaide_[oi]->Update();
	}

	if (nearIndex_ != preNearIndex) {
		isChange_ = true;
	}

	if (isChange_) {
		PopUp();
	} else {
		preNearIndex = nearIndex_;
	}
}

void TutorialUI::Draw() const {
	for (uint32_t oi = 0; oi < kGuideMax_; ++oi) {
		gaide_[oi]->Draw();
	}
}

void TutorialUI::AdaptItem() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	for (uint32_t oi = 0; oi < kGuideMax_; ++oi) {
		std::string id = "pos" + std::to_string(oi);
		gaide_[oi]->GetTransform()->SetTranslaion(adjust->GetValue<Vector3>("TutorialUI", id));
	}
}

void TutorialUI::PopUp() {
	popUpTime_ += GameTimer::DeltaTime();
	float t = popUpTime_ / popUpTimeLimit_;
	t = std::clamp(t, 0.0f, 1.0f);
	gaide_[nearIndex_]->GetTransform()->SetScale(Vector3::Lerp(Vector3(0.0f, 0.0f, 1.0f), Vector3(3.0f, 3.0f, 1.0f), Ease::Out::Back(t)));
	gaide_[preNearIndex]->GetTransform()->SetScale(Vector3::Lerp(Vector3(3.0f, 3.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Ease::In::Back(t)));

	if (popUpTime_ >= popUpTimeLimit_) {
		isChange_ = false;
		preNearIndex = nearIndex_;
		popUpTime_ = 0.0f;
	}
}


#ifdef _DEBUG
void TutorialUI::Debug_Gui() {
	ImGui::Begin("TutorialUI");
	for (uint32_t oi = 0; oi < kGuideMax_; ++oi) {
		std::string id = "gaide" + std::to_string(oi);
		if (ImGui::TreeNode(id.c_str())) {
			gaide_[oi]->Debug_Gui();
			ImGui::TreePop();
		}

		if (ImGui::Button("Adapt")) {
			AdaptItem();
		}
	}
	ImGui::End();
}
#endif