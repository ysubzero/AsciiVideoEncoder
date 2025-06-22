#include "headers/bmp.hpp"

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

	PixelArrayData.reserve(static_cast<size_t>(ceil(static_cast<double>(height_px) / ColumnsPerArray) * ceil(static_cast<double>(width_px) / RowsPerArray)));

	for (int i = height_px - 1; i >= 0; i -= ColumnsPerArray)
	{
		const int startpixel = ((i + 1) * width_px) * 3 + i * padding + 53; // including padding and -1 to start at new row
		const int endpixel = (i * width_px + RowsPerArray - 1) * 3 + i * padding + 54; // stops before padding
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
					if (idx == j || idx >= RawData.size() || idx < 56 || idx < ((i - l) * width_px + RowsPerArray - 1) * 3 + (i - l) * padding + 56) //check if it will include a piece of padding, or invalid
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
