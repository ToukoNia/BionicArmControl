fs=3000; %sampling freq
csvfile = 'data.csv';

data = readmatrix(csvfile);
timesample = data(:, 1);  % Time in microseconds
sensors = data(:, 2:end-1);  % Sensor data (middle columns)
gesture = data(:, end);  % Gesture labels (end)
numSensors = size(sensors,2)
for i=1:min(numSensors)
    ax(i)=subplot(numSensors,1,i);
    plot(sensors(:,i));
    title("Channel "+i);
end
linkaxes(ax,'y')

idx_start=find(diff([0;gesture]~=0));
idx_end = [idx_start(2:end) - 1; length(gesture)];

val = categorical(gestures(idx_start),[1 2 3 4 5 6 7], ["Elbow Flex", "Supinate", "Rest", "Open Hand", "Pronate", "Close Hand", "Elbow Extension"]) %need to map to ours
ROI = [idx_start,idx_end]
lbltable = table(ROI,val)

sig = sensors;
roiTable = lbltable;

m=signalMask(roiTable);
L=length(sig);
mask=catmask(m,L);
sigfilt=bandpass(sig,[10,400],fs);
downsig=downsample(sigfilt,3);
downmask=downsample(mask,3)

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