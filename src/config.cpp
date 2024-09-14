#include "config.hpp"
#include <fstream>

Config::Config(std::filesystem::path path) :
	path_(std::move(path)) {
	if (!load()) save();
}

Config::~Config() {
	save();
}

bool Config::load() {
	std::ifstream file(path_);
	if (!file.is_open()) return false;

	// Read whole file content
	std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	// Parse file content into json
	try {
		data_ = nlohmann::json::parse(file_content);
	} catch (...) {
		save();
	}
	return true;
}

void Config::save() const {
	std::ofstream file(path_);
	if (!file.is_open()) return;

	nlohmann::json json(data_);
	file << json.dump(4);
}

struct Config::Data *Config::operator->() {
	return &data_;
}

const struct Config::Data *Config::operator->() const {
	return &data_;
}