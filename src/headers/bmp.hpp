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