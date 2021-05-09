#include <cstdio>
#include <vector>
#include <string>
#include <sys/stat.h>
#include "./evaluator.hh"
#include <limits>

const int INF = std::numeric_limits<int>::max();

double find_accuracy(std::pair<int, int> status) {
    return 100.0 * double(status.first) / double(status.second);
}

int main(int argc, char* argv[]) {
    std::string dataset_dir = std::string(argv[1]);
    dataset_dir += "/";
    Evaluator se(dataset_dir);
    CSV_Parser parser;
    
    auto weights = parser.read_weights(dataset_dir);
    
    se.read_data(-1);
    se.find_partition_ranges();
    se.normalize_data();
    se.evaluate_model(weights);
    auto status = se.get_status();
    printf("Accuracy: %.2lf%%\n", find_accuracy(status));
}