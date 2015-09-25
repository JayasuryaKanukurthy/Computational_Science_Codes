clear all; clc;

display 'creating subset of files';
display 'This might take some time!!';

files = dir('midtermdata/train*.txt');
train = [];
data_set=[];
%Read in RAW data from files
for nm = 1:length(files)
    train_data = dlmread(strcat('midtermdata/',files(nm).name));
    data_set = train_data(1:100,2:end);
    train = [train;data_set];
    
end
for i = 2:length(train(1,:))
    dr = 0;
    if(max(train(:,i))==0)
        dr = 1;
    else
        dr = max(train(:,i));
    end
    train(:,i) = train(:,i)/dr;
end
horz = 1:length(train(1,:));
vert = 0:length(train(:,1));
train = [horz;train];
train = [vert',train];

dlmwrite('Train_Set_Full.txt',train,' ');
