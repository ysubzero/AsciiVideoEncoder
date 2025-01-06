#pragma once
#include "headers/AsciiVideoEncoder.hpp"

namespace ASC
{
	void printASCII(BMPPixel& bmppixel);

	std::vector<std::string> ImageToAscii(BMPPixel& bmppixel);

	void AsciiToImage(std::vector<std::string>& strings, BMPPixel& bmppixel, std::string path);

	void BMPToAsciiImage(BMPPixel& bmppixel, std::string path);

	void ImageToAsciiToImage(BMPPixel& bmppixel, std::string path);

	void ImageToAsciiImage(BMPPixel& bmppixel, std::string path);

	void FileToAsciiImage(std::string filename, std::string outfilename, int detail_x, int detail_y);
}

