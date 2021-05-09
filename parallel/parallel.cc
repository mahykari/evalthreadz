#include <algorithm>
#include "./parallel.hh"

double find_accuracy(std::pair<int, int> status) {
    return 100.0 * double(status.first) / double(status.second);
}

int count_datafiles(std::string dataset_dir) {
    int cnt = 0;
    for (cnt = 0; cnt < INF; ++cnt) {
        std::string datafile_name = dataset_dir + TRAIN + "_" +
            std::to_string(cnt) + CSV_EXT;
        if (!file_exists(datafile_name))
            break;        
    }

    return cnt;
}

void* read_partition(void* v) {
    Parallel_Arg* p = (Parallel_Arg*)(v);
    p->pe.read_data(p->idx);
    p->pe.find_partition_ranges();
    pthread_exit(NULL);
}

void* eval_partition(void* v) {
    auto p = (Parallel_Arg*)(v);
    
    p->pe.normalize_data();
    p->pe.evaluate_model(p->weights);
    
    auto s = p->pe.get_status();

    pthread_mutex_lock(&p->mutex);
    p->total.first += s.first;
    p->total.second += s.second;
    pthread_mutex_unlock(&p->mutex);
    
    pthread_exit(NULL);
}

void read_parition_parallel(std::vector<Parallel_Arg*>& pa,
    std::vector<pthread_t>& tid_vec, int count)
{
    for (int i = 0; i < count; ++i) {
        pthread_t tid;
        pthread_create(&tid, NULL, read_partition, (void*)(pa[i]));
        tid_vec[i] = tid;
    }        
    join_all_threads(tid_vec, count);
}

void eval_partition_parallel(std::vector<Parallel_Arg*>& pa,
    std::vector<pthread_t>& tid_vec, int count)
{
        for (int i = 0; i < count; ++i) {
        pthread_t tid;
        pthread_create(&tid, NULL, eval_partition, (void*)(pa[i]));
        tid_vec[i] = tid;
    }        
    join_all_threads(tid_vec, count);
}

void set_global_ranges(std::vector<Parallel_Arg*>& pa, int count) {
    std::vector<std::vector<std::pair<double, double> > > partial_ranges(count);
    for (int i = 0; i < count; ++i) {
        partial_ranges[i] = pa[i]->pe.get_partition_range();
    }

    std::vector<std::pair<double, double> > global_ranges(FEATURE_COUNT);
    for (size_t i = 0; i < FEATURE_COUNT; ++i) {
        std::vector<double> feature_min, feature_max;
        for (int j = 0; j < count; ++j) {
            feature_min.push_back(partial_ranges[j][i].first);
            feature_max.push_back(partial_ranges[j][i].second);
        }

        double min_val = *std::min_element(feature_min.begin(), feature_min.end());
        double max_val = *std::max_element(feature_max.begin(), feature_max.end());
        global_ranges[i] = {min_val, max_val};
    }

    for (int i = 0; i < count; ++i) {
        pa[i]->pe.set_ranges(global_ranges);
    }
}

void evaluate(std::string dataset_dir) {
    CSV_Parser cp;
    std::pair<int, int> total(0, 0);
    auto weights = cp.read_weights(dataset_dir);
    
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    int count = count_datafiles(dataset_dir);
    std::vector<Parallel_Arg*> pa(count);
    std::vector<pthread_t> tid_vec(count);

    for (int i = 0; i < count; ++i) {
        pa[i] = new Parallel_Arg {weights, total, dataset_dir, i,
            mutex, Evaluator(dataset_dir)};
    }

    read_parition_parallel(pa, tid_vec, count);
    set_global_ranges(pa, count);
    eval_partition_parallel(pa, tid_vec, count);

    printf("Accuracy: %.2lf%%\n", find_accuracy(total));
    
    for (size_t i = 0; i < pa.size(); ++i)
        delete pa[i];
}