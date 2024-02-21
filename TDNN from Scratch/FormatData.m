%% Read Data
%Import Excel File
 filename='EMG-data.csv';
 rawdata=readmatrix(filename,'NumHeaderLines',1);
 
 %% Find ends of Tests
 indices = [1];
 for i=1:size(rawdata, 1)-1
     if rawdata(i+1,1) < rawdata(i,1)
         indices = [indices i];
     end
 end
 indices = [indices size(rawdata,1)];

%% Something else
X = {};
T = {};
for i=1:size(indices, 2)-1
    X = [X; rawdata(indices(i)+1:indices(i+1),2:9)'];
    T = [T; {categorical(rawdata(indices(i)+1:indices(i+1),10))'}];
end