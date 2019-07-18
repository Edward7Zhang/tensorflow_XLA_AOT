from keras.preprocessing import image
from keras.applications.imagenet_utils import preprocess_input, decode_predictions

from tensorflow.compiler.aot.custom.predict_model import predict

image_path = input()

x = image.img_to_array(image.load_img(image_path, target_size=(160, 160)))
x = x[None, ...]
x = preprocess_input(x)
y = predict(x)
decode_predictions(y)[0]