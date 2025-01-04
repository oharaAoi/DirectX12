#pragma once
// c++
#include <vector>
// engine
#include "Engine/Lib/IJsonConverter.h"
#include "Engine/Assets/2d/Sprite.h"

class Score {

	struct ScoreParameter : public IJsonConverter {
		const std::string tag = "score";
		Vector2 targetPos;
		Vector2 scorePos;
		Vector2 slashPos;
		float interval;
		
		json ToJson(const std::string id) const override {
			return JsonBuilder(id)
				.Add("targetPos", targetPos)
				.Add("scorePos", scorePos)
				.Add("slashPos", slashPos)
				.Add("interval", interval)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "targetPos", targetPos);
			fromJson(jsonData, "scorePos", scorePos);
			fromJson(jsonData, "slashPos", slashPos);
			fromJson(jsonData, "interval", interval);
		}
	};

public:

	Score();
	~Score();

	void Init();
	void Update(uint32_t score);
	void Draw() const;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	static const uint32_t kMaxScoreDegit_ = 2;
	const uint32_t kMaxScore_ = 50;

	std::unique_ptr<Sprite> targetScore_[kMaxScoreDegit_];
	std::vector<std::unique_ptr<Sprite>> scores_;

	std::unique_ptr<Sprite> slash_;

	ScoreParameter scoreParameter_;

	Vector2 textureSize_ = { 64.0f, 64.0f };

};

