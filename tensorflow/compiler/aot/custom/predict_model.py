import numpy as np
from keras.preprocessing import image
import tensorflow as tf

lib_custom_interface = np.ctypeslib.load_library('lib_custom_interface', '/home/majortom/WORKSPACE/tensorflow_XLA_AOT/tensorflow/compiler/aot/custom')
lib_custom_interface.run.argtypes = [
    np.ctypeslib.ndpointer(np.float32, ndim=4, shape=(1, 160, 160, 3), flags=('c', 'a')),
    np.ctypeslib.ndpointer(np.float32, ndim=2, shape=(1, 30), flags=('c', 'a', 'w')),
    np.ctypeslib.ctypes.c_int,
    np.ctypeslib.ctypes.c_int]


def predict_so(In):
    In = np.require(In, np.float32, ('c', 'a'))
    Out = np.require(np.zeros((1, 30)), np.float32, ('c', 'a', 'w'))
    lib_custom_interface.run(In, Out, In.size, Out.size)
    return Out


def predict_pb(x):
    with tf.gfile.GFile('./frozen_custom_010.pb', 'rb') as f:
        graph_def = tf.compat.v1.GraphDef()
        graph_def.ParseFromString(f.read())

        with tf.Graph().as_default() as graph:
            tf.import_graph_def(
                graph_def,
                input_map=None,
                return_elements=None,
                name="",
                op_dict=None,
                producer_op_list=None
            )
            with tf.Session() as sess:
                input = graph.get_tensor_by_name('input:0')

                feed_dict = {
                    input.name: x
                }

                output_prediction = graph.get_tensor_by_name('MobilenetV2/Predictions/Reshape_1:0')
                output_prediction = sess.run(output_prediction, feed_dict)
                return output_prediction


image_path = input()

x = image.img_to_array(image.load_img(image_path, target_size=(160, 160)))
x = x[None, ...]
# x = preprocess_input(x)
# print(x)
z = predict_pb(x)

print(z[0])
y = predict_so(x)
# print(y[0])
# print(decode_predictions(y)[0])
