import tensorflow as tf
from tensorflow import keras
import numpy as np
#import csv

def PredictCurrentSignal(threshold):
    stream = "placeholder"
    model=tf.keras.models.load_model('emgTDNNNetwork/model.py')                         #gets the model from the associated folder
    testData=takeSample(stream)                                                                         #loads test data
    probability_model=tf.keras.Sequential([model,tf.keras.layers.Softmax()])            #creates the model
    predictions = probability_model.predict(testData)                                   #uses the model to predict 
    bestPrediction = max(predictions)                                    #finds the index of the value with the highest probability
    if bestPrediction>=threshold:
        prediction=predictions.index(bestPrediction)                     #finds the index of the prediction, and returns it
        return prediction
    else:                                                                #returns a value meaning no emg signal
        return 999

def takeSample(inputDataStream):
    batchSize=256                                                       #sets the batch size for the ANN
    a = np.genfromtxt(inputDataStream, delimiter=',')
    return a[-batchSize:]                                              #takes in the last 200 data parts. Need to alter this into a way that does not take the data into memory
    """

    lines=list()                                                        #has to be a better way
    with open('csv01.csv') as f:    
        for line in csv.reader(f):
            lines.append(line)
            if len(lines) > batchsize:
                lines.pop(0)
        """





