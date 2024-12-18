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

void thread(std::vector<std::thread>& threads, std::vector<std::string>& outputfiles)
{
	int64_t vect_size = outputfiles.size() / max_threads;

	std::vector<std::vector<std::string>> vectorvector;

	for (int i = 0; i < max_threads; i++)
	{
		std::vector<std::string> vector;
		if (i <= max_threads - 1)
		{
			vector.insert(vector.end(), outputfiles.begin() + (i * vect_size), outputfiles.begin() + ((i + 1) * vect_size));
		}
		else
		{
			vector.insert(vector.end(), outputfiles.begin() + (i * vect_size), outputfiles.end());
		}

		vectorvector.push_back(vector);
	}

	for (int i = 0; i < max_threads; ++i)
	{
		threads.emplace_back(ConvertFile, vectorvector[i]);
	}

	for (auto& t : threads)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
}

int main(int argc, char* argv[])
{
	std::string directory = "D:\\source\\AsciiVideoEncoder\\tempfolder\\";
	std::vector<std::thread> threads;
	threads.resize(max_threads);
	bool unconverted = true;
	if (argc < 2)
	{
		std::cerr << "Error: Usage wrong.";
		return 1;
	}

	std::string vidname = argv[1];

	std::string command = "ffmpeg -i \"" + vidname + "\" -vf \"fps = 24,scale=-1:720\" -pix_fmt bgr24 -y \"D:\\source\\AsciiVideoEncoder\\tempfolder\\output_%04d.bmp\"";
	std::cout << command << "\n";

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) {
		std::cerr << "Error: FFmpeg command failed." << std::endl;
		return 1;
	}
	else
	{
		std::cout << "Command executed successfully!" << "\n";

		std::vector<std::string> outputfiles = FSys::OutputFiles(directory);

		thread(threads, outputfiles);

		std::cout << "Done.";
		unconverted = false;
	}

	if (!unconverted)
	{
		std::string create_video = "ffmpeg -framerate 24 -i \"D:\\source\\AsciiVideoEncoder\\tempfolder\\ASCIIoutput_%04d.bmp\" -i \"" + vidname + "\" -map 0:v:0 -map 1:a:0 -vf \"scale=1920:-2,setsar=1:1\" -c:v libx264 -r 24 -y \"D:\\source\\AsciiVideoEncoder\\tempfolder\\asciioutput.mkv\"";

		int ffmpeg_video = system(create_video.c_str());

		if (ffmpeg_video != 0) {
			std::cerr << "Error: FFmpeg command failed." << std::endl;
			return 1;
		}
	}

	FSys::deleteTemporary(directory);

	return 0;
}

/*int main()
{
	ASC::FileToAsciiImage("C:\\Users\\Jandy\\Pictures\\wallpapers\\skeletonflipped.bmp", "C:\\Users\\Jandy\\Pictures\\wallpapers\\asciiskeletonflipped.bmp");
	return 0;
}*/
