#ifndef __PARALLEL_HH__
#define __PARALLEL_HH__

#include <vector>
#include <string>
#include <utility>
#include <limits>
#include "./evaluator.hh"

const int INF = std::numeric_limits<int>::max();

struct Parallel_Arg {
    const std::vector<std::vector<double> >& weights;
    std::pair<int, int>& total;
    std::string dataset_dir;
    int idx;
    pthread_mutex_t& mutex;
    Evaluator pe;
};

double find_accuracy(std::pair<int, int> status);
int count_datafiles(std::string dataset_dir);

void* read_partition(void* v);
void* eval_partition(void* v);

void join_all_threads(std::vector<pthread_t>& tid_vec, int count);

void read_parition_parallel(std::vector<Parallel_Arg*>& pa,
    std::vector<pthread_t>& tid_vec, int count);

void eval_partition_parallel(std::vector<Parallel_Arg*>& pa,
    std::vector<pthread_t>& tid_vec, int count);

void set_global_ranges(std::vector<Parallel_Arg*>& pa, int count);

void evaluate(std::string dataset_dir);

#endif