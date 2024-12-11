#include <iostream>
#include <filesystem>
#include <regex>
#include <vector>
#include <string>

namespace fs = std::filesystem;

namespace FSys
{
	std::vector<std::string> OutputFiles(const std::string& directory)
	{
		std::vector<std::string> files;
		std::regex filenamePattern(R"(^output_(\d+)\.bmp$)");

		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (fs::is_regular_file(entry.status()))
			{
				std::string filename = entry.path().filename().string();

				if (std::regex_match(filename, filenamePattern))
				{
					files.push_back(filename);
				}
			}
		}
		return files;
	}
}
