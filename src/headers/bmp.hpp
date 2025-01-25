#pragma once
#include <cstdint>
#include <array>
#include <vector>

struct PixelArray
{
	const uint8_t AverageRed;
	const uint8_t AverageGreen;
	const uint8_t AverageBlue;
	const uint8_t AverageIntensity;

	PixelArray(const uint8_t _avgred, const uint8_t _avggreen, const uint8_t _avgblue, const uint8_t _avgint);
};

struct BMPPixel
{
	const int RowsPerArray;
	const int ColumnsPerArray;

	int pixelArrayRows = 0;
	int pixelArrayColumns = 0;

	const int32_t  width_px;
	const int32_t  height_px;

	std::vector<PixelArray> PixelArrayData;

	BMPPixel(const std::vector<uint8_t>& RawData, const int _RowsPerArray, const int _ColumnsPerArray);
};

#pragma region deprecated

/*typedef struct {
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
} BMPHeader;//deprecated

struct BMP {
	BMPHeader header;
	std::vector<uint8_t> BMPData;

	BMP(const std::vector<uint8_t>& RawData);
};*/

#pragma endregion