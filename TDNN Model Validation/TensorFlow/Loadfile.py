import tensorflow as tf
from tensorflow import keras

def PredictCurrentSignal(threshold):
    model=tf.keras.models.load_model('emgTDNNNetwork/model.py')                         #gets the model from the associated folder
    testData=0                                                                          #loads test data
    probability_model=tf.keras.Sequential([model,tf.keras.layers.Softmax()])            #creates the model
    predictions = probability_model.predict(testData)                                   #uses the model to predict 
    bestPrediction = max(predictions)                                    #finds the index of the value with the highest probability
    if bestPrediction>=threshold:
        prediction=predictions.index(bestPrediction)                     #finds the index of the prediction, and returns it
        return prediction
    else:                                                                #returns a value meaning no emg signal
        return 999

def takeSample(inputDataStream):
    pass

