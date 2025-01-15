#pragma once
#include "AsciiVideoEncoder.hpp"

void clearScreen();

namespace ASC
{
	void consoleASCII(BMPPixel& bmppixel);

	void FileToTxt(std::string inputfile, std::string outputname, int detail_x, int detail_y);

	void BMPToAsciiImage(BMPPixel& bmppixel, std::string path);

	void FileToAsciiImage(std::string filename, std::string outfilename, int detail_x, int detail_y);

	void FileToConsole(std::string filename, int detail_x, int detail_y);
}

