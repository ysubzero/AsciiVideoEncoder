#include "headers/bmp.hpp"

std::ostream& operator<<(std::ostream& os, const Pixel& p)
{
	os << "R:" << static_cast<int>(p.Red) << " G:" << static_cast<int>(p.Green) << " B:" << static_cast<int>(p.Blue);
	return os;
}
