#include <iostream>
#include <iomanip>
#include <algorithm>
#include "./evaluator.hh"
#include "./misc_funcs.hh"

Evaluator::Evaluator(std::string dir): dataset_dir(dir), samples(0), correct(0) {}

void Evaluator::read_data(int idx) {
    std::string dname;
    if (idx == -1)
        dname = dataset_dir + TRAIN + CSV_EXT;
    else
        dname = dataset_dir + TRAIN + "_" + std::to_string(idx) + CSV_EXT;
    
    CSV_Data d = parser.parse_file(dname);
    
    data.resize(d.size() - 1);

    for (size_t i = LABEL_ROW + 1; i < d.size(); ++i) {
        for (size_t j = 0; j < FEATURE_COUNT; ++j) {
            data[i - 1].push_back(atof(d[i][j].c_str()));
        }
        classes.push_back(atoi(d[i].back().c_str()));
    }

    samples = int(data.size());
}

void Evaluator::normalize_data() {
    for (size_t i = 0; i < FEATURE_COUNT; ++i) {
        auto f_range = feature_ranges[i];
        double diff = f_range.second - f_range.first;
        double min_val = f_range.first;

        for (size_t j = 0; j < data.size(); ++j)
            data[j][i] = (data[j][i] - min_val) / diff;
    }
}

void Evaluator::evaluate_model(const std::vector<std::vector<double> >& weights) {
    for (size_t i = 0; i < data.size(); ++i) {
        std::vector<double> values(CLASS_COUNT);
        for (size_t j = 0; j < CLASS_COUNT; ++j)
            values[j] = dot_product(data[i], weights[j]);
        
        auto max_it = std::max_element(values.begin(), values.end());
        int guessed_class = int(std::distance(values.begin(), max_it));
        if (guessed_class == classes[i])
            correct++;
    }
}

double dot_product(const std::vector<double>& x,
    const std::vector<double>& beta) {
    double result = 0.0;
    for (size_t i = 0; i < FEATURE_COUNT; ++i)
        result += beta[i] * x[i];
    
    result += beta[BIAS_IDX];
    return result;
}

std::pair<int, int> Evaluator::get_status() { return {correct, samples}; }

void Evaluator::find_partition_ranges() {
    feature_ranges = std::vector<std::pair<double, double> >(FEATURE_COUNT);

    for (size_t i = 0; i < FEATURE_COUNT; ++i) {
        std::vector<double> feature(data.size());
        for (size_t j = 0; j < data.size(); ++j)
            feature[j] = data[j][i];
        
        double min_elem = *std::min_element(feature.begin(), feature.end());
        double max_elem = *std::max_element(feature.begin(), feature.end());
        feature_ranges[i] = {min_elem, max_elem};
    }
}

std::vector<std::pair<double, double> > Evaluator::get_partition_range() {
    return feature_ranges;
}

void Evaluator::set_ranges(std::vector<std::pair<double, double> >& r) {
    feature_ranges = r;
}

void* normalize_feature(void* v) {
    Normal_Arg* na = (Normal_Arg*)(v);
    double diff = na->f_range.second - na->f_range.first;
    double min_val = na->f_range.first;

    for (size_t row = 0; row < na->data.size(); ++row)
        na->data[row][na->idx] = (na->data[row][na->idx] - min_val) / diff;    

    pthread_exit(NULL);
}

void Evaluator::normalize_data_par() {
    std::vector<Normal_Arg*> na(FEATURE_COUNT);
    std::vector<pthread_t> tid_vec(FEATURE_COUNT);

    for (int i = 0; i < int(FEATURE_COUNT); ++i) {
        pthread_t tid;
        na[i] = new Normal_Arg {i, feature_ranges[i], data};
        pthread_create(&tid, NULL, normalize_feature, (void*)(na[i]));
        tid_vec[i] = tid;
    }

    join_all_threads(tid_vec, FEATURE_COUNT);
    for (size_t i = 0; i < FEATURE_COUNT; ++i)
        delete na[i];
}