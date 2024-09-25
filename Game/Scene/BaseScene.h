#pragma once

/// <summary>
/// BaseとなるScene
/// </summary>
class BaseScene {
public:

	BaseScene() = default;
	virtual ~BaseScene() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() const = 0;

};

