import tensorflow as tf
import myModel
import numpy as np

def PredictCurrentSignal(threshold):
    stream = "placeholder"

    model = myModel.load_model(load_weights=True)
#   model=tf.keras.models.load_model(r"C:\Users\Rayne\Documents\GitHub\BionicArmControl\TDNN Model Validation\TensorFlow\myModel")  # Load the MODEL                                                          #gets the model from the associated folder
    model.load_weights(r'C:\Users\Rayne\Documents\GitHub\BionicArmControl\TDNN Model Validation\TensorFlow\myModel\weights.h5')
    testData=readCSV(stream)                                                            #loads test data
    if testData is None or testData.size == 0:
        return 999  # Return error code if test data is empty or invalid
    predictions = model.predict(testData)                                               #Use the model to predict
    probabilities = tf.nn.softmax(predictions).numpy()                                  #Apply softmax to get probabilities
    
    bestPrediction = np.max(probabilities)                                              #Get the max probability
    prediction_index = np.argmax(probabilities)                                         #Get the index of the max probability

    if bestPrediction >= threshold:
        return prediction_index                                                         #Return the index if prediction meets the threshold
    else:
        return 999                                                                      #Error code if no prediction meets the threshold

def takeSample(inputDataStream):
    batchSize=256                                                       #sets the batch size for the ANN
    a = np.genfromtxt(inputDataStream, delimiter=',')
    return a[-batchSize:]                                              #takes in the last batchSize data parts. Need to alter this into a way that does not take the data into memory

def readCSV(stream):
    try:
        data = np.genfromtxt(stream, delimiter=',', skip_header=1, dtype=np.float32)  # Read CSV directly into NumPy array, skipping header
        return data
    except Exception as e:
        print(f"Error reading CSV file: {e}")
        return None
    
print(PredictCurrentSignal(0.7))



