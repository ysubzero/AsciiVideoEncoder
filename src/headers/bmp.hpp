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

	BMP(const std::vector<uint8_t>& RawData)
	{
		header.type = static_cast<uint16_t>(RawData[0]) | (static_cast<uint16_t>(RawData[1]) << 8);
		header.size = static_cast<uint32_t>(RawData[2]) | (static_cast<uint32_t>(RawData[3]) << 8) |
			(static_cast<uint32_t>(RawData[4]) << 16) | (static_cast<uint32_t>(RawData[5]) << 24);
		header.reserved1 = static_cast<uint16_t>(RawData[6]) | (static_cast<uint16_t>(RawData[7]) << 8);
		header.reserved2 = static_cast<uint16_t>(RawData[8]) | (static_cast<uint16_t>(RawData[9]) << 8);
		header.offset = static_cast<uint32_t>(RawData[10]) | (static_cast<uint32_t>(RawData[11]) << 8) |
			(static_cast<uint32_t>(RawData[12]) << 16) | (static_cast<uint32_t>(RawData[13]) << 24);
		header.dib_header_size = static_cast<uint32_t>(RawData[14]) | (static_cast<uint32_t>(RawData[15]) << 8) |
			(static_cast<uint32_t>(RawData[16]) << 16) | (static_cast<uint32_t>(RawData[17]) << 24);
		header.width_px = static_cast<int32_t>(RawData[18]) | (static_cast<int32_t>(RawData[19]) << 8) |
			(static_cast<int32_t>(RawData[20]) << 16) | (static_cast<int32_t>(RawData[21]) << 24);
		header.height_px = static_cast<int32_t>(RawData[22]) | (static_cast<int32_t>(RawData[23]) << 8) |
			(static_cast<int32_t>(RawData[24]) << 16) | (static_cast<int32_t>(RawData[25]) << 24);
		header.num_planes = static_cast<uint16_t>(RawData[26]) | (static_cast<uint16_t>(RawData[27]) << 8);
		header.bits_per_pixel = static_cast<uint16_t>(RawData[28]) | (static_cast<uint16_t>(RawData[29]) << 8);
		header.compression = static_cast<uint32_t>(RawData[30]) | (static_cast<uint32_t>(RawData[31]) << 8) |
			(static_cast<uint32_t>(RawData[32]) << 16) | (static_cast<uint32_t>(RawData[33]) << 24);
		header.image_size_bytes = static_cast<uint32_t>(RawData[34]) | (static_cast<uint32_t>(RawData[35]) << 8) |
			(static_cast<uint32_t>(RawData[36]) << 16) | (static_cast<uint32_t>(RawData[37]) << 24);
		header.x_resolution_ppm = static_cast<int32_t>(RawData[38]) | (static_cast<int32_t>(RawData[39]) << 8) |
			(static_cast<int32_t>(RawData[40]) << 16) | (static_cast<int32_t>(RawData[41]) << 24);
		header.y_resolution_ppm = static_cast<int32_t>(RawData[42]) | (static_cast<int32_t>(RawData[43]) << 8) |
			(static_cast<int32_t>(RawData[44]) << 16) | (static_cast<int32_t>(RawData[45]) << 24);
		header.num_colors = static_cast<uint32_t>(RawData[46]) | (static_cast<uint32_t>(RawData[47]) << 8) |
			(static_cast<uint32_t>(RawData[48]) << 16) | (static_cast<uint32_t>(RawData[49]) << 24);
		header.important_colors = static_cast<uint32_t>(RawData[50]) | (static_cast<uint32_t>(RawData[51]) << 8) |
			(static_cast<uint32_t>(RawData[52]) << 16) | (static_cast<uint32_t>(RawData[53]) << 24);

		for (int i = 54; i < RawData.size(); i++)
		{
			BMPData.push_back(RawData[i]);
		}
	}
};

struct Pixel
{
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
	uint8_t Intensity;

	Pixel(uint8_t _Red, uint8_t _Green, uint8_t _Blue):
		Red(_Red),
		Green(_Green),
		Blue(_Blue)
	{
		Intensity = static_cast<uint8_t>(0.2126 * Red + 0.7152 * Green + 0.0722 * Blue);
	}

	friend std::ostream& operator<<(std::ostream& os, const Pixel& p);
};

std::ostream& operator<<(std::ostream& os, const Pixel& p)
{
	os << "R:" << static_cast<int>(p.Red) << " G:" << static_cast<int>(p.Green) << " B:" << static_cast<int>(p.Blue);
	return os;
}

struct PixelArray
{
	std::vector<Pixel> PixelMap;

	uint8_t AverageRed;
	uint8_t AverageGreen;
	uint8_t AverageBlue;
	uint8_t AverageIntensity;

	PixelArray(std::vector<Pixel>& _PixelMap) :
		PixelMap(_PixelMap),
		AverageRed(0),
		AverageGreen(0),
		AverageBlue(0),
		AverageIntensity(0)
	{
		if (PixelMap.empty()) return;
		uint32_t SumRed = 0;
		uint32_t SumGreen = 0;
		uint32_t SumBlue = 0;
		uint32_t SumIntensity = 0;

		for (int i = 0; i < PixelMap.size(); i++)
		{
			SumRed += PixelMap[i].Red;
			SumGreen += PixelMap[i].Green;
			SumBlue += PixelMap[i].Blue;
			SumIntensity += PixelMap[i].Intensity;
		}

		AverageRed = SumRed / PixelMap.size();
		AverageGreen = SumGreen / PixelMap.size();
		AverageBlue = SumBlue / PixelMap.size();
		AverageIntensity = SumIntensity / PixelMap.size();
	}
};

struct BMPPixel
{
	BMP bmp;

	std::vector<uint8_t> CleanData;
	std::vector<Pixel> PixelData;
	std::vector<PixelArray> PixelArrayData;

	const int RowsPerArray;
	const int ColumnsPerArray;

	uint32_t pixelArrayRows = 0;
	uint32_t pixelArrayColumns = 0;

	BMPPixel(BMP& _bmp, const int _RowsPerArray = 8, const int _ColumnsPerArray = 16) :
		bmp(_bmp),
		RowsPerArray(_RowsPerArray),
		ColumnsPerArray(_ColumnsPerArray)
	{
		std::vector<uint8_t> TempData = bmp.BMPData;

		uint64_t bytesperpixel = bmp.header.bits_per_pixel / 8;

		uint64_t rowWidth = bmp.header.width_px * bytesperpixel;
		uint64_t padding = 0;
		if (rowWidth % 4 != 0)
		{
			padding = 4 - (rowWidth % 4);
			rowWidth += padding;
		}
		for (int i = 0; i < bmp.header.height_px; i++)
		{
			std::vector<uint8_t> rowData;
			for (int j = 0; j < rowWidth - padding; j++)
			{
				rowData.push_back(TempData[j]);
			}
			TempData.erase(TempData.begin(), TempData.begin() + rowWidth);
			CleanData.insert(CleanData.end(), rowData.begin(), rowData.end());
		}
		for (int i = CleanData.size() - 1; i >= 2; i -= bytesperpixel)
		{
			Pixel pix(CleanData[i], CleanData[i - 1], CleanData[i - 2]);
			PixelData.push_back(pix);
		}
		for (int i = 0; i + ColumnsPerArray <= bmp.header.height_px; i += ColumnsPerArray)
		{
			for (int j = i * bmp.header.width_px; j + RowsPerArray <= i * bmp.header.width_px + bmp.header.width_px; j += RowsPerArray)
			{
				std::vector<Pixel> PixArray;
				PixArray.push_back(PixelData[j]);

				for (int k = 0; k < RowsPerArray; k++)
				{
					for (int l = 0; l < 16; l++)
					{
						if (j + (l * bmp.header.width_px) + k < PixelData.size())
						{
							PixArray.push_back(PixelData[j + (l * bmp.header.width_px) + k]);
						}
					}
				}

				PixelArray PixArr(PixArray);

				PixelArrayData.push_back(PixArr);
				pixelArrayColumns++;
			}
			pixelArrayRows++;
		}
		pixelArrayColumns /= pixelArrayRows;
	}
};