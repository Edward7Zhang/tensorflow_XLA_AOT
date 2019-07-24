#define EIGEN_USE_THREADS
#define EIGEN_USE_CUSTOM_THREAD_POOL

#include <iostream>
#include <cstdio>
#include <jni.h>
#include <android/log.h>
#include "custom_interface_lib.h"
#include "third_party/eigen3/unsupported/Eigen/CXX11/Tensor"

float* run(float* input, float* output, int input_size, int output_size){
        std::cout << "Load .so SUCCESS" << std::endl;
        Eigen::ThreadPool tp(std::thread::hardware_concurrency());
        Eigen::ThreadPoolDevice device(&tp, tp.NumThreads());
        Classifier classifier;
        classifier.set_thread_pool(&device);

        std::copy(input, input + input_size, classifier.arg0_data());
        auto ok = classifier.Run();
        if (not ok) std::cout << "NOT OK" << std::endl;
//
//        std::cout << "input:";
//        std::cout << input << std::endl;
//
//        std::cout << "input_size:";
//        std::cout << input_size << std::endl;
//
//        std::cout << "classifier.arg0_data():";
//        std::cout << classifier.arg0_data() << std::endl;
//
//        std::cout << "output:";
//        std::cout << output << std::endl;
//
//        std::cout << "output_size:";
//        std::cout << output_size << std::endl;
//
//        std::cout << "result0_data():";
//        std::cout << classifier.result0_data() << std::endl;
//
//        for(int i = 0; i < 30; i++){
//            std::cout << "restul0_";
//            std::cout << i;
//            std::cout << " : ";
//            std::cout << classifier.result0(0,i) << std::endl;
//            __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~OUTPUT== %f~~~~~~~~~~~~~~~\n", classifier.result0(0,i));
//        }

        std::copy(classifier.result0_data(), classifier.result0_data() + output_size, output);

        return output;
    }

#ifndef _Included_com_qihoo_cleandroid_sdk_imageclassfier_core_classfier_process_CustomClassifier
#define _Included_com_qihoo_cleandroid_sdk_imageclassfier_core_classfier_process_CustomClassifier
#ifdef __cplusplus
extern "C" {
#endif
    JNIEXPORT void JNICALL Java_com_qihoo_cleandroid_sdk_imageclassfier_core_classfier_process_CustomClassifier_getPredictResult
      (JNIEnv *env, jobject obj, jobjectArray inputArray, jobjectArray outputArray, jint inputSize, jint outputSize){
        jboolean isCopy = JNI_FALSE;

          jint rows = env->GetArrayLength(inputArray);
//          __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~inputRows== %d~~~~~~~~~~~~~~~\n", rows);
          jobjectArray tempInputArray = (jobjectArray)env->GetObjectArrayElement(inputArray, 0);
//          __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~input: 1~~~~~~~~~~~~~~\n");
          jobjectArray tTempInputArray = (jobjectArray)env->GetObjectArrayElement(tempInputArray, 0);
//          __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~input: 2~~~~~~~~~~~~~~~\n");
          jobjectArray tTTempInputArray = (jobjectArray)env->GetObjectArrayElement(tTempInputArray, 0);
//          __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~input: 3~~~~~~~~~~~~~~~\n");
          jfloat* input = env->GetFloatArrayElements((jfloatArray)tTTempInputArray, 0);
//          __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~input: %f~~~~~~~~~~~~~~~\n", input);


        jobjectArray tempOutputArray = (jobjectArray)env->GetObjectArrayElement(outputArray,0);
//        __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~output: 1~~~~~~~~~~~~~~~\n");
        jfloat* output = env->GetFloatArrayElements((jfloatArray)tempOutputArray, 0);
//        __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~output: %f~~~~~~~~~~~~~~~\n", output);

        jfloat* resultPointer = run(input, output, inputSize, outputSize);

//        jclass floatArrayClz = env->FindClass("[[F");
//        if(floatArrayClz == NULL) return NULL;
//        outputArray = env->NewObjectArray(outputSize, floatArrayClz, NULL );
//        if(outputArray == NULL) return NULL;
        for(int i = 0; i < 1; i++){
            jfloat temp[outputSize];
            jfloatArray floatArray = env->NewFloatArray(outputSize);
//            if(floatArray == NULL) return NULL;
            for(int j=0; j < outputSize; j++){
                temp[j] = *(resultPointer + j);
            }
//            __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~output: 2~~~~~~~~~~~~~~~\n");
            env->SetFloatArrayRegion(floatArray, 0, outputSize, temp);
//            __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~output: 3~~~~~~~~~~~~~~~\n");
            env->SetObjectArrayElement(outputArray, i, floatArray);
//            __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~output: 4~~~~~~~~~~~~~~~\n");
            env->DeleteLocalRef(floatArray);
        }
//        __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~output: %f~~~~~~~~~~~~~~~\n", outputArray[0]);
      }
#ifdef __cplusplus
}
#endif
#endif

