% Import the data
P = "C:\Users\adbot\GitHub\BionicArmControl\Pattern Recognition\Data Collection\LabelledData";
S = dir(fullfile(P,'*_labelled.csv')); 

rawdata=[];
[S.Mass] = deal('');
[S.Height] = deal('');
for k = 1:numel(S)
    F = fullfile(P,S(k).name);
    S(k).data = readtable(F);
    rawdata=[rawdata;S(k).data(2:end,:)];
end

rawdata = convertvars(rawdata, 'Label', 'categorical'); % convert 'label' column to categorical for one-hot encoding

% One-hot encoding removes any false relationships that could be interpreted having the classes as numbers
data = [rawdata(:,1) rawdata(:,2) rawdata(:,3) onehotencode(rawdata(:,4))]; 

trainData = [];
valData = [];

% 
% % define network layers
% layers = [ ...
%     sequenceInputLayer(8)
% 
%     convolution1dLayer(25,32, "Padding","same")
%     batchNormalizationLayer
%     reluLayer
% 
%     convolution1dLayer(25,32, "Padding","same")
%     batchNormalizationLayer
%     reluLayer
% 
%     convolution1dLayer(25,32, "Padding","same")
%     batchNormalizationLayer
%     reluLayer
% 
%     dropoutLayer(0.2)
% 
%     fullyConnectedLayer(7)
%     softmaxLayer];
% 
% % display network design
% analyzeNetwork(layers)
% 
% % training options
% options = trainingOptions("adam", ...
%     InitialLearnRate=0.001, ...
%     MaxEpochs=50, ...
%     MiniBatchSize=128, ...
%     Shuffle="every-epoch", ...
%     ValidationData=valData, ...
%     ValidationFrequency=10, ...
%     Plots="training-progress", ...
%     Metrics="accuracy", ...
%     L2Regularization=0.0001, ...
%     ExecutionEnvironment="auto", ...
%     Verbose=true);
% 
% net = trainnet(trainData,layers,"crossentropy",options);