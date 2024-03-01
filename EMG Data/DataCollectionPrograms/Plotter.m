%% Get Data
%Import Excel File
filename='Curl.txt';
data=readmatrix(filename,'NumHeaderLines',1);

%% Plot
%Time = [0:size(data)(1)]
plot(data)