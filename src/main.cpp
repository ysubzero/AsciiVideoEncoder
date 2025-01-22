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

void ConvertFile(const std::vector<std::string>& split, const uint32_t start, const uint32_t end, const std::string directory)
{
	for (uint32_t i = start; i < end; ++i)
	{
		if (terminate_program)
		{
			break;
		}
		ASC::FileToAsciiImage(directory + "/" + split[i], directory + "/ASCII" + split[i], 4, 8);
		std::print("{0} {1}{2}", "Converted", split[i], "\n");
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

int VideoToAsciiVideo(const std::string vidname, const std::string directory, const int framerate)
{
	if (!video(vidname))
	{
		std::print(std::cerr, "Invalid Type\n");
		return 1;
	}

	std::vector<std::thread> threads;
	threads.resize(max_threads);
	bool unconverted = true;

	std::string command = "ffmpeg -i \"" + vidname + "\" -vf \"fps = 24,scale=-1:720\" -pix_fmt bgr24 -y \"" + directory + "/output_%04d.bmp\"";
	std::print("{0}{1}", command, "\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) 
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		return 1;
	}

	else
	{
		std::print("Command executed successfully!\n");

		std::vector<std::string> outputfiles = FSys::OutputFiles(directory);

		auto start = std::chrono::high_resolution_clock::now();
		thread(threads, outputfiles, directory);
		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> duration = end - start;
		if (!terminate_program)
		{
			std::print("Done. This task took {} seconds.\n", duration.count());
			unconverted = false;
		}
	}

	if (!unconverted && !terminate_program)
	{
		std::string create_video = "ffmpeg -framerate 24 -i \"" + directory + "/ASCIIoutput_%04d.bmp\" -i \"" + vidname + "\" -map 0:v:0 -map 1:a:0 -vf \"scale=1920:-2,setsar=1:1\" -c:v libx264 -r 24 -y \"" + directory + "/asciioutput.mkv\"";

		int ffmpeg_video = system(create_video.c_str());

		if (ffmpeg_video != 0) 
		{
			std::print(std::cerr, "ffmpeg failed.\n");
			return 1;
		}
	}

	FSys::deleteTemporary(directory);
	return 0;
}

int VideoToConsole(const std::string vidname, const std::string directory, const int framerate)
{
	if (!video(vidname))
	{
		std::print(std::cerr, "Invalid Type\n");
		return 1;
	}

	std::vector<std::thread> threads;
	threads.resize(max_threads);
	bool unconverted = true;

	std::string command = "ffmpeg -i \"" + vidname + "\" -vf \"fps = 24,scale=-1:480\" -pix_fmt bgr24 -y \"" + directory + "/output_%04d.bmp\"";
	std::print("{0}{1}", command ,"\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0)
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		return 1;
	}

	else
	{
		std::print("Command executed successfully!\n");
		std::vector<std::string> outputfiles = FSys::OutputFiles(directory);
		clearScreen();
		const int sleep_t = (1000 / framerate) - 15;
		for (auto& s : outputfiles)
		{
			ASC::FileToConsole(directory + "/" + s, 8, 16);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000/25));
		}
	}

	FSys::deleteTemporary(directory);
	clearScreen();
	return 0;
}

int ConvertImageToAsciiImage(const std::string vidname, const std::string directory, const int detail)
{
	if (video(vidname))
	{
		std::print(std::cerr, "Invalid Type\n");
		return 1;
	}

	std::string command = "ffmpeg -i \"" + vidname + "\" -loglevel +error -pix_fmt bgr24 -y " + directory + "/images/input.bmp\"";
	std::print("Running ffmpeg...\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) 
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		FSys::deleteTemporary(directory); return 1;
	}

	std::print("Converted.\n");

	ASC::FileToAsciiImage(directory + "/images/input.bmp", directory + "/images/output.bmp", detail , detail * 2);
	std::print("Done.\n");
	return 0;
}

int ImageToText(const std::string vidname, const std::string directory, const int detail)
{
	if (video(vidname))
	{
		std::cerr << "Invalid Type\n";
		return 1;
	}

	std::string command = "ffmpeg -i \"" + vidname + "\" -pix_fmt bgr24 -y " + directory + "/images/input.bmp\"";
	std::print("{0}{1}", command, "\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) 
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		FSys::deleteTemporary(directory); return 1;
	}

	std::print("Converted.\n");

	ASC::FileToTxt(directory + "/images/input.bmp", directory + "/images/output.txt", 4, 8);
	std::print("Done.\n");
	return 0;
}

int main(int argc, char* argv[])
{
	std::string homefolder;

	std::ifstream settings("settings.txt");

	struct passwd *pw = getpwuid(getuid());

	if (!settings) 
	{
		std::print("No settings found. Creating default settings.\n");
		homefolder = std::string(pw->pw_dir) + "/AsciiVideoEncoder";
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

	fs::path homeDir(homefolder);

	if (homefolder.empty() || !fs::is_directory(homeDir))
	{
		std::print("Invalid Home Folder! Check your settings!\n");
		return 1;
	}

	if (!fs::exists(homefolder)) 
	{fs::create_directory(homefolder);}

	if (!fs::exists(homefolder + "/images"))
	{fs::create_directory(homefolder + "/images");}

	std::print("Clearing cache...\n");
	std::cout << std::flush;
	FSys::deleteTemporary(homefolder);

	std::unordered_map<std::string, std::function<int(const std::string, const std::string, const int)>> functionMap;
	functionMap["-v"] = VideoToAsciiVideo;
	functionMap["-i"] = ConvertImageToAsciiImage;
	functionMap["-c"] = VideoToConsole;
	functionMap["-t"] = ImageToText;

	signalhandle = homefolder;
	std::signal(SIGINT, signal_handler);
	std::signal(SIGTERM, signal_handler);

	if (argc != 4)
	{
		std::string output = argc > 4 ? "Too many arguments." : "Not enough arguments.";
		std::print(std::cerr, "{}", output);
		return 1;
	}

	std::string userinput = argv[1];
	std::string vidname = argv[2];

	int frate;

	try
	{
		frate = std::stoi(argv[3]);
	}
	catch (const std::invalid_argument& e)
	{
		std::print(std::cerr, "Invalid framerate/detail."); return 1;
	}
	catch (const std::out_of_range& e)
	{
		std::print(std::cerr, "Framerate/detail out of range."); return 1;
	}

	if (frate > 60 || frate <= 0)
	{
		std::print(std::cerr, "Framerate/detail must be inbetween 1 and 60."); return 1;
	}

	int result;

	if (functionMap.find(userinput) != functionMap.end())
	{
		result = functionMap[userinput](vidname, homefolder, frate);
	}
	else
	{
		std::print(std::cerr, "Not a function.");
		FSys::deleteTemporary(homefolder); return 1;
	}

	if (result == 1)
	{
		std::string outcerr = terminate_program ? "Clearing cache..." : "Failure: Error occured.";
		std::print(std::cerr, "{}", outcerr);
		FSys::deleteTemporary(homefolder); return 1;
	}

	std::print("Success!\n");

	if (userinput != "-c")
	{std::print("{0} {1}", "Saved to", homefolder);}

	else
	{std::print("Playback done.");}

	return 0;
}
