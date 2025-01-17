#include "headers/functions.hpp"

const Cha::Dash dash;
const Cha::At at;
const Cha::Colon colon;
const Cha::Dot dot;
const Cha::Equal equal;
const Cha::Hashtag hashtag;
const Cha::Plus plus;
const Cha::Space space;

	void clearScreen() 
	{
		std::print("\033[2J\033[H");
	}

	void ASC::consoleASCII(const BMPPixel& bmppixel)
	{
		std::string result = "";
		for (int row{ 0 }; row < bmppixel.pixelArrayRows; ++row)
		{
			for (int i{ (row + 1) * bmppixel.pixelArrayColumns - 1 }; i >= row * bmppixel.pixelArrayColumns; --i)
			{
				uint8_t intensity = bmppixel.PixelArrayData[i].AverageIntensity / 32;

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
		std::print("\033[H");
		std::print("{0}", result);
		std::cout << std::flush;
	}

	void ASC::FileToTxt(const std::string& filename, const std::string& outputname, const int detail_x, const int detail_y)
	{
		std::ifstream input(filename, std::ios::binary);

		std::vector<uint8_t> rawData(std::istreambuf_iterator<char>(input), {});

		BMP bmp(rawData);

		BMPPixel bmppixel(bmp, detail_x, detail_y);

		std::string result = "";

		for (int row = 0; row < bmppixel.pixelArrayRows; ++row)
		{
			for (int i = (row + 1) * bmppixel.pixelArrayColumns - 1; i >= row * bmppixel.pixelArrayColumns; --i)
			{
				uint8_t intensity = bmppixel.PixelArrayData[i].AverageIntensity / 32;

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

	static const std::array<uint8_t, 24>& getmap(const int j, const uint8_t intensity)
	{
		switch (intensity)
		{
		case 0:
			return space.bit_map[j];
		case 1:
			return dot.bit_map[j];
		case 2:
			return colon.bit_map[j];
		case 3:
			return dash.bit_map[j];
		case 4 :
			return equal.bit_map[j];
		case 5:
			return plus.bit_map[j];
		case 6:
			return hashtag.bit_map[j];
		default:
			return at.bit_map[j];
		}
		
	}

	void ASC::BMPToAsciiImage(const BMPPixel& bmppixel, const std::string& path)
	{
#pragma region Header Data
		uint16_t  type = 0x4D42;
		uint16_t  reserved1 = 0;
		uint16_t  reserved2 = 0;
		uint32_t  offset = 54;
		uint32_t  dib_header_size = 40;
		int32_t   width_px = static_cast<int32_t>((bmppixel.PixelArrayData.size() / bmppixel.pixelArrayRows) * 8);
		int32_t   height_px = static_cast<int32_t>((bmppixel.PixelArrayData.size() / bmppixel.pixelArrayColumns) * 16);
		uint16_t  num_planes = 1;
		uint16_t  bits_per_pixel = 24;
		uint32_t  compression = 0;
		uint32_t  image_size_bytes = (bmppixel.pixelArrayColumns * 8) * (bmppixel.pixelArrayRows * 16) * 3;
		uint32_t  size = image_size_bytes + 54;

		float image_aspect_ratio = static_cast<float>(bmppixel.pixelArrayColumns * 8) /
			static_cast<float>(bmppixel.pixelArrayRows * 16);

		float print_width_meters = 0.2f;

		float print_height_meters = print_width_meters / image_aspect_ratio;

		int32_t x_resolution_ppm = static_cast<int32_t>((bmppixel.pixelArrayColumns * 8) / print_width_meters);
		int32_t y_resolution_ppm = static_cast<int32_t>((bmppixel.pixelArrayRows * 16) / print_height_meters);

		uint32_t  num_colors = 0;
		uint32_t  important_colors = 0;

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
						int id = row * bmppixel.pixelArrayColumns + i;
						uint8_t intensity = bmppixel.PixelArrayData[id].AverageIntensity / 32;

						const std::array<uint8_t, 24>& bitmap = getmap(j, intensity);

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
			std::print(std::cerr,"Error: Could not open output file.");
		}
	}

	void ASC::FileToAsciiImage(const std::string& filename, const std::string& outfilename, const int detail_x, const int detail_y)
	{
		std::ifstream input(filename, std::ios::binary);

		std::vector<uint8_t> rawData(std::istreambuf_iterator<char>(input), {});

		BMP bmp(rawData);

		BMPPixel bmppixel(bmp, detail_x, detail_y);

		ASC::BMPToAsciiImage(bmppixel, outfilename);
	}

	void ASC::FileToConsole(const std::string& filename, const int detail_x, const int detail_y)
	{
		std::ifstream input(filename, std::ios::binary);

		std::vector<uint8_t> rawData(std::istreambuf_iterator<char>(input), {});

		BMP bmp(rawData);

		BMPPixel bmppixel(bmp, detail_x, detail_y);

		ASC::consoleASCII(bmppixel);
	}

