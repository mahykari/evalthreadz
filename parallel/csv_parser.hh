#ifndef __CSV_PARSER_HH__
#define __CSV_PARSER_HH__

#include <vector>
#include <string>
#include "./misc_funcs.hh"

const size_t LABEL_ROW = 0;
const char COMMA = ',';
const std::string CSV_EXT = ".csv";
const std::string WEIGHTS = "weights";

typedef std::vector<std::string> CSV_Row;
typedef std::vector<CSV_Row> CSV_Data;

class CSV_Parser {
public:
    CSV_Data parse_file(std::string file_name);
    void divide_file(std::string file_name, std::string name_tmplt, size_t div_factor);
    std::vector<std::vector<double> > read_weights(std::string dataset_dir);
};

#endif