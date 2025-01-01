import tensorflow as tf
from tensorflow.keras.layers import Conv1D, MaxPooling1D, BatchNormalization, LSTM, Bidirectional
from tensorflow.keras.layers import Dense, Dropout, Activation, Flatten
from keras.models import Sequential
from tensorflow.keras import layers

window_size =1
n_features=8

#Create the input
model = Sequential()
model.add(Conv1D(filters=128, kernel_size = (1), activation='relu', input_shape = (window_size, n_features)))
model.add(Dropout(0.2))
model.add(BatchNormalization())
model.add(MaxPooling1D(pool_size=(1)))

#Add CNN Layers (1 Dimensional)
model.add(Conv1D(64, kernel_size = (1), activation='relu'))
model.add(Activation('relu'))
model.add(Dropout(0.2))
model.add(BatchNormalization())
model.add(MaxPooling1D(pool_size=(1)))
    
model.add(Conv1D(32, kernel_size = (1), activation='relu'))
model.add(Activation('relu'))
model.add(Dropout(0.2))
model.add(BatchNormalization())
model.add(MaxPooling1D(pool_size=(1)))

#Bi-LSTM Added in Series
model.add(Bidirectional(LSTM(32, return_sequences=True)))
model.add(BatchNormalization())
model.add(Flatten())
model.add(Dense(64, activation = "relu"))   
model.add(layers.Dense(10))

#Compile
model.compile(optimizer = 'Adam', loss = "mse")     
model.summary()