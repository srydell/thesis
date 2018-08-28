#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <unistd.h>
#include <string>

/**
* @brief: Appends the current PID to the filename provided and returns the opened file stream
*
* @param: std::string& filename
*
* @return: decltype(auto)
*/
decltype(auto) GetUniqueFile(std::string& filename) {
	std::ofstream file;
	std::stringstream ss;
	ss <<  filename << getpid();
	std::string filename_pid = ss.str();
	file.open(filename_pid, std::ios_base::app);
	return file;
}

int main() {
	std::string s = "hello";
	// Generates hello<PID>
	auto test = GetUniqueFile(s);
	test << "testing2";
}
