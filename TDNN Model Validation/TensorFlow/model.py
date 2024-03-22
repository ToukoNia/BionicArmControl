#    This file was created by
#    MATLAB Deep Learning Toolbox Converter for TensorFlow Models.
#    29-Feb-2024 16:47:48

import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers

def create_model():
    sequenceinput = keras.Input(shape=(None,8))
    conv1d_1 = layers.Conv1D(64, 5, padding="same", name="conv1d_1_")(sequenceinput)
    relu_1 = layers.ReLU()(conv1d_1)
    conv1d_2 = layers.Conv1D(64, 5, padding="same", name="conv1d_2_")(relu_1)
    relu_2 = layers.ReLU()(conv1d_2)
    fc = layers.Dense(7, name="fc_")(relu_2)
    softmax = layers.Softmax()(fc)
    classoutput = softmax

    model = keras.Model(inputs=[sequenceinput], outputs=[classoutput])
    return model
