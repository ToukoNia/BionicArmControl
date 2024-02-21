%% Get Data
%Import Excel File
filename='test1.csv';
data=readmatrix(filename,'NumHeaderLines',1);

%Assign to variable
%inputMatrix=array2timetable(data(:,2:9),"RowTimes",seconds(data(:,1)/1000))
x=[data(:,1:9)];
c1=size(x,2);
r1=length(x);
x=reshape(x,c1,r1);
X=con2seq(x);

%Manual transposition???
% t=[data(:,1) data(:,10)];
% c1=size(t,2);
% r1=length(t);
% t=reshape(t,c1,r1);
% T=con2seq(t);


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