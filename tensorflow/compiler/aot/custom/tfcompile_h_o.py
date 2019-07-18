import os

os.popen('cd ~/WORKSPACE/tensorflow_XLA_AOT/bazel-bin/tensorflow/compiler/aot \
        && \
        ./tfcompile \
        --graph=frozen_custom_010.pb \
        --config=custom_interface.config.pbtxt \
        --cpp_class="Classifier"')
