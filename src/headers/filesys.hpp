#pragma once
#include <iostream>
#include <filesystem>
#include <regex>
#include <vector>
#include <string>

namespace fs = std::filesystem;

namespace FSys
{
	std::vector<std::string> OutputFiles(const std::string& directory);

	void deleteTemporary(const std::string& directory);
}
