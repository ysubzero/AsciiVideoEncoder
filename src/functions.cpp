#include "headers/functions.hpp"

static std::array<uint8_t, 24> getmap(const int j, const uint32_t intensity)
{
	switch (intensity)
	{
	case 0:
		return Cha::space_bit_map[j];
	case 1:
		return Cha::dot_bit_map[j];
	case 2:
		return Cha::colon_bit_map[j];
	case 3:
		return Cha::dash_bit_map[j];
	case 4:
		return Cha::equal_bit_map[j];
	case 5:
		return Cha::plus_bit_map[j];
	case 6:
		return Cha::hashtag_bit_map[j];
	default:
		return Cha::at_bit_map[j];
	}

}

void clearScreen() 
{
	std::print("\033[2J\033[H");
}

void ASC::Text(const std::string& filename, const std::string& outputname, const int detail_x, const int detail_y)
{
	std::ifstream input(filename, std::ios::binary);

	const std::vector<uint8_t> rawData(std::istreambuf_iterator<char>(input), {});

	const BMPPixel bmppixel(rawData, detail_x, detail_y);

	std::string result;
	result.reserve(bmppixel.pixelArrayRows * (bmppixel.pixelArrayColumns + 1));

	for (int row = 0; row < bmppixel.pixelArrayRows; ++row)
	{
		for (int i = (row + 1) * bmppixel.pixelArrayColumns - 1; i >= row * bmppixel.pixelArrayColumns; --i)
		{
			const uint8_t intensity = bmppixel.PixelArrayData[i].AverageIntensity / 32;

			switch (intensity)
			{
			case 0:
				result += " ";
				break;
			case 1:
				result += ".";
				break;
			case 2:
				result += ":";
				break;
			case 3:
				result += "-";
				break;
			case 4:
				result += "=";
				break;
			case 5:
				result += "+";
				break;
			case 6:
				result += "#";
				break;
			default:
				result += "@";
				break;
			}
		}
		result += "\n";
	}

	std::ofstream outfile(outputname);
	std::print(outfile, "{0}", result);
	outfile.close();
}

void ASC::BMPAscii(const BMPPixel& bmppixel, const std::string& path)
{
#pragma region Header Data
	constexpr uint16_t  type = 0x4D42;
	constexpr uint16_t  reserved1 = 0;
	constexpr uint16_t  reserved2 = 0;
	constexpr uint32_t  offset = 54;
	constexpr uint32_t  dib_header_size = 40;
	const int32_t   width_px = static_cast<int32_t>((bmppixel.PixelArrayData.size() / bmppixel.pixelArrayRows) * 8);
	const int32_t   height_px = static_cast<int32_t>((bmppixel.PixelArrayData.size() / bmppixel.pixelArrayColumns) * 16);
	constexpr uint16_t  num_planes = 1;
	constexpr uint16_t  bits_per_pixel = 24;
	constexpr uint32_t  compression = 0;
	const uint32_t  image_size_bytes = (bmppixel.pixelArrayColumns * 8) * (bmppixel.pixelArrayRows * 16) * 3;
	const uint32_t  size = image_size_bytes + 54;

	const float image_aspect_ratio = static_cast<float>(bmppixel.pixelArrayColumns * 8) / static_cast<float>(bmppixel.pixelArrayRows * 16);
	constexpr float print_width_meters = 0.2f;
	const float print_height_meters = print_width_meters / image_aspect_ratio;

	const int32_t x_resolution_ppm = static_cast<int32_t>((bmppixel.pixelArrayColumns * 8) / print_width_meters);
	const int32_t y_resolution_ppm = static_cast<int32_t>((bmppixel.pixelArrayRows * 16) / print_height_meters);

	constexpr uint32_t  num_colors = 0;
	constexpr uint32_t  important_colors = 0;

	std::vector<uint8_t> bmp(size);

	bmp[0] = (uint8_t)(type & 0xFF);
	bmp[1] = (uint8_t)((type >> 8) & 0xFF);

	bmp[2] = (uint8_t)(size & 0xFF);
	bmp[3] = (uint8_t)((size >> 8) & 0xFF);
	bmp[4] = (uint8_t)((size >> 16) & 0xFF);
	bmp[5] = (uint8_t)((size >> 24) & 0xFF);

	bmp[6] = (uint8_t)(reserved1 & 0xFF);
	bmp[7] = (uint8_t)((reserved1 >> 8) & 0xFF);

	bmp[8] = (uint8_t)(reserved2 & 0xFF);
	bmp[9] = (uint8_t)((reserved2 >> 8) & 0xFF);

	bmp[10] = (uint8_t)(offset & 0xFF);
	bmp[11] = (uint8_t)((offset >> 8) & 0xFF);
	bmp[12] = (uint8_t)((offset >> 16) & 0xFF);
	bmp[13] = (uint8_t)((offset >> 24) & 0xFF);

	bmp[14] = (uint8_t)(dib_header_size & 0xFF);
	bmp[15] = (uint8_t)((dib_header_size >> 8) & 0xFF);
	bmp[16] = (uint8_t)((dib_header_size >> 16) & 0xFF);
	bmp[17] = (uint8_t)((dib_header_size >> 24) & 0xFF);

	bmp[18] = (uint8_t)(width_px & 0xFF);
	bmp[19] = (uint8_t)((width_px >> 8) & 0xFF);
	bmp[20] = (uint8_t)((width_px >> 16) & 0xFF);
	bmp[21] = (uint8_t)((width_px >> 24) & 0xFF);

	bmp[22] = (uint8_t)(height_px & 0xFF);
	bmp[23] = (uint8_t)((height_px >> 8) & 0xFF);
	bmp[24] = (uint8_t)((height_px >> 16) & 0xFF);
	bmp[25] = (uint8_t)((height_px >> 24) & 0xFF);

	bmp[26] = (uint8_t)(num_planes & 0xFF);
	bmp[27] = (uint8_t)((num_planes >> 8) & 0xFF);

	bmp[28] = (uint8_t)(bits_per_pixel & 0xFF);
	bmp[29] = (uint8_t)((bits_per_pixel >> 8) & 0xFF);

	bmp[30] = (uint8_t)(compression & 0xFF);
	bmp[31] = (uint8_t)((compression >> 8) & 0xFF);
	bmp[32] = (uint8_t)((compression >> 16) & 0xFF);
	bmp[33] = (uint8_t)((compression >> 24) & 0xFF);

	bmp[34] = (uint8_t)(image_size_bytes & 0xFF);
	bmp[35] = (uint8_t)((image_size_bytes >> 8) & 0xFF);
	bmp[36] = (uint8_t)((image_size_bytes >> 16) & 0xFF);
	bmp[37] = (uint8_t)((image_size_bytes >> 24) & 0xFF);

	bmp[38] = (uint8_t)(x_resolution_ppm & 0xFF);
	bmp[39] = (uint8_t)((x_resolution_ppm >> 8) & 0xFF);
	bmp[40] = (uint8_t)((x_resolution_ppm >> 16) & 0xFF);
	bmp[41] = (uint8_t)((x_resolution_ppm >> 24) & 0xFF);

	bmp[42] = (uint8_t)(y_resolution_ppm & 0xFF);
	bmp[43] = (uint8_t)((y_resolution_ppm >> 8) & 0xFF);
	bmp[44] = (uint8_t)((y_resolution_ppm >> 16) & 0xFF);
	bmp[45] = (uint8_t)((y_resolution_ppm >> 24) & 0xFF);

	bmp[46] = (uint8_t)(num_colors & 0xFF);
	bmp[47] = (uint8_t)((num_colors >> 8) & 0xFF);
	bmp[48] = (uint8_t)((num_colors >> 16) & 0xFF);
	bmp[49] = (uint8_t)((num_colors >> 24) & 0xFF);

	bmp[50] = (uint8_t)(important_colors & 0xFF);
	bmp[51] = (uint8_t)((important_colors >> 8) & 0xFF);
	bmp[52] = (uint8_t)((important_colors >> 16) & 0xFF);
	bmp[53] = (uint8_t)((important_colors >> 24) & 0xFF);
#pragma endregion

	{
		int idx = 54;
		for (int row{ bmppixel.pixelArrayRows - 1 }; row >= 0; --row)
		{
			for (int j{ 0 }; j < 16; j++)
			{
				for (int i{ bmppixel.pixelArrayColumns - 1 }; i >= 0; --i)
				{
					const int id = row * bmppixel.pixelArrayColumns + i;
					const uint32_t intensity = bmppixel.PixelArrayData[id].AverageIntensity / 32;

					const std::array<uint8_t, 24> bitmap = getmap(j, intensity);

					for (int k = 0; k < 24; k++)
					{
						bmp[idx++] = bitmap[k];
					}
				}

			}
		}
	}

	std::ofstream outputFile(path, std::ios::binary);

	if (outputFile.is_open()) 
	{
		outputFile.write(reinterpret_cast<const char*>(bmp.data()), bmp.size());
		outputFile.close();
	}
	else
	{
		std::print(std::cerr,"Error: Could not open output file.\n");
	}
}

void ASC::BMPColor(const BMPPixel& bmppixel, const std::string& path)
{
#pragma region Header Data
	constexpr uint16_t  type = 0x4D42;
	constexpr uint16_t  reserved1 = 0;
	constexpr uint16_t  reserved2 = 0;
	constexpr uint32_t  offset = 54;
	constexpr uint32_t  dib_header_size = 40;
	const int32_t   width_px = static_cast<int32_t>((bmppixel.PixelArrayData.size() / bmppixel.pixelArrayRows) * 8);
	const int32_t   height_px = static_cast<int32_t>((bmppixel.PixelArrayData.size() / bmppixel.pixelArrayColumns) * 16);
	constexpr uint16_t  num_planes = 1;
	constexpr uint16_t  bits_per_pixel = 24;
	constexpr uint32_t  compression = 0;
	const uint32_t  image_size_bytes = (bmppixel.pixelArrayColumns * 8) * (bmppixel.pixelArrayRows * 16) * 3;
	const uint32_t  size = image_size_bytes + 54;

	const float image_aspect_ratio = static_cast<float>(bmppixel.pixelArrayColumns * 8) / static_cast<float>(bmppixel.pixelArrayRows * 16);
	constexpr float print_width_meters = 0.2f;
	const float print_height_meters = print_width_meters / image_aspect_ratio;

	const int32_t x_resolution_ppm = static_cast<int32_t>((bmppixel.pixelArrayColumns * 8) / print_width_meters);
	const int32_t y_resolution_ppm = static_cast<int32_t>((bmppixel.pixelArrayRows * 16) / print_height_meters);

	constexpr uint32_t  num_colors = 0;
	constexpr uint32_t  important_colors = 0;

	std::vector<uint8_t> bmp(size);

	bmp[0] = (uint8_t)(type & 0xFF);
	bmp[1] = (uint8_t)((type >> 8) & 0xFF);

	bmp[2] = (uint8_t)(size & 0xFF);
	bmp[3] = (uint8_t)((size >> 8) & 0xFF);
	bmp[4] = (uint8_t)((size >> 16) & 0xFF);
	bmp[5] = (uint8_t)((size >> 24) & 0xFF);

	bmp[6] = (uint8_t)(reserved1 & 0xFF);
	bmp[7] = (uint8_t)((reserved1 >> 8) & 0xFF);

	bmp[8] = (uint8_t)(reserved2 & 0xFF);
	bmp[9] = (uint8_t)((reserved2 >> 8) & 0xFF);

	bmp[10] = (uint8_t)(offset & 0xFF);
	bmp[11] = (uint8_t)((offset >> 8) & 0xFF);
	bmp[12] = (uint8_t)((offset >> 16) & 0xFF);
	bmp[13] = (uint8_t)((offset >> 24) & 0xFF);

	bmp[14] = (uint8_t)(dib_header_size & 0xFF);
	bmp[15] = (uint8_t)((dib_header_size >> 8) & 0xFF);
	bmp[16] = (uint8_t)((dib_header_size >> 16) & 0xFF);
	bmp[17] = (uint8_t)((dib_header_size >> 24) & 0xFF);

	bmp[18] = (uint8_t)(width_px & 0xFF);
	bmp[19] = (uint8_t)((width_px >> 8) & 0xFF);
	bmp[20] = (uint8_t)((width_px >> 16) & 0xFF);
	bmp[21] = (uint8_t)((width_px >> 24) & 0xFF);

	bmp[22] = (uint8_t)(height_px & 0xFF);
	bmp[23] = (uint8_t)((height_px >> 8) & 0xFF);
	bmp[24] = (uint8_t)((height_px >> 16) & 0xFF);
	bmp[25] = (uint8_t)((height_px >> 24) & 0xFF);

	bmp[26] = (uint8_t)(num_planes & 0xFF);
	bmp[27] = (uint8_t)((num_planes >> 8) & 0xFF);

	bmp[28] = (uint8_t)(bits_per_pixel & 0xFF);
	bmp[29] = (uint8_t)((bits_per_pixel >> 8) & 0xFF);

	bmp[30] = (uint8_t)(compression & 0xFF);
	bmp[31] = (uint8_t)((compression >> 8) & 0xFF);
	bmp[32] = (uint8_t)((compression >> 16) & 0xFF);
	bmp[33] = (uint8_t)((compression >> 24) & 0xFF);

	bmp[34] = (uint8_t)(image_size_bytes & 0xFF);
	bmp[35] = (uint8_t)((image_size_bytes >> 8) & 0xFF);
	bmp[36] = (uint8_t)((image_size_bytes >> 16) & 0xFF);
	bmp[37] = (uint8_t)((image_size_bytes >> 24) & 0xFF);

	bmp[38] = (uint8_t)(x_resolution_ppm & 0xFF);
	bmp[39] = (uint8_t)((x_resolution_ppm >> 8) & 0xFF);
	bmp[40] = (uint8_t)((x_resolution_ppm >> 16) & 0xFF);
	bmp[41] = (uint8_t)((x_resolution_ppm >> 24) & 0xFF);

	bmp[42] = (uint8_t)(y_resolution_ppm & 0xFF);
	bmp[43] = (uint8_t)((y_resolution_ppm >> 8) & 0xFF);
	bmp[44] = (uint8_t)((y_resolution_ppm >> 16) & 0xFF);
	bmp[45] = (uint8_t)((y_resolution_ppm >> 24) & 0xFF);

	bmp[46] = (uint8_t)(num_colors & 0xFF);
	bmp[47] = (uint8_t)((num_colors >> 8) & 0xFF);
	bmp[48] = (uint8_t)((num_colors >> 16) & 0xFF);
	bmp[49] = (uint8_t)((num_colors >> 24) & 0xFF);

	bmp[50] = (uint8_t)(important_colors & 0xFF);
	bmp[51] = (uint8_t)((important_colors >> 8) & 0xFF);
	bmp[52] = (uint8_t)((important_colors >> 16) & 0xFF);
	bmp[53] = (uint8_t)((important_colors >> 24) & 0xFF);
#pragma endregion

	{
		int idx = 54;
		for (int row{ bmppixel.pixelArrayRows - 1 }; row >= 0; --row)
		{
			for (int j{ 0 }; j < 16; j++)
			{
				for (int i{ bmppixel.pixelArrayColumns - 1 }; i >= 0; --i)
				{
					const int id = row * bmppixel.pixelArrayColumns + i;
					const uint32_t intensity = bmppixel.PixelArrayData[id].AverageIntensity / 32;

					const float avg = static_cast<float>(bmppixel.PixelArrayData[id].AverageRed + bmppixel.PixelArrayData[id].AverageGreen + bmppixel.PixelArrayData[id].AverageBlue) / 3.0f;

					//deviation from average
					const float red_dev = avg != 0 ? static_cast<float>(bmppixel.PixelArrayData[id].AverageRed) / avg : 0;
					const float green_dev = avg != 0 ? static_cast<float>(bmppixel.PixelArrayData[id].AverageGreen) / avg : 0;
					const float blue_dev = avg!= 0 ? static_cast<float>(bmppixel.PixelArrayData[id].AverageBlue) / avg : 0;

					const std::array<uint8_t, 24> bitmap = getmap(j, intensity);

					for (int k = 0; k < 24; k += 3)
					{
						bmp[idx++] = static_cast<uint8_t>(std::clamp(bitmap[k] * blue_dev, 0.0f, 255.0f));
						bmp[idx++] = static_cast<uint8_t>(std::clamp(bitmap[k + 1] * green_dev, 0.0f, 255.0f));
						bmp[idx++] = static_cast<uint8_t>(std::clamp(bitmap[k + 2] * red_dev, 0.0f, 255.0f));
					}
				}

			}
		}
	}

	std::ofstream outputFile(path, std::ios::binary);

	if (outputFile.is_open())
	{
		outputFile.write(reinterpret_cast<const char*>(bmp.data()), bmp.size());
		outputFile.close();
	}
	else
	{
		std::print(std::cerr, "Error: Could not open output file.\n");
	}
}

void ASC::AsciiImage(const std::string& filename, const std::string& outfilename, const int detail_x, const int detail_y)
{
	std::ifstream input(filename, std::ios::binary);

	const std::vector<uint8_t> rawData(std::istreambuf_iterator<char>(input), {});

	const BMPPixel bmppixel(rawData, detail_x, detail_y);

	ASC::BMPAscii(bmppixel, outfilename);
}

void ASC::ColorImage(const std::string& filename, const std::string& outfilename, const int detail_x, const int detail_y)
{
	std::ifstream input(filename, std::ios::binary);

	const std::vector<uint8_t> rawData(std::istreambuf_iterator<char>(input), {});

	const BMPPixel bmppixel(rawData, detail_x, detail_y);

	ASC::BMPColor(bmppixel, outfilename);
}

std::string ASC::Console(const std::string& filename, const int detail_x, const int detail_y)
{
	std::ifstream input(filename, std::ios::binary);

	const std::vector<uint8_t> rawData(std::istreambuf_iterator<char>(input), {});

	const BMPPixel bmppixel(rawData, detail_x, detail_y);

	std::string result;
	result.reserve(bmppixel.pixelArrayRows * (bmppixel.pixelArrayColumns + 1));

	for (int row{ 0 }; row < bmppixel.pixelArrayRows; ++row)
	{
		for (int i{ (row + 1) * bmppixel.pixelArrayColumns - 1 }; i >= row * bmppixel.pixelArrayColumns; --i)
		{
			const uint8_t intensity = bmppixel.PixelArrayData[i].AverageIntensity / 32;

			switch (intensity)
			{
			case 0:
				result += " ";
				break;
			case 1:
				result += ".";
				break;
			case 2:
				result += ":";
				break;
			case 3:
				result += "-";
				break;
			case 4:
				result += "=";
				break;
			case 5:
				result += "+";
				break;
			case 6:
				result += "#";
				break;
			default:
				result += "@";
				break;
			}
		}
		result += "\n";
	}
	return result;
}

std::string ASC::ColorConsole(const std::string& filename, const int detail_x, const int detail_y)
{
	std::ifstream input(filename, std::ios::binary);

	const std::vector<uint8_t> rawData(std::istreambuf_iterator<char>(input), {});

	const BMPPixel bmppixel(rawData, detail_x, detail_y);

	std::string result;
	result.reserve(bmppixel.pixelArrayRows * (bmppixel.pixelArrayColumns * 19 + 1));

	for (int row{ 0 }; row < bmppixel.pixelArrayRows; ++row)
	{
		for (int i{ (row + 1) * bmppixel.pixelArrayColumns - 1 }; i >= row * bmppixel.pixelArrayColumns; --i)
		{
			const std::string color = "\033[38;2;" + std::to_string(bmppixel.PixelArrayData[i].AverageRed) + ';' + std::to_string(bmppixel.PixelArrayData[i].AverageGreen) + ';' + std::to_string(bmppixel.PixelArrayData[i].AverageBlue) + 'm';
			const uint8_t intensity = bmppixel.PixelArrayData[i].AverageIntensity / 32;

			switch (intensity)
			{
			case 0:
				result += " ";
				break;
			case 1:
				result += color + '.';
				break;
			case 2:
				result += color + ':';
				break;
			case 3:
				result += color + '-';
				break;
			case 4:
				result += color + '=';
				break;
			case 5:
				result += color + '+';
				break;
			case 6:
				result += color + '#';
				break;
			default:
				result += color + '@';
				break;
			}
		}
		result += "\n";
	}
	return result;
}


