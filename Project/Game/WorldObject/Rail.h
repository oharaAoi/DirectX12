#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class Rail : public BaseGameObject {
public:

	Rail();
	~Rail();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

	void SetTransform(const Vector3& transform) { worldTransform_->SetTranslaion(transform); }

	void SetBottomVertex(Mesh::VertexData* vertexData);

	Mesh::VertexData* GetTopVertex();

private:

	std::unique_ptr<Mesh> mesh_;
	std::unique_ptr<Material> material_;

	std::vector<Mesh::VertexData> vertexData_;

	std::unique_ptr<WorldTransform> worldTransform_;

};

