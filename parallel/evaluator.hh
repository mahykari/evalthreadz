#ifndef __EVALUATOR_HH__
#define __EVALUATOR_HH__

#include <vector>
#include <string>
#include <utility>

#include "./csv_parser.hh"

const std::string TRAIN = "train";
const size_t FEATURE_COUNT = 20;
const size_t BIAS_IDX = 20;
const size_t CLASS_COUNT = 4;

class Evaluator {
public:
    Evaluator(std::string dir);
    void read_data(int idx);
    void normalize_data();
    void evaluate_model(const std::vector<std::vector<double> >& weights);
    std::pair<int, int> get_status();
    void find_partition_ranges();
    std::vector<std::pair<double, double> > get_partition_range();
    void set_ranges(std::vector<std::pair<double, double> >& r);

    void normalize_data_par();

protected:
    std::vector<std::vector<double> > data;
    std::vector<int> classes;
    CSV_Parser parser;
    std::string dataset_dir;
    int samples, correct;
    std::vector<std::pair<double, double> > feature_ranges;
};

struct Normal_Arg {
    int idx;
    std::pair<double, double> f_range;
    std::vector<std::vector<double> >& data; 
};

void* normalize_feature(void* v);
double dot_product(const std::vector<double>& x, const std::vector<double>& beta);
#endif