#    This file was created by
#    MATLAB Deep Learning Toolbox Converter for TensorFlow Models.
#    23-Nov-2024 15:03:46

import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers

def create_model():
    sequence = keras.Input(shape=(None,13,24,8))
    seqfold_out,seqfold_miniBatchSize = SequenceFoldingLayer((13,24,8))(sequence)
    Inception_1a_3x3_reduce = layers.Conv2D(16, (1,1), name="Inception_1a_3x3_reduce_")(seqfold_out)
    Inception_1a_3x3_relu_reduce = layers.ReLU()(Inception_1a_3x3_reduce)
    Inception_1a_3x3_prepadded = layers.ZeroPadding2D(padding=((1,1),(1,1)))(Inception_1a_3x3_relu_reduce)
    Inception_1a_3x3 = layers.Conv2D(18, (3,3), name="Inception_1a_3x3_")(Inception_1a_3x3_prepadded)
    Inception_1a_1x1 = layers.Conv2D(18, (1,1), name="Inception_1a_1x1_")(seqfold_out)
    Inception_1a_pool_prepadded = layers.ZeroPadding2D(padding=((1,1),(1,1)))(seqfold_out)
    Inception_1a_pool = layers.MaxPool2D(pool_size=(3,3), strides=(1,1))(Inception_1a_pool_prepadded)
    Inception_1a_pool_proj = layers.Conv2D(18, (1,1), name="Inception_1a_pool_proj_")(Inception_1a_pool)
    Inception_1a_5x5_reduce = layers.Conv2D(16, (1,1), name="Inception_1a_5x5_reduce_")(seqfold_out)
    Inception_1a_5x5_relu_reduce_2 = layers.ReLU()(Inception_1a_5x5_reduce)
    Inception_1a_5x5_prepadded = layers.ZeroPadding2D(padding=((2,2),(2,2)))(Inception_1a_5x5_relu_reduce_2)
    Inception_1a_5x5 = layers.Conv2D(18, (5,5), name="Inception_1a_5x5_")(Inception_1a_5x5_prepadded)
    depthcat_1a = layers.Concatenate(axis=-1)([Inception_1a_3x3, Inception_1a_5x5, Inception_1a_pool_proj, Inception_1a_1x1])
    Inception_1a_relu = layers.ReLU()(depthcat_1a)
    Inception_1b_pool_prepadded = layers.ZeroPadding2D(padding=((1,1),(1,1)))(Inception_1a_relu)
    Inception_1b_pool = layers.MaxPool2D(pool_size=(3,3), strides=(1,1))(Inception_1b_pool_prepadded)
    Inception_1b_pool_proj = layers.Conv2D(18, (1,1), name="Inception_1b_pool_proj_")(Inception_1b_pool)
    Inception_1b_3x3_reduce = layers.Conv2D(16, (1,1), name="Inception_1b_3x3_reduce_")(Inception_1a_relu)
    Inception_1b_3x3_relu_reduce = layers.ReLU()(Inception_1b_3x3_reduce)
    Inception_1b_3x3_prepadded = layers.ZeroPadding2D(padding=((1,1),(1,1)))(Inception_1b_3x3_relu_reduce)
    Inception_1b_3x3 = layers.Conv2D(18, (3,3), name="Inception_1b_3x3_")(Inception_1b_3x3_prepadded)
    Inception_1b_1x1 = layers.Conv2D(18, (1,1), name="Inception_1b_1x1_")(Inception_1a_relu)
    Inception_1b_5x5_reduce = layers.Conv2D(16, (1,1), name="Inception_1b_5x5_reduce_")(Inception_1a_relu)
    Inception_1b_5x5_relu_reduce_2 = layers.ReLU()(Inception_1b_5x5_reduce)
    Inception_1b_5x5_prepadded = layers.ZeroPadding2D(padding=((2,2),(2,2)))(Inception_1b_5x5_relu_reduce_2)
    Inception_1b_5x5 = layers.Conv2D(18, (5,5), name="Inception_1b_5x5_")(Inception_1b_5x5_prepadded)
    depthcat_1b = layers.Concatenate(axis=-1)([Inception_1b_5x5, Inception_1b_1x1, Inception_1b_3x3, Inception_1b_pool_proj])
    Inception_1b = layers.ReLU()(depthcat_1b)
    Inception_1c_pool_prepadded = layers.ZeroPadding2D(padding=((1,1),(1,1)))(Inception_1b)
    Inception_1c_pool = layers.MaxPool2D(pool_size=(3,3), strides=(1,1))(Inception_1c_pool_prepadded)
    Inception_1c_pool_proj = layers.Conv2D(18, (1,1), name="Inception_1c_pool_proj_")(Inception_1c_pool)
    Inception_1c_3x3_reduce = layers.Conv2D(16, (1,1), name="Inception_1c_3x3_reduce_")(Inception_1b)
    Inception_1c_3x3_relu_reduce = layers.ReLU()(Inception_1c_3x3_reduce)
    Inception_1c_3x3_prepadded = layers.ZeroPadding2D(padding=((1,1),(1,1)))(Inception_1c_3x3_relu_reduce)
    Inception_1c_3x3 = layers.Conv2D(18, (3,3), name="Inception_1c_3x3_")(Inception_1c_3x3_prepadded)
    Inception_1c_5x5_reduce = layers.Conv2D(16, (1,1), name="Inception_1c_5x5_reduce_")(Inception_1b)
    Inception_1c_5x5_relu_reduce_2 = layers.ReLU()(Inception_1c_5x5_reduce)
    Inception_1c_5x5_prepadded = layers.ZeroPadding2D(padding=((2,2),(2,2)))(Inception_1c_5x5_relu_reduce_2)
    Inception_1c_5x5 = layers.Conv2D(18, (5,5), name="Inception_1c_5x5_")(Inception_1c_5x5_prepadded)
    Inception_1c_1x1 = layers.Conv2D(18, (1,1), name="Inception_1c_1x1_")(Inception_1b)
    depthcat_1c = layers.Concatenate(axis=-1)([Inception_1c_1x1, Inception_1c_3x3, Inception_1c_5x5, Inception_1c_pool_proj])
    addition_1ac = layers.Add()([Inception_1a_relu, depthcat_1c])
    Inception_1c = layers.ReLU()(addition_1ac)
    Inception_1d_5x5_reduce = layers.Conv2D(16, (1,1), name="Inception_1d_5x5_reduce_")(Inception_1c)
    Inception_1d_5x5_relu_reduce_2 = layers.ReLU()(Inception_1d_5x5_reduce)
    Inception_1d_5x5_prepadded = layers.ZeroPadding2D(padding=((2,2),(2,2)))(Inception_1d_5x5_relu_reduce_2)
    Inception_1d_5x5 = layers.Conv2D(18, (5,5), name="Inception_1d_5x5_")(Inception_1d_5x5_prepadded)
    Inception_1d_pool_prepadded = layers.ZeroPadding2D(padding=((1,1),(1,1)))(Inception_1c)
    Inception_1d_pool = layers.MaxPool2D(pool_size=(3,3), strides=(1,1))(Inception_1d_pool_prepadded)
    Inception_1d_pool_proj = layers.Conv2D(18, (1,1), name="Inception_1d_pool_proj_")(Inception_1d_pool)
    Inception_1d_1x1 = layers.Conv2D(18, (1,1), name="Inception_1d_1x1_")(Inception_1c)
    Inception_1d_3x3_reduce = layers.Conv2D(16, (1,1), name="Inception_1d_3x3_reduce_")(Inception_1c)
    Inception_1d_3x3_relu_reduce = layers.ReLU()(Inception_1d_3x3_reduce)
    Inception_1d_3x3_prepadded = layers.ZeroPadding2D(padding=((1,1),(1,1)))(Inception_1d_3x3_relu_reduce)
    Inception_1d_3x3 = layers.Conv2D(18, (3,3), name="Inception_1d_3x3_")(Inception_1d_3x3_prepadded)
    depthcat_1d = layers.Concatenate(axis=-1)([Inception_1d_1x1, Inception_1d_3x3, Inception_1d_5x5, Inception_1d_pool_proj])
    Inception_1d = layers.ReLU()(depthcat_1d)
    Inception_1e_pool_prepadded = layers.ZeroPadding2D(padding=((1,1),(1,1)))(Inception_1d)
    Inception_1e_pool = layers.MaxPool2D(pool_size=(3,3), strides=(1,1))(Inception_1e_pool_prepadded)
    Inception_1e_pool_proj = layers.Conv2D(18, (1,1), name="Inception_1e_pool_proj_")(Inception_1e_pool)
    Inception_1e_1x1 = layers.Conv2D(18, (1,1), name="Inception_1e_1x1_")(Inception_1d)
    Inception_1e_5x5_reduce = layers.Conv2D(16, (1,1), name="Inception_1e_5x5_reduce_")(Inception_1d)
    Inception_1e_5x5_relu_reduce_2 = layers.ReLU()(Inception_1e_5x5_reduce)
    Inception_1e_5x5_prepadded = layers.ZeroPadding2D(padding=((2,2),(2,2)))(Inception_1e_5x5_relu_reduce_2)
    Inception_1e_5x5 = layers.Conv2D(18, (5,5), name="Inception_1e_5x5_")(Inception_1e_5x5_prepadded)
    Inception_1e_3x3_reduce = layers.Conv2D(16, (1,1), name="Inception_1e_3x3_reduce_")(Inception_1d)
    Inception_1e_3x3_relu_reduce = layers.ReLU()(Inception_1e_3x3_reduce)
    Inception_1e_3x3_prepadded = layers.ZeroPadding2D(padding=((1,1),(1,1)))(Inception_1e_3x3_relu_reduce)
    Inception_1e_3x3 = layers.Conv2D(18, (3,3), name="Inception_1e_3x3_")(Inception_1e_3x3_prepadded)
    depthcat_1e = layers.Concatenate(axis=-1)([Inception_1e_1x1, Inception_1e_3x3, Inception_1e_5x5, Inception_1e_pool_proj])
    addition_1ce = layers.Add()([depthcat_1e, Inception_1c])
    Inception_1e = layers.ReLU()(addition_1ce)
    Inception_1f_3x3_reduce = layers.Conv2D(16, (1,1), name="Inception_1f_3x3_reduce_")(Inception_1e)
    Inception_1f_3x3_relu_reduce = layers.ReLU()(Inception_1f_3x3_reduce)
    Inception_1f_3x3_prepadded = layers.ZeroPadding2D(padding=((1,1),(1,1)))(Inception_1f_3x3_relu_reduce)
    Inception_1f_3x3 = layers.Conv2D(18, (3,3), name="Inception_1f_3x3_")(Inception_1f_3x3_prepadded)
    Inception_1f_3x3_relu = layers.ReLU()(Inception_1f_3x3)
    Inception_1f_pool_prepadded = layers.ZeroPadding2D(padding=((1,1),(1,1)))(Inception_1e)
    Inception_1f_pool = layers.MaxPool2D(pool_size=(3,3), strides=(1,1))(Inception_1f_pool_prepadded)
    Inception_1f_pool_proj = layers.Conv2D(18, (1,1), name="Inception_1f_pool_proj_")(Inception_1f_pool)
    Inception_1f_relu_pool_proj = layers.ReLU()(Inception_1f_pool_proj)
    Inception_1f_1x1 = layers.Conv2D(18, (1,1), name="Inception_1f_1x1_")(Inception_1e)
    Inception_1f_1x1_relu = layers.ReLU()(Inception_1f_1x1)
    Inception_1f_5x5_reduce = layers.Conv2D(16, (1,1), name="Inception_1f_5x5_reduce_")(Inception_1e)
    Inception_1f_5x5_relu_reduce_2 = layers.ReLU()(Inception_1f_5x5_reduce)
    Inception_1f_5x5_prepadded = layers.ZeroPadding2D(padding=((2,2),(2,2)))(Inception_1f_5x5_relu_reduce_2)
    Inception_1f_5x5 = layers.Conv2D(18, (5,5), name="Inception_1f_5x5_")(Inception_1f_5x5_prepadded)
    Inception_1f_5x5_relu = layers.ReLU()(Inception_1f_5x5)
    depthcat_1f = layers.Concatenate(axis=-1)([Inception_1f_1x1_relu, Inception_1f_3x3_relu, Inception_1f_5x5_relu, Inception_1f_relu_pool_proj])
    CCNormLayer = layers.Lambda(lambda X: tf.nn.local_response_normalization(X, depth_radius=2.000000, bias=2.000000, alpha=0.000020, beta=0.750000))
    crossnorm_1 = CCNormLayer(depthcat_1f)
    sequnfold = SequenceUnfoldingLayer((13,24,72))(crossnorm_1, seqfold_miniBatchSize)
    sequnfoldperm = layers.TimeDistributed(layers.Permute((3,2,1)))(sequnfold)
    flatten = layers.TimeDistributed(layers.Flatten())(sequnfoldperm)
    flatten_lstm_input = flatten
    lstm = layers.LSTM(128, name='lstm_', activation='tanh', recurrent_activation='sigmoid', return_sequences=True, return_state=False)(flatten_lstm_input)
    fc_1 = layers.Dense(6, name="fc_1_")(lstm)
    softmax = layers.Softmax()(fc_1)
    classoutput = softmax

    model = keras.Model(inputs=[sequence], outputs=[classoutput])
    return model

## Helper layers:

class SequenceFoldingLayer(tf.keras.layers.Layer):
    def __init__(self, dataShape, name=None):
        super(SequenceFoldingLayer, self).__init__(name=name)
        self.dataShape = dataShape;

    def call(self, input):
        # Two outputs: Y and batchSize
        shape = tf.shape(input)
        return tf.reshape(input, (-1,) + self.dataShape), shape[0]


class SequenceUnfoldingLayer(tf.keras.layers.Layer):
    def __init__(self, dataShape, name=None):
        super(SequenceUnfoldingLayer, self).__init__(name=name)
        self.dataShape = dataShape;

    def call(self, X, batchSize):
        return tf.reshape(X, (batchSize, -1) + self.dataShape)
