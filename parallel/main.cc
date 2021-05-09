#include "./evaluator.hh"
#include "./csv_parser.hh"
#include "./parallel.hh"

int main(int argc, char* argv[]) {
    std::string dataset_dir(argv[1]);
    dataset_dir += "/";
    evaluate(dataset_dir);
}