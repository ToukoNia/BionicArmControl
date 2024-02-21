%% Following code is from the Matlab EMG example
% Running this script will put the data into the variable 'traindata' which
% will be in the workspace and is used in the 'trainTDNN' script
% This script does not partition the data
% The function at the end has a commented out section which removes some of
% the classifications.
% Running this file may take some time
%%
fs = 3000;

localfile = matlab.internal.examples.downloadSupportFile("SPT","data/MyoelectricData.zip");
datasetFolder = fullfile(fileparts(localfile),"MyoelectricData");
if ~exist(datasetFolder,"dir")
unzip(localfile,datasetFolder)
end

sds1 = signalDatastore(datasetFolder,IncludeSubFolders=true,SampleRate=fs);
p = endsWith(sds1.Files,"d.mat");
sdssig = subset(sds1,p);
%%
sds2 = signalDatastore(datasetFolder,SignalVariableNames=["motion";"data_indx"],IncludeSubfolders=true);
p = endsWith(sds2.Files,"i.mat");
sdslbl = subset(sds2,p);
%%
signal = preview(sdssig);

for i = 1:8
    ax(i) = subplot(4,2,i);
    plot(signal(:,i))
    title("Channel"+i)
end

linkaxes(ax,"y")
%%
lbls = {};

i = 1;
while hasdata(sdslbl)

    label = read(sdslbl);
    
    idx_start = label{2}(2:end-1)';
    idx_end = [idx_start(2:end)-1;idx_start(end)+(3*fs)];
    
    val = categorical(label{1}(2:end-1)',[1 2 3 4 5 6 7], ...
          ["HandOpen" "HandClose" "WristFlexion" "WristExtension" "Supination" "Pronation" "Rest"]);
    ROI = [idx_start idx_end];

    % In some cases, the number of label values and ROIs are not equal.
    % To eliminate these inconsistencies, remove the extra label value or ROI limits. 
    if numel(val) < size(ROI,1)
        ROI(end,:) = [];
    elseif numel(val) > size(ROI,1)
        val(end) = [];
    end

    lbltable = table(ROI,val);
    lbls{i} = {lbltable};

    i = i+1;
end
%%
lblDS = signalDatastore(lbls);
lblstable = preview(lblDS);
lblstable{1}
%%
DS = combine(sdssig,lblDS);
combinedData = preview(DS)
%%
figure
msk = signalMask(combinedData{2});
plotsigroi(msk,combinedData{1}(:,1))
%%
tDS = transform(DS,@preprocess);
transformedData = preview(tDS)


traindata = readall(tDS);                                                   %This is the line that is important and the variable that the training will use




%%
function Tsds = preprocess(inputDS)

sig = inputDS{1};
roiTable = inputDS{2};

% Remove first and last rest periods from signal
sig(roiTable.ROI(end,2):end,:) = [];
sig(1:roiTable.ROI(1,1),:) = [];

% Shift ROI limits to account for deleting start and end of signal
roiTable.ROI = roiTable.ROI-(roiTable.ROI(1,1)-1);

% Create signal mask
m = signalMask(roiTable);
L = length(sig);

% Obtain sequence of category labels and remove pronation, supination, and rest motions
mask = catmask(m,L);
%idx = ~ismember(mask,{'Pronation','Supination','Rest'});
%mask = mask(idx);
%sig = sig(idx,:);

% % Create new signal mask without pronation and supination categories
% m2 = signalMask(mask);
% m2.SpecifySelectedCategories = true;
% % m2.SelectedCategories = [1 2 3 4 7];
% m2.SelectedCategories = [1 2 3 4];
% mask = catmask(m2);

% Filter and downsample signal data
sigfilt = bandpass(sig,[10 400],3000);
downsig = downsample(sigfilt,3);

% Downsample label data
downmask = downsample(mask,3);

targetLength = 12000;
% Get number of chunks
numChunks = floor(size(downsig,1)/targetLength);

% Truncate signal and mask to integer number of chunks
sig = downsig(1:numChunks*targetLength,:);
mask = downmask(1:numChunks*targetLength);

% Create a cell array containing signal chunks
sigOut = {};
step = 0;

for i = 1:numChunks
    sigOut{i,1} = sig(1+step:i*targetLength,:)';
    step = step+targetLength;
end

% Create a cell array containing mask chunks
lblOut = reshape(mask,targetLength,numChunks)';
lblOut = num2cell(lblOut,2);
    
% Output a two-column cell array with all chunks
Tsds = [sigOut,lblOut];
end
