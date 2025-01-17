#include "headers/AsciiVideoEncoder.hpp"
#include "headers/functions.hpp"
#include <csignal>

const uint32_t max_threads = std::thread::hardware_concurrency();

std::string signalhandle;

std::atomic<bool> terminate_program(false);

void signal_handler(int signal) 
{
	terminate_program = true;
	std::print("Terminating...\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	FSys::deleteTemporary(signalhandle);
	std::exit(0);
}

bool video(const std::string vidname)
{
	std::vector<std::string> videoExtensions = { ".mp4", ".avi", ".mkv", ".mov", ".flv", ".webm", ".mpg", ".mpeg", ".wmv" };

	size_t dotPos = vidname.rfind('.');
	if (dotPos == std::string::npos)
	{
		std::print(std::cerr, "Invalid file.\n");
		return false;
	}

	std::string extension = vidname.substr(dotPos);

	return std::find(videoExtensions.begin(), videoExtensions.end(), extension) != videoExtensions.end();
}

void ConvertFile(const std::vector<std::string>& split, uint32_t start, uint32_t end, const std::string directory)
{

		for (uint32_t i = start; i < end; i++)
		{
			if (!terminate_program)
			{
				std::string file = split[i];
				ASC::FileToAsciiImage(directory + "\\" + file, directory + "\\ASCII" + file, 4, 8);
				std::print("Converted {}\r", file);
			}
			else
			{
				break;
			}
		}
}

void thread(std::vector<std::thread>& threads, const std::vector<std::string>& outputfiles, const std::string directory)
{
	int64_t vect_size = outputfiles.size() / max_threads;

	for (int i = 0; i < max_threads; ++i)
	{
		uint32_t end;
		uint32_t start = i * vect_size;

		if (i < max_threads - 1) 
		{
			end = (i + 1) * vect_size;
		}
		else 
		{
			end = outputfiles.size();
		}

		threads.emplace_back(ConvertFile, outputfiles, start, end, directory);
	}


	for (auto& t : threads) {if (t.joinable()) { t.join(); }}
}

int VideoToAsciiVideo(const std::string vidname, const std::string directory)
{
	if (!video(vidname))
	{
		std::print(std::cerr, "Invalid Type\n");
		FSys::deleteTemporary(directory); return 1;
	}

	std::vector<std::thread> threads;
	threads.resize(max_threads);
	bool unconverted = true;

	std::string command = "ffmpeg -i \"" + vidname + "\" -vf \"fps = 24,scale=-1:720\" -pix_fmt bgr24 -y \"" + directory + "\\output_%04d.bmp\"";
	std::print("{0}{1}", command, "\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) 
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		FSys::deleteTemporary(directory); return 1;
	}

	else
	{
		std::print("Command executed successfully!\n");

		std::vector<std::string> outputfiles = FSys::OutputFiles(directory);

		auto start = std::chrono::high_resolution_clock::now();
		thread(threads, outputfiles, directory);
		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> duration = end - start;
		std::print("Done. This task took {} seconds.\n", duration.count());

		unconverted = false;
	}

	if (!unconverted && !terminate_program)
	{
		std::string create_video = "ffmpeg -framerate 24 -i \"" + directory + "\\ASCIIoutput_%04d.bmp\" -i \"" + vidname + "\" -map 0:v:0 -map 1:a:0 -vf \"scale=1920:-2,setsar=1:1\" -c:v libx264 -r 24 -y \"" + directory + "\\asciioutput.mkv\"";

		int ffmpeg_video = system(create_video.c_str());

		if (ffmpeg_video != 0) 
		{
			std::print(std::cerr, "ffmpeg failed.\n");
			FSys::deleteTemporary(directory); return 1;
		}
		FSys::deleteTemporary(directory);
	}
	else
	{
		return 1;
	}
	return 0;
}

int ConvertImageToAsciiImage(const std::string vidname, const std::string directory)
{
	if (video(vidname))
	{
		std::print(std::cerr, "Invalid Type\n");
		FSys::deleteTemporary(directory); return 1;
	}

	std::string command = "ffmpeg -i \"" + vidname + "\" -pix_fmt bgr24 -y " + directory + "\\images\\input.bmp\"";
	std::print("{0}{1}", command, "\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) 
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		FSys::deleteTemporary(directory); return 1;
	}

	std::print("Converted.\n");

	ASC::FileToAsciiImage(directory + "\\images\\input.bmp", directory + "\\images\\output.bmp", 4 , 8);
	std::print("Done.\n");
	return 0;
}

int VideoToConsole(const std::string vidname, const std::string directory)
{
	if (!video(vidname))
	{
		std::print(std::cerr, "Invalid Type\n");
		FSys::deleteTemporary(directory); return 1;
	}

	std::vector<std::thread> threads;
	threads.resize(max_threads);
	bool unconverted = true;

	std::string command = "ffmpeg -i \"" + vidname + "\" -vf \"fps = 24,scale=-1:640\" -pix_fmt bgr24 -y \"" + directory + "\\output_%04d.bmp\"";
	std::print("{0}{1}", command ,"\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) 
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		FSys::deleteTemporary(directory); return 1;
	}

	else
	{
		std::print("Command executed successfully!\n");
		std::vector<std::string> outputfiles = FSys::OutputFiles(directory);
		clearScreen();
		for (auto& s : outputfiles)
		{
			if (!terminate_program)
			{
				ASC::FileToConsole(directory + "\\" + s, 8, 16);
				std::this_thread::sleep_for(std::chrono::milliseconds(27));
			}
			else
			{
				return 1;
			}
		}
	}

	FSys::deleteTemporary(directory);
	clearScreen();
	return 0;
}

int ImageToText(const std::string vidname, const std::string directory)
{
	if (video(vidname))
	{
		std::cerr << "Invalid Type\n";
		FSys::deleteTemporary(directory); return 1;
	}

	std::string command = "ffmpeg -i \"" + vidname + "\" -pix_fmt bgr24 -y " + directory + "\\images\\input.bmp\"";
	std::print("{0}{1}", command, "\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) 
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		FSys::deleteTemporary(directory); return 1;
	}

	std::print("Converted.\n");

	ASC::FileToTxt(directory + "\\images\\input.bmp", directory + "\\images\\output.txt", 4, 8);
	std::print("Done.\n");
	return 0;
}

int main(int argc, char* argv[])
{
	std::string homefolder;

	std::ifstream settings("settings.txt");

	if (!settings) 
	{
		std::print("No settings found. Creating default settings.\n");
		homefolder = "C:\\AsciiVideoEncoder";
		std::ofstream set("settings.txt");
		std::print(set,"{0}", homefolder);
		set.close();
	}
	else
	{
		std::print("Settings found.\n");
		std::getline(settings, homefolder);
		settings.close();
	}

	if (!fs::exists(homefolder)) 
	{fs::create_directory(homefolder);}

	if (!fs::exists(homefolder + "\\images"))
	{fs::create_directory(homefolder + "\\images");}

	std::print("Clearing cache...\n");
	std::cout << std::flush;
	FSys::deleteTemporary(homefolder);

	std::unordered_map<std::string, std::function<int(const std::string, const std::string)>> functionMap;
	functionMap["-v"] = VideoToAsciiVideo;
	functionMap["-i"] = ConvertImageToAsciiImage;
	functionMap["-c"] = VideoToConsole;
	functionMap["-t"] = ImageToText;

	signalhandle = homefolder;
	std::signal(SIGINT, signal_handler);
	std::signal(SIGTERM, signal_handler);

	if (argc > 3)
	{
		std::print(std::cerr, "Too many arguments.");
		FSys::deleteTemporary(homefolder); return 1;
	}

	if (argc < 3)
	{
		std::print(std::cerr, "Not enough arguments.");
		FSys::deleteTemporary(homefolder); return 1;
	}

	std::string userinput = argv[1];

	std::string vidname = argv[2];

	int result;

	if (functionMap.find(userinput) != functionMap.end())
	{
		result = functionMap[userinput](vidname, homefolder);
	}
	else
	{
		std::print(std::cerr, "Not a function.");
		FSys::deleteTemporary(homefolder); return 1;
	}

	if (result == 1)
	{
		std::print(std::cerr, "Failed.");
		FSys::deleteTemporary(homefolder); return 1;
	}
	std::cout << std::flush;
	if (userinput != "-c")
	{std::print("{0} {1}", "\nSaved to", homefolder);}

	else
	{std::print("Playback done.");}

	return 0;
}
