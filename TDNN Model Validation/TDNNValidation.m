%% Get Data
%Import Excel File
filename='test1.csv';
data=readmatrix(filename,'NumHeaderLines',1);

%Assign to variable
%inputMatrix=array2timetable(data(:,2:9),"RowTimes",seconds(data(:,1)/1000))
x=[data(:,1:9)]';
X=con2seq(x);
%needs to be fixed so it dosen't hurt peoples brains
%X=combine(data(:,1),data(:,2),data(:,3),data(:,4),data(:,5),data(:,6),data(:,7),data(:,8),data(:,9));
t=[data(:,1) data(:,10)]';
T=con2seq(t);


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