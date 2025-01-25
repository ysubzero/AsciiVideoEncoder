#include "headers/AsciiVideoEncoder.hpp"
#include "headers/functions.hpp"

const uint32_t max_threads = std::thread::hardware_concurrency();

std::string folder;

std::atomic<bool> terminate_program(false);

void signal_handler(int signal) 
{
	terminate_program = true;
	std::print("Terminating...\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	FSys::deleteTemporary(folder);
	std::exit(0);
}

static bool video(const std::string& vidname)
{
	const std::vector<std::string> videoExtensions = { ".mp4", ".avi", ".mkv", ".mov", ".flv", ".webm", ".mpg", ".mpeg", ".wmv" };

	size_t dotPos = vidname.rfind('.');
	if (dotPos == std::string::npos)
	{
		std::print(std::cerr, "Invalid file.\n");
		return false;
	}

	std::string extension = vidname.substr(dotPos);

	return std::find(videoExtensions.begin(), videoExtensions.end(), extension) != videoExtensions.end();
}

static void ConvertFile(const std::vector<std::string>& split, const size_t start, const size_t end, const std::string& directory, const bool color)
{
	auto func = color ? static_cast<void(*)(const std::string&, const std::string&, const int, const int)>(&ASC::ColorImage)
		: static_cast<void(*)(const std::string&, const std::string&, const int, const int)>(&ASC::AsciiImage);

		for (uint32_t i = start; i < end; i++)
		{
			if (!terminate_program)
			{
				std::string file = split[i];
				func(directory + "\\" + file, directory + "\\ASCII" + file, 4, 8);
				std::print("Converted {}\r", file);
			}
			else {break;}
		}
}

static void thread(std::vector<std::thread>& threads, const std::vector<std::string>& outputfiles, const std::string& directory, const bool color)
{
	const size_t vect_size = outputfiles.size() / max_threads;

	for (size_t i = 0; i < max_threads; ++i)
	{
		size_t end;
		size_t start = i * vect_size;

		if (i < max_threads - 1) 
		{
			end = (i + 1) * vect_size;
		}
		else {end = outputfiles.size();}

		threads.emplace_back(ConvertFile, outputfiles, start, end, directory, color);
	}


	for (auto& t : threads) {if (t.joinable()) { t.join(); }}
}

static int AsciiVideo(const std::string& vidname, const std::string& directory, const int framerate, const bool color)
{
	if (!video(vidname))
	{
		std::print(std::cerr, "Invalid Type\n");
		return 1;
	}

	std::vector<std::thread> threads;
	bool unconverted = true;

	const std::string command = "ffmpeg -i \"" + vidname + "\" -loglevel +error -vf \"fps = " + std::to_string(framerate) + ",scale=-1:720\" -pix_fmt bgr24 -y \"" + directory + "\\output_%05d.bmp\"";
	std::print("Running ffmpeg...\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) 
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		return 1;
	}

	else
	{
		std::print("Command executed successfully!\n");

		const std::vector<std::string> outputfiles = FSys::OutputFiles(directory);

		const auto start = std::chrono::high_resolution_clock::now();
		thread(threads, outputfiles, directory, color);
		const auto end = std::chrono::high_resolution_clock::now();

		const std::chrono::duration<double> duration = end - start;
		if (!terminate_program)
		{
			std::print("Done. This task took {} seconds.\n", duration.count());
			unconverted = false;
		}
	}

	if (!unconverted && !terminate_program)
	{
		std::print("Running ffmpeg...\n");

		std::string create_video = "ffmpeg -framerate "+ std::to_string(framerate) + " -i \"" + directory + "\\ASCIIoutput_%05d.bmp\" -i \"" + vidname + "\" -loglevel +error -map 0:v:0 -map 1:a:0 -vf \"scale=1920:-2,setsar=1:1\" -c:v libx264 -r 24 -y \"" + directory + "\\asciioutput.mkv\"";

		int ffmpeg_video = system(create_video.c_str());

		if (ffmpeg_video != 0) 
		{
			std::print(std::cerr, "ffmpeg failed.\n");
			return 1;
		}
		FSys::deleteTemporary(directory);
	}
	else
	{return 1;}

	return 0;
}

static int Console(const std::string& vidname, const std::string& directory, const int framerate, const bool color)
{
	if (!video(vidname))
	{
		std::print(std::cerr, "Invalid Type\n");
		return 1;
	}

	std::vector<std::thread> threads;
	threads.resize(max_threads);
	bool unconverted = true;

	const std::string command = "ffmpeg -i \"" + vidname + "\" -loglevel +error -vf \"fps = " + std::to_string(framerate) + ",scale=-1:640\" -pix_fmt bgr24 -y \"" + directory + "\\output_%05d.bmp\"";
	std::print("Running ffmpeg...\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0)
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		return 1;
	}

	else
	{
		std::print("Command executed successfully!\n");
		const std::vector<std::string> outputfiles = FSys::OutputFiles(directory);
		clearScreen();
		const int sleep_t = (1000 / framerate) - 15;
		for (auto& s : outputfiles)
		{
			if (!terminate_program)
			{
				if (color)
				{
					ASC::ColorConsole(directory + "\\" + s, 8, 16);
				}
				else
				{
					ASC::Console(directory + "\\" + s, 8, 16);
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(sleep_t));
			}
			else
			{
				std::print("\033[0m");
				std::print("Terminated\n");
				return 1;
			}
		}
	}

	std::print("\033[0m");
	FSys::deleteTemporary(directory);
	clearScreen();
	return 0;
}

static int AsciiImage(const std::string& vidname, const std::string& directory, const int detail, const bool color)
{
	if (video(vidname))
	{
		std::print(std::cerr, "Invalid Type\n");
		return 1;
	}

	std::string command = "ffmpeg -i \"" + vidname + "\" -loglevel +error -pix_fmt bgr24 -y " + directory + "\\images\\input.bmp\"";
	std::print("Running ffmpeg...\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) 
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		FSys::deleteTemporary(directory); return 1;
	}

	std::print("Converted.\n");

	if (color) {ASC::ColorImage(directory + "\\images\\input.bmp", directory + "\\images\\output.bmp", detail, detail * 2);}
	else {ASC::AsciiImage(directory + "\\images\\input.bmp", directory + "\\images\\output.bmp", detail, detail * 2);}

	std::print("Done.\n");
	return 0;
}

int Text(const std::string& vidname, const std::string& directory, const int detail, const bool color)
{
	//color remains unused for now.
	if (video(vidname))
	{
		std::cerr << "Invalid Type\n";
		return 1;
	}

	std::string command = "ffmpeg -i \"" + vidname + "\" -loglevel +error -pix_fmt bgr24 -y " + directory + "\\images\\input.bmp\"";
	std::print("Running ffmpeg...\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) 
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		FSys::deleteTemporary(directory); return 1;
	}

	std::print("Converted.\n");

	ASC::Text(directory + "\\images\\input.bmp", directory + "\\images\\output.txt", detail, detail * 2);
	std::print("Done.\n");
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc >= 2 && std::string(argv[1]) == "-h")
	{
		std::print("Commands:\n\t-v: Outputs ASCII video.\n\t-i: Outputs ASCII image.\n\t-c: Outputs to Console.\n\t-t: Outputs to text.\n\t-k: Outputs Color Video.\n\t-j: Outputs Color Image.\n\t-m: Outputs to Console, colored.\n\
Correct format for command line input is: AsciiVideoEncoder [Command] [Filename] [Framerate/detail].\n\
Video commands use the fourth argument as framerate while image commands use the fourth argument as the detail (how the pixels are mapped to an ASCII character).\n\
For a detail of 4 it would be: 4 x 8 = 32 pixels mapped to each ASCII character.");
		return 0;
	}

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
	{
		try
		{
			std::print("Folder does not exist. Creating folder...\n");
			fs::create_directory(homefolder);

			fs::create_directory(homefolder + "\\images");

			if (!fs::exists(homefolder) || !fs::exists(homefolder + "\\images"))
			{
				std::print(std::cerr, "Unable to create folder!");
				return 1;
			}
		}
		catch (const std::exception& e)
		{
			std::print(std::cerr, "Error: {}", e.what());
			return 1;
		}
	}

	std::print("Clearing cache...\n");
	std::cout << std::flush;
	FSys::deleteTemporary(homefolder);

	std::unordered_map<std::string, std::function<int(const std::string&, const std::string&, const int, const bool)>> functionMap;
	functionMap["-v"] = AsciiVideo;
	functionMap["-i"] = AsciiImage;
	functionMap["-c"] = Console;
	functionMap["-t"] = Text;
	functionMap["-k"] = AsciiVideo;
	functionMap["-j"] = AsciiImage;
	functionMap["-m"] = Console;

	std::unordered_map<std::string, bool> colorMap;
	colorMap["-v"] = false;
	colorMap["-i"] = false;
	colorMap["-c"] = false;
	colorMap["-t"] = false;
	colorMap["-k"] = true;
	colorMap["-j"] = true;
	colorMap["-m"] = true;

	folder = homefolder;
	std::signal(SIGINT, signal_handler);
	std::signal(SIGTERM, signal_handler);

	if (argc != 4)
	{
		std::string output = argc > 4 ? "Too many arguments." : "Not enough arguments.";
		output += "\nCorrect format for command line input is : AsciiVideoEncoder [Command] [Filename] [Framerate/detail].";
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

	if (functionMap.find(userinput) != functionMap.end() && colorMap.find(userinput) != colorMap.end())
	{
		result = functionMap[userinput](vidname, homefolder, frate, colorMap[userinput]);
	}
	else
	{
		std::print(std::cerr, "Not a function.\n Use -h to list all available functions.");
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
