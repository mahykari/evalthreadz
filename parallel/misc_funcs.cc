#include <sys/stat.h>

#include "./misc_funcs.hh"

std::vector<std::string> tokenize(std::string line, char seperator) {
    int start = 0;
    std::string token;
    std::vector<std::string> tokens;
    for (size_t i = 0; i < line.length(); ++i) {
        if (i == line.length() - 1 && line[i] != seperator) {
            token = line.substr(start, i - start + 1);
            tokens.push_back(token);
        }
        else {
            if(line[i] == seperator){
            token = line.substr(start, i - start);
            start = i + 1;
            tokens.push_back(token);
            }
        }
    }
    return tokens;
}

std::vector<std::string> convert_argv(char* argv[], int argc) {
    std::vector<std::string> res;
    for (int i = 0; i < argc; ++i) {
        res.push_back(std::string(argv[i]));
    }

    return res;
}

bool dir_is_usable(const std::string& dir) {
    return dir != "." && dir != ".." && dir.find("_fifo") == std::string::npos;
}

std::vector<std::string> list_dir(const std::string& dir) {
    std::vector<std::string> res;
    struct dirent* ent;
    DIR* d = opendir(dir.c_str());
    while ((ent = readdir(d)) != NULL) {
        if (dir_is_usable(std::string(ent->d_name)))
            res.push_back(ent->d_name);
    }
    return res;
}

bool file_exists(std::string file_name) {
    struct stat buffer;
    return stat(file_name.c_str(), &buffer) != -1;
}

void join_all_threads(std::vector<pthread_t>& tid_vec, int count) {
    for (int i = 0; i < count; ++i) {
        pthread_join(tid_vec[i], NULL);
    }
}