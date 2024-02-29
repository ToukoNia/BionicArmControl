import tensorflow as tf
from tensorflow import keras
from emgTDNNNetwork import model

model=tf.keras.models.load_model('emgTDNNNetwork/model.py')                         #gets the model from the associated folder
testData=0                                                                          #loads test data
probability_model=tf.keras.Sequential([model,tf.keras.layers.Softmax()])            #creates the model
predictions = probability_model.predict(testData)                                   #uses the model to predict
