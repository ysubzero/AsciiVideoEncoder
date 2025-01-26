#pragma once
#include "AsciiVideoEncoder.hpp"

void clearScreen();

namespace ASC
{
	void Text(const std::string& inputfile, const std::string& outputname, const int detail_x, const int detail_y);

	void BMPAscii(const BMPPixel& bmppixel, const std::string& path);

	void BMPColor(const BMPPixel& bmppixel, const std::string& path);

	void AsciiImage(const std::string& filename, const std::string& outfilename, const int detail_x, const int detail_y);

	void ColorImage(const std::string& filename, const std::string& outfilename, const int detail_x, const int detail_y);

	std::string Console(const std::string& filename, const int detail_x, const int detail_y);

	std::string ColorConsole(const std::string& filename, const int detail_x, const int detail_y);
}

