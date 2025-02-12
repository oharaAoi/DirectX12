#pragma once
#include "Game/Camera/BaseCamera.h"
#include "Engine/Components/Attribute/AttributeGui.h"

/// <summary>
/// 3d空間上に配置するカメラ
/// </summary>
class Camera3d :
	public BaseCamera,
	public AttributeGui{
public:

	Camera3d();
	~Camera3d() override;

	void Finalize() override;
	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

private:

};

