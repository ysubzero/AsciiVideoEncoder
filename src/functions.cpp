#include "headers/functions.hpp"

	void clearScreen() 
	{
	#if defined(_WIN32) || defined(_WIN64)
		// Windows
		system("cls");
	#elif defined(__linux__) || defined(__APPLE__)
		// Linux or macOS
		system("clear");
	#else
		std::cerr << "Unknown operating system.";
	#endif
	}

	void ASC::consoleASCII(BMPPixel& bmppixel)
	{
		clearScreen();
		std::string result = "";
		for (int row = 0; row < bmppixel.pixelArrayRows; ++row)
		{
			for (int i = (row + 1) * bmppixel.pixelArrayColumns - 1; i >= row * bmppixel.pixelArrayColumns; --i)
			{
				if (i < 0)
				{
					break;
				}
				if (bmppixel.PixelArrayData[i].AverageIntensity < 25)
				{
					result += " ";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 50)
				{
					result += ".";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 75)
				{
					result += ":";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 100)
				{
					result += "-";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 150)
				{
					result += "=";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 200)
				{
					result += "+";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 225)
				{
					result += "#";
				}
				else
				{
					result += "@";
				}
			}
			result += "\n";
		}
		std::print("{0}", result);
	}

	void ASC::FileToTxt(std::string filename, std::string outputname, int detail_x, int detail_y)
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
				if (i < 0)
				{
					break;
				}
				if (bmppixel.PixelArrayData[i].AverageIntensity < 25)
				{
					result += " ";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 50)
				{
					result += ".";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 75)
				{
					result += ":";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 100)
				{
					result += "-";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 150)
				{
					result += "=";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 200)
				{
					result += "+";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 225)
				{
					result += "#";
				}
				else
				{
					result += "@";
				}
			}
			result += "\n";
		}

		std::ofstream outfile(outputname);
		std::print(outfile, "{0}", result);
		outfile.close();
	}

	void ASC::BMPToAsciiImage(BMPPixel& bmppixel, std::string path)
	{
		const Cha::Dash dash;
		const Cha::At at;
		const Cha::Colon colon;
		const Cha::Dot dot;
		const Cha::Equal equal;
		const Cha::Hashtag hashtag;
		const Cha::Plus plus;
		const Cha::Space space;
#pragma region Header Data
		BMPHeader& header = bmppixel.bmp.header;
		uint16_t  type = header.type;
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

		std::vector<uint8_t> header_data;

		header_data.push_back((uint8_t)(type & 0xFF));
		header_data.push_back((uint8_t)((type >> 8) & 0xFF));

		header_data.push_back((uint8_t)(size & 0xFF));
		header_data.push_back((uint8_t)((size >> 8) & 0xFF));
		header_data.push_back((uint8_t)((size >> 16) & 0xFF));
		header_data.push_back((uint8_t)((size >> 24) & 0xFF));

		header_data.push_back((uint8_t)(reserved1 & 0xFF));
		header_data.push_back((uint8_t)((reserved1 >> 8) & 0xFF));

		header_data.push_back((uint8_t)(reserved2 & 0xFF));
		header_data.push_back((uint8_t)((reserved2 >> 8) & 0xFF));

		header_data.push_back((uint8_t)(offset & 0xFF));
		header_data.push_back((uint8_t)((offset >> 8) & 0xFF));
		header_data.push_back((uint8_t)((offset >> 16) & 0xFF));
		header_data.push_back((uint8_t)((offset >> 24) & 0xFF));

		header_data.push_back((uint8_t)(dib_header_size & 0xFF));
		header_data.push_back((uint8_t)((dib_header_size >> 8) & 0xFF));
		header_data.push_back((uint8_t)((dib_header_size >> 16) & 0xFF));
		header_data.push_back((uint8_t)((dib_header_size >> 24) & 0xFF));

		header_data.push_back((uint8_t)((width_px) & 0xFF));
		header_data.push_back((uint8_t)(((width_px) >> 8) & 0xFF));
		header_data.push_back((uint8_t)(((width_px) >> 16) & 0xFF));
		header_data.push_back((uint8_t)(((width_px) >> 24) & 0xFF));

		header_data.push_back((uint8_t)((height_px) & 0xFF));
		header_data.push_back((uint8_t)(((height_px) >> 8) & 0xFF));
		header_data.push_back((uint8_t)(((height_px) >> 16) & 0xFF));
		header_data.push_back((uint8_t)(((height_px) >> 24) & 0xFF));

		header_data.push_back((uint8_t)(num_planes & 0xFF));
		header_data.push_back((uint8_t)((num_planes >> 8) & 0xFF));

		header_data.push_back((uint8_t)(bits_per_pixel & 0xFF));
		header_data.push_back((uint8_t)((bits_per_pixel >> 8) & 0xFF));

		header_data.push_back((uint8_t)(compression & 0xFF));
		header_data.push_back((uint8_t)((compression >> 8) & 0xFF));
		header_data.push_back((uint8_t)((compression >> 16) & 0xFF));
		header_data.push_back((uint8_t)((compression >> 24) & 0xFF));

		header_data.push_back((uint8_t)(image_size_bytes & 0xFF));
		header_data.push_back((uint8_t)((image_size_bytes >> 8) & 0xFF));
		header_data.push_back((uint8_t)((image_size_bytes >> 16) & 0xFF));
		header_data.push_back((uint8_t)((image_size_bytes >> 24) & 0xFF));

		header_data.push_back((uint8_t)((x_resolution_ppm) & 0xFF));
		header_data.push_back((uint8_t)(((x_resolution_ppm) >> 8) & 0xFF));
		header_data.push_back((uint8_t)(((x_resolution_ppm) >> 16) & 0xFF));
		header_data.push_back((uint8_t)(((x_resolution_ppm) >> 24) & 0xFF));

		header_data.push_back((uint8_t)((y_resolution_ppm) & 0xFF));
		header_data.push_back((uint8_t)(((y_resolution_ppm) >> 8) & 0xFF));
		header_data.push_back((uint8_t)(((y_resolution_ppm) >> 16) & 0xFF));
		header_data.push_back((uint8_t)(((y_resolution_ppm) >> 24) & 0xFF));

		header_data.push_back((uint8_t)(num_colors & 0xFF));
		header_data.push_back((uint8_t)((num_colors >> 8) & 0xFF));
		header_data.push_back((uint8_t)((num_colors >> 16) & 0xFF));
		header_data.push_back((uint8_t)((num_colors >> 24) & 0xFF));

		header_data.push_back((uint8_t)(important_colors & 0xFF));
		header_data.push_back((uint8_t)((important_colors >> 8) & 0xFF));
		header_data.push_back((uint8_t)((important_colors >> 16) & 0xFF));
		header_data.push_back((uint8_t)((important_colors >> 24) & 0xFF));
#pragma endregion

		std::vector<uint8_t> data;

		for (int row = bmppixel.pixelArrayRows - 1; row >= 0; --row)
		{
			for (int j = 0; j < 16; j++)
			{
				for (int i = (row + 1) * bmppixel.pixelArrayColumns - 1; i >= row * bmppixel.pixelArrayColumns; --i)
				{
					if (i < 0)
					{
						break;
					}
					if (bmppixel.PixelArrayData[i].AverageIntensity < 25)
					{
						for (auto dat : space.bit_map[j])
						{
							data.push_back(dat);
						}
					}
					else if (bmppixel.PixelArrayData[i].AverageIntensity < 50)
					{
						for (auto dat : dot.bit_map[j])
						{
							data.push_back(dat);
						}
					}
					else if (bmppixel.PixelArrayData[i].AverageIntensity < 75)
					{
						for (auto dat : colon.bit_map[j])
						{
							data.push_back(dat);
						}
					}
					else if (bmppixel.PixelArrayData[i].AverageIntensity < 100)
					{
						for (auto dat : dash.bit_map[j])
						{
							data.push_back(dat);
						}
					}
					else if (bmppixel.PixelArrayData[i].AverageIntensity < 150)
					{
						for (auto dat : equal.bit_map[j])
						{
							data.push_back(dat);
						}
					}
					else if (bmppixel.PixelArrayData[i].AverageIntensity < 200)
					{
						for (auto dat : plus.bit_map[j])
						{
							data.push_back(dat);
						}
					}
					else if (bmppixel.PixelArrayData[i].AverageIntensity < 225)
					{
						for (auto dat : hashtag.bit_map[j])
						{
							data.push_back(dat);
						}
					}
					else
					{
						for (auto dat : at.bit_map[j])
						{
							data.push_back(dat);
						}
					}
				}

			}
		}

		std::vector<uint8_t> bmp = header_data;
		bmp.insert(bmp.end(), data.begin(), data.end());

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

	void ASC::FileToAsciiImage(std::string filename, std::string outfilename, int detail_x, int detail_y)
	{
		std::ifstream input(filename, std::ios::binary);

		std::vector<uint8_t> rawData(std::istreambuf_iterator<char>(input), {});

		BMP bmp(rawData);

		BMPPixel bmppixel(bmp, detail_x, detail_y);

		ASC::BMPToAsciiImage(bmppixel, outfilename);
	}

	void ASC::FileToConsole(std::string filename, int detail_x, int detail_y)
	{
		std::ifstream input(filename, std::ios::binary);

		std::vector<uint8_t> rawData(std::istreambuf_iterator<char>(input), {});

		BMP bmp(rawData);

		BMPPixel bmppixel(bmp, detail_x, detail_y);

		ASC::consoleASCII(bmppixel);
	}

