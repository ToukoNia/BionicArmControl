%% Get Data
%Import Excel File
filename='test3.csv';
data=readmatrix(filename,'NumHeaderLines',1);

%Assign to variable
%inputMatrix=array2timetable(data(:,2:9),"RowTimes",seconds(data(:,1)/1000))
X=[data(1:63196,1:9)]';
T=[data(1:63196,1) data(1:63196,10)]';



%% Create TDNN 
delay = 1:2;
neurons =10;

net=timedelaynet(delay,neurons);
net=configure(net,X,T);
net.numinputs = 8;
net.trainParam.epochs=30;
net=train(net,X,T);

%% Predictions
outputs=net(X); 

%% Evaluation

performance=perform(net,T,outputs); %needs to be altered to test it on untrained data