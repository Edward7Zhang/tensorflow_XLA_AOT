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
        return output;
    }

#ifndef _Included_com_qihoo_cleandroid_sdk_imageclassfier_core_classfier_process_CustomClassifier
#define _Included_com_qihoo_cleandroid_sdk_imageclassfier_core_classfier_process_CustomClassifier
#ifdef __cplusplus
extern "C" {
#endif
    JNIEXPORT jobjectArray JNICALL Java_com_qihoo_cleandroid_sdk_imageclassfier_core_classfier_process_CustomClassifier_getPredictResult
      (JNIEnv *env, jobject obj, jobjectArray inputArray, jobjectArray outputArray, jint inputSize, jint outputSize){
        jboolean isCopy = JNI_FALSE;
        __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~input: %d~~~~~~~~~~~~~~~\n", inputArray);
        jobjectArray tempInputArray = (jobjectArray)env->GetObjectArrayElement(inputArray, 0);
        tempInputArray = (jobjectArray)env->GetObjectArrayElement(tempInputArray, 0);
        tempInputArray = (jobjectArray)env->GetObjectArrayElement(tempInputArray, 0);
        jfloatArray tTempInputArray = (jfloatArray)env->GetObjectArrayElement(tempInputArray, 0);
        jfloat* input = static_cast<jfloat *>(env->GetPrimitiveArrayCritical(tTempInputArray, &isCopy));
        __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~isCopy: %d~~~~~~~~~~~~~~~\n", isCopy);

        jfloatArray tempOutputArray = (jfloatArray)outputArray;
        jfloat* output = static_cast<jfloat *>(env->GetPrimitiveArrayCritical(tempOutputArray, &isCopy));

        __android_log_print(ANDROID_LOG_INFO, "NATIVE", "~~~~~~~~~isCopy: %d~~~~~~~~~~~~~~~\n", isCopy);

        jfloat* resultPointer = run(input, output, inputSize, outputSize);

        jclass floatArrayClz = env->FindClass("[[F");
        if(floatArrayClz == NULL) return NULL;
        jobjectArray resultData = env->NewObjectArray(outputSize, floatArrayClz, NULL );
        if(resultData == NULL) return NULL;
        for(int i = 0; i < 1; i++){
            jfloat temp[outputSize];
            jfloatArray floatArray = env->NewFloatArray(outputSize);
            if(floatArray == NULL) return NULL;
            for(int j=0; j < outputSize; j++){
                temp[j] = *(resultPointer + j);
            }
            env->SetFloatArrayRegion(floatArray, 0, outputSize, temp);
            env->SetObjectArrayElement(resultData, i, floatArray);
            env->DeleteLocalRef(floatArray);
        }
        return resultData;
      }
#ifdef __cplusplus
}
#endif
#endif

