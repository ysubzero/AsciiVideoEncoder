#include "headers/AsciiVideoEncoder.hpp"
#include "headers/functions.hpp"

const uint32_t max_threads = std::thread::hardware_concurrency();

bool video(std::string vidname)
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

void ConvertFile(std::vector<std::string> split, std::string directory)
{
	for (auto& file : split)
	{
		ASC::FileToAsciiImage(directory + "\\" + file, directory + "\\ASCII" + file, 4, 8);
		std::print("{0} {1}{2}", "Converted", file, "\n");
	}
}

void thread(std::vector<std::thread>& threads, std::vector<std::string>& outputfiles, std::string directory)
{
	int64_t vect_size = outputfiles.size() / max_threads;

	std::vector<std::vector<std::string>> vectorvector;

	for (int i = 0; i < max_threads; ++i)
	{
		std::vector<std::string> vector;

		if (i <= max_threads - 1) 
		{vector.insert(vector.end(), outputfiles.begin() + (i * vect_size), outputfiles.begin() + ((i + 1) * vect_size));}

		else 
		{vector.insert(vector.end(), outputfiles.begin() + (i * vect_size), outputfiles.end());}

		vectorvector.push_back(vector);
	}

	for (int i = 0; i < max_threads; ++i) {threads.emplace_back(ConvertFile, vectorvector[i], directory);}

	for (auto& t : threads) {if (t.joinable()) { t.join(); }}
}

int VideoToAsciiVideo(std::string vidname, std::string directory)
{
	if (!video(vidname))
	{
		std::print(std::cerr, "Invalid Type\n");
		return 1;
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
		return 1;
	}

	else
	{
		std::print("Command executed successfully!\n");

		std::vector<std::string> outputfiles = FSys::OutputFiles(directory);

		thread(threads, outputfiles, directory);

		std::print("Done.\n");
		unconverted = false;
	}

	if (!unconverted)
	{
		std::string create_video = "ffmpeg -framerate 24 -i \"" + directory + "\\ASCIIoutput_%04d.bmp\" -i \"" + vidname + "\" -map 0:v:0 -map 1:a:0 -vf \"scale=1920:-2,setsar=1:1\" -c:v libx264 -r 24 -y \"" + directory + "\\asciioutput.mkv\"";

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

int ConvertImageToAsciiImage(std::string vidname, std::string directory)
{
	if (video(vidname))
	{
		std::print(std::cerr, "Invalid Type\n");
		return 1;
	}

	std::string command = "ffmpeg -i \"" + vidname + "\" -pix_fmt bgr24 -y " + directory + "\\images\\input.bmp\"";
	std::print("{0}{1}", command, "\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) 
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		return 1;
	}

	std::print("Converted.\n");

	ASC::FileToAsciiImage(directory + "\\images\\input.bmp", directory + "\\images\\output.bmp", 4 , 8);
	std::print("Done.\n");
	return 0;
}

int VideoToConsole(std::string vidname, std::string directory)
{
	if (!video(vidname))
	{
		std::print(std::cerr, "Invalid Type\n");
		return 1;
	}

	std::vector<std::thread> threads;
	threads.resize(max_threads);
	bool unconverted = true;

	std::string command = "ffmpeg -i \"" + vidname + "\" -vf \"fps = 24,scale=-1:480\" -pix_fmt bgr24 -y \"" + directory + "\\output_%04d.bmp\"";
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

		for (auto& s : outputfiles)
		{
			ASC::FileToConsole(directory + "\\" + s, 8, 16);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000/25));
		}
	}

	FSys::deleteTemporary(directory);
	clearScreen();
	return 0;
}

int ImageToText(std::string vidname, std::string directory)
{
	if (video(vidname))
	{
		std::cerr << "Invalid Type\n";
		return 1;
	}

	std::string command = "ffmpeg -i \"" + vidname + "\" -pix_fmt bgr24 -y " + directory + "\\images\\input.bmp\"";
	std::print("{0}{1}", command, "\n");

	int ffmpeg_result = system(command.c_str());

	if (ffmpeg_result != 0) 
	{
		std::print(std::cerr, "ffmpeg failed.\n");
		return 1;
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

	fs::path homeDir(homefolder);

	if (homefolder.empty() || !fs::is_directory(homeDir))
	{
		std::print("Invalid Home Folder! Check your settings!\n");
		return 1;
	}

	if (!fs::exists(homefolder)) 
	{fs::create_directory(homefolder);}

	if (!fs::exists(homefolder + "\\images"))
	{fs::create_directory(homefolder + "\\images");}

	std::print("Clearing cache...\n");
	FSys::deleteTemporary(homefolder);

	std::unordered_map<std::string, std::function<int(std::string, std::string)>> functionMap;
	functionMap["-v"] = VideoToAsciiVideo;
	functionMap["-i"] = ConvertImageToAsciiImage;
	functionMap["-c"] = VideoToConsole;
	functionMap["-t"] = ImageToText;

	if (argc > 3)
	{
		std::print(std::cerr, "Too many arguments.");
		return 1;
	}

	if (argc < 3)
	{
		std::print(std::cerr, "Not enough arguments.");
		return 1;
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
		return 1;
	}

	if (result == 1)
	{
		std::print(std::cerr, "Failed.");
		return 1;
	}

	if (userinput != "-c")
	{std::print("{0} {1}", "\nSaved to", homefolder);}

	else
	{std::print("Playback done.");}

	return 0;
}
