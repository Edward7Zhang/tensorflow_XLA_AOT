#define EIGEN_USE_THREADS
#define EIGEN_USE_CUSTOM_THREAD_POOL

#include <iostream>
#include "out.h"
#include "third_party/eigen3/unsupported/Eigen/CXX11/Tensor"

extern "C" int run(float* input, float* output, int input_size, int output_size){
    std::cout << "Load .so SUCCESS" << std::endl;
    Eigen::ThreadPool tp(std::thread::hardware_concurrency());
    Eigen::ThreadPoolDevice device(&tp, tp.NumThreads());
    Classifier classifier;
    classifier.set_thread_pool(&device);

    std::copy(input, input + input_size, classifier.arg0_data());
    auto ok = classifier.Run();
    if (not ok) return -1;
    std::copy(classifier.result0_data(), classifier.result0_data() + output_size, output);
    return 0;
}
