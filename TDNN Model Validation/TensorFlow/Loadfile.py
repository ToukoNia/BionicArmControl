import os
import tensorflow as tf
from tensorflow import keras
from emgTDNNNetwork import model

model=tf.keras.models.load_model('emgTDNNNetwork/model.py')
probability_model=tf.keras.Sequential([model,tf.keras.layers.Softmax()])
predictions = probability_model.predict(testData)
