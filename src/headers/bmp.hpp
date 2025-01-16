#pragma once
#include <iostream>
#include <cstdint>
#include <array>
#include <vector>

typedef struct {
	uint16_t  type;
	uint32_t  size;
	uint16_t  reserved1;
	uint16_t  reserved2;
	uint32_t  offset;
	uint32_t  dib_header_size;
	int32_t   width_px;
	int32_t   height_px;
	uint16_t  num_planes;
	uint16_t  bits_per_pixel;
	uint32_t  compression;
	uint32_t  image_size_bytes;
	int32_t   x_resolution_ppm; 
	int32_t   y_resolution_ppm;
	uint32_t  num_colors;
	uint32_t  important_colors;
} BMPHeader;

struct BMP {
	BMPHeader header;
	std::vector<uint8_t> BMPData;

	BMP(const std::vector<uint8_t>& RawData);
};

struct Pixel
{
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
	Pixel(uint8_t _Red, uint8_t _Green, uint8_t _Blue);

	friend std::ostream& operator<<(std::ostream& os, const Pixel& p);

};

std::ostream& operator<<(std::ostream& os, const Pixel& p);

struct PixelArray
{
	std::vector<Pixel> PixelMap;

	uint8_t AverageRed;
	uint8_t AverageGreen;
	uint8_t AverageBlue;
	uint8_t AverageIntensity;

	PixelArray(std::vector<Pixel>& _PixelMap);

	PixelArray(std::vector<Pixel>& _PixelMap, uint8_t _avgred, uint8_t _avggreen, uint8_t _avgblue, uint8_t _avgint);
};

struct BMPPixel
{
	BMP bmp;

	std::vector<uint8_t> CleanData;
	std::vector<Pixel> PixelData;
	std::vector<PixelArray> PixelArrayData;

	const int RowsPerArray;
	const int ColumnsPerArray;

	int pixelArrayRows = 0;
	int pixelArrayColumns = 0;

	BMPPixel(BMP& _bmp, const int _RowsPerArray, const int _ColumnsPerArray);
};