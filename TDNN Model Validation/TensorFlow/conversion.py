import myModel
import tensorflow as tf

model = myModel.load_model(load_weights=True)
model.summary()


