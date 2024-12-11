// AsciiVideoEncoder.cpp : Defines the entry point for the application.
//

#include "headers/AsciiVideoEncoder.hpp"
#include "headers/functions.hpp"
#include <thread>

const uint32_t max_threads = std::thread::hardware_concurrency();

void ConvertFile(std::vector<std::string> split)
{
	for (auto& file : split)
	{
		std::cout << "Converting " << file << " ";
		ASC::FileToAsciiImage("D:\\source\\AsciiVideoEncoder\\tempfolder\\" + file, "D:\\source\\AsciiVideoEncoder\\tempfolder\\ASCII" + file);
		std::cout << "Converted. \n";
	}
}

int main(int argc, char* argv[])
{
	bool unconverted = true;
	if (argc < 2)
	{
		std::cerr << "Error: Usage wrong.";
		return 1;
	}

	std::string vidname = argv[1];

	std::string command = "ffmpeg -i \"" + vidname + "\" -vf \"fps = 24,scale=-1:480\" -pix_fmt bgr24 -y \"D:\\source\\AsciiVideoEncoder\\tempfolder\\output_%04d.bmp\"";
	std::cout << command << "\n";

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) {
		std::cerr << "Error: FFmpeg command failed." << std::endl;
		return 1;
	}
	else
	{
		std::cout << "Command executed successfully!" << "\n";

		std::string directory = "D:\\source\\AsciiVideoEncoder\\tempfolder\\";

		std::vector<std::string> outputfiles = FSys::OutputFiles(directory);
		std::size_t const half_size = outputfiles.size() / 2;

		std::vector<std::string> osplit1(outputfiles.begin(), outputfiles.begin() + half_size);
		std::vector<std::string> osplit2(outputfiles.begin() + half_size, outputfiles.end());

		std::size_t const half_size1 = osplit1.size() / 2;
		std::size_t const half_size2 = osplit2.size() / 2;


		std::vector<std::string> split1(osplit1.begin(), osplit1.begin() + half_size1);
		std::vector<std::string> split2(osplit1.begin() + half_size1, osplit1.end());

		std::vector<std::string> split3(osplit2.begin(), osplit2.begin() + half_size2);
		std::vector<std::string> split4(osplit2.begin() + half_size2, osplit2.end());

		if (max_threads < 8)
		{
			std::thread th_1(ConvertFile, split1);
			std::thread th_2(ConvertFile, split2);
			std::thread th_3(ConvertFile, split3);
			std::thread th_4(ConvertFile, split4);

			th_1.join();
			th_2.join();
			th_3.join();
			th_4.join();

		}
		else
		{
			std::vector<std::string> split1a(split1.begin(), split1.begin() + split1.size() / 4);
			std::vector<std::string> split1b(split1.begin() + split1.size() / 4, split1.begin() + split1.size() / 2);
			std::vector<std::string> split1c(split1.begin() + split1.size() / 2, split1.begin() + 3 * split1.size() / 4);
			std::vector<std::string> split1d(split1.begin() + 3 * split1.size() / 4, split1.end());

			std::vector<std::string> split2a(split2.begin(), split2.begin() + split2.size() / 4);
			std::vector<std::string> split2b(split2.begin() + split2.size() / 4, split2.begin() + split2.size() / 2);
			std::vector<std::string> split2c(split2.begin() + split2.size() / 2, split2.begin() + 3 * split2.size() / 4);
			std::vector<std::string> split2d(split2.begin() + 3 * split2.size() / 4, split2.end());

			std::vector<std::string> split3a(split3.begin(), split3.begin() + split3.size() / 4);
			std::vector<std::string> split3b(split3.begin() + split3.size() / 4, split3.begin() + split3.size() / 2);
			std::vector<std::string> split3c(split3.begin() + split3.size() / 2, split3.begin() + 3 * split3.size() / 4);
			std::vector<std::string> split3d(split3.begin() + 3 * split3.size() / 4, split3.end());

			std::vector<std::string> split4a(split4.begin(), split4.begin() + split4.size() / 4);
			std::vector<std::string> split4b(split4.begin() + split4.size() / 4, split4.begin() + split4.size() / 2);
			std::vector<std::string> split4c(split4.begin() + split4.size() / 2, split4.begin() + 3 * split4.size() / 4);
			std::vector<std::string> split4d(split4.begin() + 3 * split4.size() / 4, split4.end());

			std::thread th_1a(ConvertFile, split1a);
			std::thread th_1b(ConvertFile, split1b);
			std::thread th_1c(ConvertFile, split1c);
			std::thread th_1d(ConvertFile, split1d);

			std::thread th_2a(ConvertFile, split2a);
			std::thread th_2b(ConvertFile, split2b);
			std::thread th_2c(ConvertFile, split2c);
			std::thread th_2d(ConvertFile, split2d);

			std::thread th_3a(ConvertFile, split3a);
			std::thread th_3b(ConvertFile, split3b);
			std::thread th_3c(ConvertFile, split3c);
			std::thread th_3d(ConvertFile, split3d);

			std::thread th_4a(ConvertFile, split4a);
			std::thread th_4b(ConvertFile, split4b);
			std::thread th_4c(ConvertFile, split4c);
			std::thread th_4d(ConvertFile, split4d);

			th_1a.join();
			th_1b.join();
			th_1c.join();
			th_1d.join();

			th_2a.join();
			th_2b.join();
			th_2c.join();
			th_2d.join();

			th_3a.join();
			th_3b.join();
			th_3c.join();
			th_3d.join();

			th_4a.join();
			th_4b.join();
			th_4c.join();
			th_4d.join();
		}

		std::cout << "Done.";
		unconverted = false;
	}



	if (!unconverted)
	{
		std::string create_video = "ffmpeg -framerate 24 -i \"D:\\source\\AsciiVideoEncoder\\tempfolder\\ASCIIoutput_%04d.bmp\" -i \"" + vidname + "\" -map 0:v:0 -map 1:a:0 -vf \"scale=1280:-2,setsar=1:1\" -c:v libx264 -r 24 \"D:\\source\\AsciiVideoEncoder\\tempfolder\\asciioutput.mkv\"";

		int ffmpeg_video = system(create_video.c_str());

		if (ffmpeg_video != 0) {
			std::cerr << "Error: FFmpeg command failed." << std::endl;
			return 1;
		}
	}


	return 0;
}
