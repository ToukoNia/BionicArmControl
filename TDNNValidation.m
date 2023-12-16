% wholeData = readtable("EMG-data.csv");

rf = rowfilter(wholeData);

for i=1:36
    writetable(wholeData(rf.label == i,:),strcat('test',int2str(i),'.csv'));
end 
