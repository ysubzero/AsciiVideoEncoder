#include "headers/filesys.hpp"

std::vector<std::string> FSys::OutputFiles(const std::string& directory)
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

void FSys::deleteTemporary(const std::string& directory)
{
	try
	{
		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (fs::is_regular_file(entry) && entry.path().extension() == ".bmp")
			{
				fs::remove(entry.path());
			}
		}
	}
	catch (const fs::filesystem_error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}