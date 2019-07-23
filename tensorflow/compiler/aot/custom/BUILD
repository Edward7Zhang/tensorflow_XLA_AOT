load("//tensorflow/compiler/aot:tfcompile.bzl", "tf_library")

tf_library(
    name = "custom_interface",

    cpp_class = "Classifier",

    graph = "frozen_custom_010.pb",

    config = "custom_interface.config.pbtxt",
)

cc_library(
    name = "library",
    hdrs = ["custom_interface_lib.h", "com_qihoo_cleandroid_sdk_imageclassfier_core_classfier_process_CustomClassifier.h"],
    srcs = ["custom_interface_tfcompile_function.o","custom_interface_tfcompile_metadata.o"],
)

cc_binary(
    name = "lib_custom_interface.so",

    srcs = [
    "com_qihoo_cleandroid_sdk_imageclassfier_core_classfier_process_CustomClassifier.cc",
    ],

    deps = [
    ":library",
    "//tensorflow/compiler/tf2xla:xla_compiled_cpu_function",
    "//tensorflow/core:framework_lite",
    "//tensorflow/compiler/xla:cpu_function_runtime",
    "//tensorflow/compiler/xla/service/cpu:runtime_conv2d",
    "//tensorflow/compiler/xla/service/cpu:runtime_matmul",
    "//third_party/eigen3",

    ],

    linkopts = ["-shared"],

    linkshared = 1,

    copts = ["-fPIC"],
)