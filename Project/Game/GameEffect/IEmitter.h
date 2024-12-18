#pragma once
#include <memory>
#include "Engine/Engine.h"

class IEmitter {
public:

	virtual ~IEmitter() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() const = 0;

	void SetIsDead(bool isDead) { isDead_ = isDead; }
	const bool GetIsDead() const { return isDead_; }

protected:

	bool isDead_;

};

