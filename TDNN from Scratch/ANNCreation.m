%% Create TDNN 
delay = [1:2];
hiddenlayers = 10;

net=timedelaynet(delay,hiddenlayers);
net.numInputs = 8;


 layers = [ ...
     sequenceInputLayer(8)
     convolution1dLayer(1,32)
     reluLayer
     fullyConnectedLayer(8)
     softmaxLayer
     classificationLayer];
 
 
 
 options = trainingOptions("adam", ...
     MaxEpochs=150, ...
     InitialLearnRate=0.01,...
     Shuffle="every-epoch", ...
     GradientThreshold=1, ...
     Verbose=false, ...
     Plots="training-progress");


 %[Xs,Xi,Ai,Ts] = preparets(net,X,T);


 net=trainNetwork(X,T,layers,options);


%% Predictions
outputs=net(inputMatrix); 

%% Evaluation
performance=perform(net,class,outputs); %needs to be altered to test it on untrained data