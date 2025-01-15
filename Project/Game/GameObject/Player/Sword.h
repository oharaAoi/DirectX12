#pragma once
#include <memory>
#include "Engine/Assets/GameObject/BaseGameObject.h"

class Sword : 
	public BaseGameObject {
public:

	Sword();
	~Sword();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	BaseGameObject* GetSwordTip() { return swordTip_.get(); }
	BaseGameObject* GetSwordRoot() { return swordRoot_.get(); }

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	Vector3 swordRotate_;
	Vector3 swordRootOffset_ = { -4.5f, 0.0f, 0.0f };
	Vector3 swordTipOffset_ = { -0.5f, 0.0f, 0.0f };

	std::unique_ptr<BaseGameObject> swordTip_;
	std::unique_ptr<BaseGameObject> swordRoot_;

};

