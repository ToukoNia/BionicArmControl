import tensorflow as tf
from tensorflow import keras
import numpy as np
import csv

def PredictCurrentSignal(threshold):
    stream = "test36.csv"
    model=tf.keras.layers.TFSMLayer(r'C:\Users\Rayne\Downloads\myModel\myModel\model.py', call_endpoint='serving_default')                         #gets the model from the associated folder
    testData=readCSV(stream)                                                            #loads test data
    probability_model=tf.keras.Sequential([model,tf.keras.layers.Softmax()])            #creates the model
    predictions = probability_model.predict(testData)                                   #uses the model to predict 
    bestPrediction = max(predictions)                                                   #finds the index of the value with the highest probability
    if bestPrediction>=threshold:
        prediction=predictions.index(bestPrediction)                                    #finds the index of the prediction, and returns it
        return prediction
    else:                                                                               #returns a value meaning no emg signal
        return 999                                                                      #error code

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
def readCSV(stream):
    with open(stream) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        lines=list()
        for row in csv_reader:
            if line_count == 0:
                print(f'Column names are {", ".join(row)}')
                line_count += 1
            else:
                lines.append(row)
        return lines
    
print(PredictCurrentSignal(0.7))



