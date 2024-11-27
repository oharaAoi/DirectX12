#include "BossController.h"
#include "Engine/Math/Easing.h"

BossController::BossController() {
}

BossController::~BossController() {
}

void BossController::Init(Boss* boss) {
	boss_ = boss;

	boss_->GetBossLeftHand()->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3::FORWARD()));
	boss_->GetBossRightHand()->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3(0.0f, 0.0f, 1.0f).Normalize()) *
															 boss_->GetBossRightHand()->GetLocalQuaternion());

	title_ = std::make_unique<BaseGameObject>();
	retry_ = std::make_unique<BaseGameObject>();

	title_->Init();
	retry_->Init();

	title_->SetObject("title.obj");
	retry_->SetObject("retry.obj");

	title_->GetTransform()->SetTranslaion(Vector3(0.0f, 3.2f, 0.0f));
	retry_->GetTransform()->SetTranslaion(Vector3(1.0f, 3.2f, 0.0f));

	title_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3::UP()));
	retry_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3::UP()));

	leftHand_ = boss_->GetBossLeftHand();
	rightHand_ = boss_->GetBossRightHand();

	title_->GetTransform()->SetParentTranslation(rightHand_->GetWorldPos());
	retry_->GetTransform()->SetParentTranslation(leftHand_->GetWorldPos());

	rightHandInitPos_ = boss_->GetBossRightHand()->GetTransform()->GetTranslation();
	leftHandInitPos_ = boss_->GetBossLeftHand()->GetTransform()->GetTranslation();

	preSelect_ = false;

	moveTitleZ_ = 0.0f;
	moveRetryZ_ = 0.0f;

	moveTime_ = 0.0f;
	moveTimeLimit_ = 0.3f;

	isMove_ = false;
	isRightMove_ = true;

	titleMoveRotate_ = 0.0f;
	retryMoveRotate_ = 0.0f;

	titleRotate_ = Quaternion();
	retryRotate_ = Quaternion();
}

void BossController::Update(bool goTitle) {
	// タイトルright　/ リトライleft
	if (goTitle != preSelect_) {
		isMove_ = true;
		moveTime_ = 0.0f;

		if (goTitle) {	// 右手を前に出す
			isRightMove_ = true;
		} else {
			isRightMove_ = false;
		}
	}

	titleMoveRotate_ = GameTimer::DeltaTime();
	retryMoveRotate_ = GameTimer::DeltaTime();

	if (isMove_) {
		moveTime_ += GameTimer::DeltaTime();

		// 移動させる
		if (isRightMove_) {	// 右手を前に出す
			float titleT = moveTime_ / moveTimeLimit_;
			moveTitleZ_ = std::lerp(0.0f, moveSpeed_, Ease::InOut::Cubic(titleT));

			float retryT = 1 - titleT;
			moveRetryZ_ = std::lerp(0.0f, moveSpeed_, Ease::InOut::Cubic(retryT));

			retryMoveRotate_ = 45.0f * toRadian;

		} else {		// 左手を前に出す
			float retryT = moveTime_ / moveTimeLimit_;
			moveRetryZ_ = std::lerp(0.0f, moveSpeed_, Ease::InOut::Cubic(retryT));

			float titleT = 1 - retryT;
			moveTitleZ_ = std::lerp(0.0f, moveSpeed_, Ease::InOut::Cubic(titleT));

			titleMoveRotate_ = 45.0f * toRadian;
		}

		// 時間範囲
		if (moveTime_ >= moveTimeLimit_) {
			isMove_ = false;

			if (isRightMove_) {
				retryMoveRotate_ = 180.0f * toRadian;
			} else {
				titleMoveRotate_ = 180.0f * toRadian;
			}
		}
	}

	Vector3 rightPos = rightHandInitPos_;
	rightPos.z += moveTitleZ_;

	Vector3 leftPos = leftHandInitPos_;
	leftPos.z += moveRetryZ_;

	titleRotate_ = Quaternion::AngleAxis(titleMoveRotate_, Vector3::UP());
	retryRotate_ = Quaternion::AngleAxis(retryMoveRotate_, Vector3::UP());

	leftHand_->GetTransform()->SetTranslaion(leftPos);
	rightHand_->GetTransform()->SetTranslaion(rightPos);

	title_->GetTransform()->SetQuaternion(titleRotate_ * title_->GetTransform()->GetQuaternion());
	retry_->GetTransform()->SetQuaternion(retryRotate_ * retry_->GetTransform()->GetQuaternion());

	preSelect_ = goTitle;
}

void BossController::PostUpdate() {
	title_->Update();
	retry_->Update();
}

void BossController::Draw() const {
	title_->Draw();
	retry_->Draw();

}

#ifdef _DEBUG
void BossController::Debug_Gui() {
	ImGui::Begin("BossController");
	if (ImGui::TreeNode("title")) {
		title_->Debug_Gui();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("retry")) {
		retry_->Debug_Gui();
		ImGui::TreePop();
	}
	ImGui::End();
}
#endif