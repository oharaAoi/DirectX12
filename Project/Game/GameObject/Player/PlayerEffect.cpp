#include "PlayerEffect.h"
#include "Engine/Math/MyRandom.h"
#include "Engine/Math/Easing.h"

PlayerEffect::PlayerEffect() {
}

PlayerEffect::~PlayerEffect() {
}

void PlayerEffect::Init() {

	for (int i = 0; i < 40; i++) {
		std::unique_ptr<PlayersGrain> p;
		p = std::make_unique<PlayersGrain>();

		p->live_ = false;
		std::unique_ptr<BaseGameObject> newObj;
		newObj = std::make_unique<BaseGameObject>();
		p->grain_ = std::move(newObj);
		p->grain_->Init();
		p->grain_->SetObject("prediction.obj");



		fragument_.push_back(std::move(p));
	}

}

void PlayerEffect::Update() {

	for (auto& p : fragument_) {

		if (!p->live_) {
			continue;
		}
		if (p->lifeTime_ <= 0.0f) {
			p->live_ = false;
		}
		else {
			p->lifeTime_ -= GameTimer::DeltaTime();
			if (p->lifeTime_ < 0.0f) {
				p->lifeTime_ = 0.0f;
			}
		}


		float u = p->lifeTime_ / p->maxLifeTime_;
		float t = (1.0f - u);

		Vector3 pos = p->grain_->GetTransform()->GetTranslation();
		p->velocity_.x= std::lerp(p->startVelo_.x, p->endVelo_.x, Ease::Out::Quad(t));
		p->velocity_.y = std::lerp(p->startVelo_.y, p->endVelo_.y, Ease::Out::Quad(t));
		p->velocity_.z = std::lerp(p->startVelo_.z, p->endVelo_.z, Ease::Out::Quad(t));
		pos += p->velocity_ * GameTimer::DeltaTime();
		p->grain_->GetTransform()->SetTranslaion(pos);


		float xSize = std::lerp(p->startSize_.x, p->endSize_.x, Ease::Out::Quad(t));
		float ySize = std::lerp(p->startSize_.y, p->endSize_.y, Ease::Out::Quad(t));
		float zSize = std::lerp(p->startSize_.z, p->endSize_.z, Ease::Out::Quad(t));
		p->grain_->GetTransform()->SetScale({ xSize,ySize,zSize });

		Vector4 co = p->grain_->GetColor();
		p->grain_->SetColor({ co.x,co.y,co.z,Ease::In::Quad(u) });

		p->grain_->Update();

	}

}

void PlayerEffect::Draw() {
	for (auto& p : fragument_) {
		if (p->live_) {
			p->grain_->Draw();
		}
	}
}

void PlayerEffect::EmitFragment(Vector3 pos,Vector2 dir) {
	int count = 0;
	for (auto& p : fragument_) {
		if (p->live_) {
			continue;
		}

		float xRand = RandomFloat(8.0f, 12.0f);
		float yRand = RandomFloat(-4.0f, 4.0f);
		float zRand = RandomFloat(-10.0f, 20.0f);
		if (dir.x == -1) {

		}if (dir.x == 1) {
			xRand = RandomFloat(-12.0f, -8.0f);
			yRand = RandomFloat(-4.0f, 4.0f);
			zRand = RandomFloat(-10.0f, 20.0f);
		}
		if (dir.y == -1) {
			xRand = RandomFloat(-4.0f, 4.0f);
			yRand = RandomFloat(-12.0f, -8.0f);
			zRand = RandomFloat(-10.0f, 20.0f);
			p->grain_->SetColor({ 0.4f,0.4f,0.4f,1.0f });
		}


		p->maxLifeTime_ = 0.36f;
		p->lifeTime_ = 0.36f;

		if (count <10) {
			p->velocity_ = { -xRand,yRand,zRand };
			p->endVelo_ = { -xRand,yRand,zRand };
		}
		else {
			p->velocity_ = { xRand,yRand,zRand };
			p->endVelo_ = { xRand,yRand,zRand };
		}
		p->startVelo_ = { 0.0f,0.0f,0.0f };
		

		p->startSize_ = { 0.1f,0.1f,0.1f };
		p->endSize_ = { 0.8f,0.8f,0.8f };
		p->live_ = true;


		float angle = atan2(p->velocity_.y, p->velocity_.x);
		p->grain_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(angle, Vector3::FORWARD()));
		p->grain_->GetTransform()->SetTranslaion(pos);
		p->grain_->SetColor({ 1.0f,0.8f,0.0f,1.0f });

		p->grain_->Update();

		count++;
		if (count == 25) {
			return;
		}
	}

}

void PlayerEffect::EmitDown(Vector3 pos, Vector2 dir) {
	int count = 0;
	for (auto& p : fragument_) {
		if (p->live_) {
			continue;
		}

		float xRand = RandomFloat(8.0f, 12.0f);
		float yRand = RandomFloat(-4.0f, 4.0f);
		float zRand = RandomFloat(-10.0f, 20.0f);

		if (dir.y == -1) {
			xRand = RandomFloat(-6.0f, 6.0f);
			yRand = RandomFloat(-12.0f, -8.0f);
			zRand = RandomFloat(-1.0f, 16.0f);
			p->grain_->SetColor({ 0.4f,0.4f,0.4f,1.0f });
		}


		p->maxLifeTime_ = 0.36f;
		p->lifeTime_ = 0.36f;


		p->velocity_ = { -xRand,yRand,zRand };
		p->endVelo_ = { 0.0f,0.0f,0.0f };
		
		p->startVelo_ = { -xRand,yRand,zRand };


		p->startSize_ = { 0.1f,0.1f,0.1f };
		p->endSize_ = { 0.8f,0.8f,0.8f };
		p->live_ = true;


		float angle = atan2(p->velocity_.y, p->velocity_.x);
		p->grain_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(angle, Vector3::FORWARD()));
		p->grain_->GetTransform()->SetTranslaion(pos);
		p->grain_->SetColor({ 1.0f,0.8f,0.0f,1.0f });

		p->grain_->Update();

		count++;
		if (count == 10) {
			return;
		}
	}
}
