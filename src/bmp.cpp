#include "headers/bmp.hpp"

BMP::BMP(const std::vector<uint8_t>& RawData)
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


	constexpr uint64_t bytesperpixel = 3;

	uint64_t rowWidth = header.width_px * bytesperpixel;
	const uint64_t padding = rowWidth % 4 != 0 ? 4 - (rowWidth % 4) : 0;
	rowWidth += padding;

	{
		uint64_t iter = 0;
		BMPData.resize(header.height_px * (rowWidth - padding));
		for (uint64_t i = 0; i < header.height_px; ++i)
		{
			for (uint64_t j = i * rowWidth; j < (i * rowWidth) + rowWidth - padding; ++j)
			{
				BMPData[iter] = RawData[j+54];
				++iter;
			}
		}
	}
}//deprecated

PixelArray::PixelArray(const uint8_t _avgred, const uint8_t _avggreen, const uint8_t _avgblue, const uint8_t _avgint) :
	AverageRed(_avgred),
	AverageGreen(_avggreen),
	AverageBlue(_avgblue),
	AverageIntensity(_avgint)
{}

BMPPixel::BMPPixel(const std::vector<uint8_t>& RawData, const int _RowsPerArray, const int _ColumnsPerArray) :
	width_px(static_cast<int32_t>(RawData[18]) | (static_cast<int32_t>(RawData[19]) << 8) |
		(static_cast<int32_t>(RawData[20]) << 16) | (static_cast<int32_t>(RawData[21]) << 24)),
	height_px(static_cast<int32_t>(RawData[22]) | (static_cast<int32_t>(RawData[23]) << 8) |
		(static_cast<int32_t>(RawData[24]) << 16) | (static_cast<int32_t>(RawData[25]) << 24)),
	RowsPerArray(_RowsPerArray),
	ColumnsPerArray(_ColumnsPerArray)
{
	constexpr uint64_t bytesperpixel = 3;

	const uint64_t rowWidth = width_px * bytesperpixel;
	const uint64_t padding = rowWidth % 4 != 0 ? 4 - (rowWidth % 4) : 0;

	for (int i = height_px - 1; i >= 0; i -= ColumnsPerArray)
	{
		const int startpixel = ((i + 1) * width_px) * 3 + i * padding + 53;
		const int endpixel = (i * width_px + RowsPerArray - 1) * 3 + i * padding + 54;
		for (int j = startpixel; j >= endpixel; j -= RowsPerArray * 3)
		{
			int id = 1;

			uint32_t SumRed = RawData[j];
			uint32_t SumGreen = RawData[j - 1];
			uint32_t SumBlue = RawData[j - 2];

			for (int k = 0; k < RowsPerArray; ++k) // x from j (origin)
			{
				for (int l = 0; l < ColumnsPerArray; ++l) // y from j (origin)
				{
					const int idx = j - (((l * width_px) + k) * 3) - l * padding;
					if (idx == j || idx >= RawData.size() || idx < 56 || idx < ((i - l) * width_px + RowsPerArray - 1) * 3 + (i - l) * padding + 56)
					{
						continue;
					}
					SumRed += RawData[idx];
					SumGreen += RawData[idx - 1];
					SumBlue += RawData[idx - 2];
					++id;
				}
			}

			const uint8_t AverageRed = SumRed / id;
			const uint8_t AverageGreen = SumGreen / id;
			const uint8_t AverageBlue = SumBlue / id;
			const uint8_t AverageIntensity = static_cast<uint32_t>(0.2126 * SumRed + 0.7152 * SumGreen + 0.0722 * SumBlue) / id;

			PixelArrayData.emplace_back(PixelArray(AverageRed, AverageGreen, AverageBlue, AverageIntensity));
			++pixelArrayColumns;
		}
		++pixelArrayRows;
	}
	pixelArrayColumns /= pixelArrayRows;
}