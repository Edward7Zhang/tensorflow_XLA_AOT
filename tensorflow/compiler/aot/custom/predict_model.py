import numpy as np
from keras.preprocessing import image

lib_custom_interface = np.ctypeslib.load_library('lib_custom_interface', '/home/majortom/WORKSPACE/tensorflow_XLA_AOT/tensorflow/compiler/aot/custom')
lib_custom_interface.run.argtypes = [
    np.ctypeslib.ndpointer(np.float32, ndim=4, shape=(1, 160, 160, 3)),
    np.ctypeslib.ndpointer(np.float32, ndim=2, shape=(1, 30)),
    np.ctypeslib.ctypes.c_int,
    np.ctypeslib.ctypes.c_int]


def predict(In):
    In = np.require(In, np.float32)
    Out = np.require(np.zeros((1, 30)), np.float32)
    lib_custom_interface.run(In, Out, In.size, Out.size)
    return Out


image_path = input()

x = image.img_to_array(image.load_img(image_path, target_size=(160, 160)))
x = x[None, ...]
# x = preprocess_input(x)
# print(x)
y = predict(x)
print(y[0])
# print(decode_predictions(y)[0])
