%% Create TDNN
% This script sets up the layers for the TDNN network and trains the   
% network using the data found in 'traindata'.
% To aquire the data run the 'setupData' script
% This will take time to run!


%% Layers
% The sequenceInputLayer, fullyConnectedLayer, softmaxLayer and
% classificationLayer layers are necessary for a sequence input classifier.
% The convolution1DLayer and reluLayer are what make it a TDNN (I think)
% Adding more of these two should increase the depth of the network
% (increases training time)
% If using different data don't forget to change the number of inputs and
% outputs.

layers = [ ...
    sequenceInputLayer(8)                                                   % How many inputs there are
    convolution1dLayer(5,64, "Padding","same")                              % Sets up a convolution with an overlap of 5, and 64 filters. It pads the edges with same items.
    reluLayer                                                               % Activation with max thresholding at 0
    convolution1dLayer(5,64, "Padding","same")                              
    reluLayer
    fullyConnectedLayer(7)                                                  % How many classes
    softmaxLayer                                                            % Applies a softmax function which does a loss function
    classificationLayer];                                                   % Calculates the output class

%% Training Options
% These setup how the network will train
% They are quite arbitrary at the moment so could be improved.
% Information abou the different options can be found in the documentation
% at https://uk.mathworks.com/help/deeplearning/ref/trainingoptions.html

options = trainingOptions("adam", ...
    MaxEpochs=150, ...
    InitialLearnRate=0.01,...
    Shuffle="every-epoch", ...
    GradientThreshold=1e5, ...
    Verbose=false, ...
    Plots="training-progress");

%% Train the Network
% Uses deep learning to train the network using the layer structure and
% training options defined previously.
% The data comes from the cell array 'traindata' which comes from running
% the 'setupData script.
% The first column in 'traindata' contains the inputs and the second is the
% output classes

net=trainNetwork(traindata(:,1),traindata(:,2),layers,options);

%% Test the Network
% This measures the accuracy of the network
% It currently uses the training data which is not ideal

predTest = classify(net,traindata(:,1),MiniBatchSize=32);
confusionchart([traindata{:,2}],[predTest{:}],Normalization="column-normalized")

exportNetworkToTensorFlow(net, "emgTDNNNetwork")