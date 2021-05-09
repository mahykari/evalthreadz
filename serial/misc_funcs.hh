#ifndef __MISC_FUNCS_HH__
#define __MISC_FUNCS_HH__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <pthread.h>

std::vector<std::string> tokenize(std::string line, char seperator);
std::vector<std::string> convert_argv(char* argv[], int argc);
bool dir_is_usable(const std::string& dir);
std::vector<std::string> list_dir(const std::string& dir);
bool file_exists(std::string file_name);

void join_all_threads(std::vector<pthread_t>& tid_vec, int count);

template<typename T>
std::ostream& operator<<(std::ostream& out, std::vector<T>& vec) {
	out << '[';
	for (auto it = vec.begin(); it != vec.end(); it++) {
		if (it != vec.begin())
			out << ", ";
		out << *it;
	}
	out << ']';
	return out;
}

#endif