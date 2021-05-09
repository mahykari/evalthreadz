#include <fstream>
#include "./csv_parser.hh"
#include "./misc_funcs.hh"

CSV_Data CSV_Parser::parse_file(std::string file_name) {
    CSV_Data res;
    std::ifstream csv_file;
    csv_file.open(file_name);
    std::string csv_line;
    
    while (getline(csv_file, csv_line)) {
        CSV_Row cr = tokenize(csv_line, ',');
        res.push_back(cr);
    }

    csv_file.close();
    return res;
}

void CSV_Parser::divide_file(std::string file_name, std::string name_tmplt,
    size_t div_factor) {
    std::vector<std::string> contents;
    std::ifstream csv_file;
    csv_file.open(file_name);
    std::string csv_line;

    while (getline(csv_file, csv_line))
        contents.push_back(csv_line);

    csv_file.close();

    size_t part_size = (contents.size() - 1) / div_factor;

    for (size_t i = 0; i < div_factor - 1; ++i) {
        std::ofstream part_file;
        part_file.open(name_tmplt + std::to_string(i) + CSV_EXT);

        part_file << contents[LABEL_ROW] << '\n';
        for (size_t j = i * part_size + 1; j < (i + 1) * part_size + 1; ++j) {
            part_file << contents[j] << '\n';
        }
        part_file.close();
    }

    std::ofstream last_part;
    last_part.open(name_tmplt + std::to_string(div_factor - 1) + CSV_EXT);
    
    last_part << contents[LABEL_ROW] << '\n';
    for (size_t i = (div_factor - 1) * part_size + 1; i < contents.size(); ++i) {
        last_part << contents[i] << '\n';
    }
    
    last_part.close();
}

std::vector<std::vector<double> >
CSV_Parser::read_weights(std::string dataset_dir) {
    std::vector<std::vector<double> > weights;
    std::string wname = dataset_dir + WEIGHTS + CSV_EXT;
    CSV_Data w = this->parse_file(wname);

    weights.resize(w.size() - 1);

    for (size_t i = LABEL_ROW + 1; i < w.size(); ++i)
        for (size_t j = 0; j < w[i].size(); ++j)
            weights[i - 1].push_back(atof(w[i][j].c_str()));    

    return weights;
}