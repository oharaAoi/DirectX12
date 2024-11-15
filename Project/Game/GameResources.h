#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <Externals/nlohmann/json.hpp>

using json = nlohmann::json;

class GameResources {

public:

	GameResources();
	~GameResources();

	void Load();

	void LoadResourcesData(const std::string& directoryPath);
	void LoadResources(const std::string& filePath);

private:

	const std::string engineDirectoryPath_ = "./Engine/Resources/LoadData/";

};

