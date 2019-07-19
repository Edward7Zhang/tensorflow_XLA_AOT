#define EIGEN_USE_THREADS
#define EIGEN_USE_CUSTOM_THREAD_POOL

#include <iostream>
#include <cstdio>
#include "custom_interface_lib.h"
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
    std::cout << "input:";
    std::cout << input << std::endl;

    std::cout << "input_size:";
    std::cout << input_size << std::endl;

    std::cout << "classifier.arg0_data():";
    std::cout << classifier.arg0_data() << std::endl;

    std::cout << "output:";
    std::cout << output << std::endl;

    std::cout << "output_size:";
    std::cout << output_size << std::endl;

    std::cout << "result0_data():";
    std::cout << classifier.result0_data() << std::endl;

    for(int i = 0; i < 30; i++){
        std::cout << "restul0_";
        std::cout << i;
        std::cout << " : ";
        std::cout << classifier.result0(0,i) << std::endl;
    }

    std::copy(classifier.result0_data(), classifier.result0_data() + output_size, output);
    return 0;
}
