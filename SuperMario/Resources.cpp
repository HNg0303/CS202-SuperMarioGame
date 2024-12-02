#include "Resources.h"

string convertToUnixPath(const string& path) {
	std::string unixPath = path;
	std::replace(unixPath.begin(), unixPath.end(), '\\', '/');
	return unixPath;
}

void Resources::loadResource() {
	//load textures
	string texturePath = convertToUnixPath(fs::current_path().string() + "/Resource/Textures/");
	string marioPath = convertToUnixPath(fs::current_path().string() + "/Resource/Mario/");
	string lugiPath = convertToUnixPath(fs::current_path().string() + "/Resource/Luigi/");


	for (const auto& file : fs::directory_iterator(texturePath)) {
		if (file.is_regular_file() && (file.path().extension() == ".png" || file.path().extension() == ".jpg")) {
			string filePath = convertToUnixPath(file.path().string());
			
			if (!textures[file.path().filename().string()].loadFromFile(file.path().string())) {
				std::cout << "Loading from: " << file.path().string() << std::endl;
				std::cout << "Load texture failed\n";
			}
			else {
				std::cout << "Load texture succeed\n";
				cout << file.path().filename().string();
			}
		}
	}
	//load character Mario
	for (auto& file : fs::directory_iterator(marioPath)) {
		if (file.is_regular_file() && (file.path().extension() == ".png" || file.path().extension() == ".jpg")) {
			string filePath = convertToUnixPath(file.path().string());
			if (!textures[file.path().filename().string()].loadFromFile(file.path().string())) {
				std::cout << "Loading from: " << file.path().string() << std::endl;
				std::cout << "Load Mario failed\n";
			}
			else {
				std::cout << "Load Mario succeed\n";
			}
		}
	}
	//Load Character Luigi
	for (auto& file : fs::directory_iterator(lugiPath)) {
		if (file.is_regular_file() && (file.path().extension() == ".png" || file.path().extension() == ".jpg")) {
			string filePath = convertToUnixPath(file.path().string());
			if (!textures[file.path().filename().string()].loadFromFile(file.path().string())) {
				std::cout << "Load character Luigi failed\n";
			}
			else {
				std::cout << "Load character Luigi succeed\n";
			}
		}
	}
}

Texture Resources :: getTexture(string textureName) {
	return textures[textureName];
}