#include "headers/AsciiVideoEncoder.hpp"
#include "headers/ascii.hpp"

namespace ASC
{
	void printASCII(BMPPixel& bmppixel)
	{
		std::string result = "";
		for (int row = bmppixel.pixelArrayRows - 1; row >= 0; row--)
		{
			for (int i = (row + 1) * bmppixel.pixelArrayColumns - 1; i > row * bmppixel.pixelArrayColumns; i--)
			{
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

		std::ofstream outfile("testbit6.txt");
		outfile << result;
		outfile.close();
	}

	std::vector<std::string> ImageToAscii(BMPPixel& bmppixel)
	{
		std::vector<std::string> strings;

		for (int row = bmppixel.pixelArrayRows - 1; row >= 0; row--)
		{
			std::string string_row = "";
			for (int i = (row + 1) * bmppixel.pixelArrayColumns - 1; i > row * bmppixel.pixelArrayColumns; i--)
			{
				if (bmppixel.PixelArrayData[i].AverageIntensity < 25)
				{
					string_row += " ";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 50)
				{
					string_row += ".";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 75)
				{
					string_row += ":";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 100)
				{
					string_row += "-";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 150)
				{
					string_row += "=";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 200)
				{
					string_row += "+";
				}
				else if (bmppixel.PixelArrayData[i].AverageIntensity < 225)
				{
					string_row += "#";
				}
				else
				{
					string_row += "@";
				}
			}
			strings.push_back(string_row);
		}
		return strings;
	}

	void AsciiToImage(std::vector<std::string>& strings, BMPPixel& bmppixel, std::string path)
	{
		const Cha::Dash dash;
		const Cha::At at;
		const Cha::Colon colon;
		const Cha::Dot dot;
		const Cha::Equal equal;
		const Cha::Hashtag hashtag;
		const Cha::Plus plus;
		const Cha::Space space;

		BMPHeader& header = bmppixel.bmp.header;
		uint16_t  type = header.type;
		uint16_t  reserved1 = 0;
		uint16_t  reserved2 = 0;
		uint32_t  offset = 54;
		uint32_t  dib_header_size = 40;
		int32_t   width_px = strings[0].size() * 8;
		int32_t   height_px = strings.size() * 16;
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

		std::vector<uint8_t> data;

		for (auto& string : strings)
		{
			for (int i = 0; i < 16; i++)
			{
				for (auto& ch : string)
				{
					switch (ch)
					{
						case ' ':
							for (auto dat : space.bit_map[i])
							{
								data.push_back(dat);
							}
							break;
						case '.':
							for (auto dat : dot.bit_map[i])
							{
								data.push_back(dat);
							}
							break;
						case '-':
							for (auto dat : dash.bit_map[i])
							{
								data.push_back(dat);
							}
							break;
						case '=':
							for (auto dat : equal.bit_map[i])
							{
								data.push_back(dat);
							}
							break;
						case ':':
							for (auto dat : colon.bit_map[i])
							{
								data.push_back(dat);
							}
							break;
						case '+':
							for (auto dat : plus.bit_map[i])
							{
								data.push_back(dat);
							}
							break;
						case '#':
							for (auto dat : hashtag.bit_map[i])
							{
								data.push_back(dat);
							}
							break;
						case '@':
							for (auto dat : at.bit_map[i])
							{
								data.push_back(dat);
							}
							break;
						default:
							for (auto dat : space.bit_map[i])
							{
								data.push_back(dat);
							}
							break;
					}
				}
			}
		}
		
		std::vector<uint8_t> bmp = header_data;
		bmp.insert(bmp.end(), data.begin(), data.end());

		std::ofstream outputFile(path, std::ios::binary);

		if (outputFile.is_open()) {
			outputFile.write(reinterpret_cast<const char*>(bmp.data()), bmp.size());
			outputFile.close();
		}
		else {
			std::cerr << "Error: Could not open output file." << std::endl;
		}
	}

	void BMPToAsciiImage(BMPPixel& bmppixel, std::string path)
	{
		const Cha::Dash dash;
		const Cha::At at;
		const Cha::Colon colon;
		const Cha::Dot dot;
		const Cha::Equal equal;
		const Cha::Hashtag hashtag;
		const Cha::Plus plus;
		const Cha::Space space;

		BMPHeader& header = bmppixel.bmp.header;
		uint16_t  type = header.type;
		uint16_t  reserved1 = 0;
		uint16_t  reserved2 = 0;
		uint32_t  offset = 54;
		uint32_t  dib_header_size = 40;
		int32_t   width_px = static_cast<int32_t>((bmppixel.PixelArrayData.size()/ bmppixel.pixelArrayRows) * 8);
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

		#pragma region Header Data
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

		if (outputFile.is_open()) {
			outputFile.write(reinterpret_cast<const char*>(bmp.data()), bmp.size());
			outputFile.close();
		}
		else 
		{
			std::cerr << "Error: Could not open output file." << std::endl;
		}
	}

	void ImageToAsciiToImage(BMPPixel& bmppixel, std::string path)
	{
		std::vector<std::string> Ascii = ImageToAscii(bmppixel);
		AsciiToImage(Ascii, bmppixel, path);
	}

	void ImageToAsciiImage(BMPPixel& bmppixel, std::string path)
	{
		BMPToAsciiImage(bmppixel, path);
	}

	void FileToAsciiImage(std::string filename, std::string outfilename)
	{
		std::ifstream input(filename, std::ios::binary);

		std::vector<uint8_t> rawData(std::istreambuf_iterator<char>(input), {});

		BMP bmp(rawData);

		BMPPixel bmppixel(bmp, 4, 8);

		ImageToAsciiImage(bmppixel, outfilename);
	}
}

