#pragma once
#include "headers/AsciiVideoEncoder.hpp"

void clearScreen();

namespace ASC
{
	void consoleASCII(const BMPPixel& bmppixel);

	void FileToTxt(const std::string inputfile, const std::string outputname, const int detail_x, const int detail_y);

	void BMPToAsciiImage(const BMPPixel& bmppixel, const std::string path);

	void FileToAsciiImage(const std::string filename, const std::string outfilename, const int detail_x, const int detail_y);

	void FileToConsole(const std::string filename, const int detail_x, const int detail_y);
}

