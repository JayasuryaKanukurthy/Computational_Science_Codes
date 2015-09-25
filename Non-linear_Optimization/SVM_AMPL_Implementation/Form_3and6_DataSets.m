clear all; clc;

train3 = dlmread('midtermdata/train3.txt');

train3_subset = train3(1:500,:);

clear train3;


train6 = dlmread('midtermdata/train6.txt');

train6_subset = train6(1:500,:);

clear train6;

test3 = dlmread('midtermdata/test3.txt');

test3_subset = test3(1:500,:);

clear test3;

test6 = dlmread('midtermdata/test6.txt');

test6_subset = test6(1:500,:);

clear test6;



train_set_20 = [train3_subset;train6_subset];
test_set_20 = [test3_subset;test6_subset];

for i = 2:length(train_set_20(:,1))
    dr = 0;
    if(max(train_set_20(i,2:end))==0)
        dr=1;
    else
        dr = max(train_set_20(i,2:end));
    end
    train_set_20(i,2:end) = train_set_20(i,2:end)/dr;
end

for i = 2:length(test_set_20(:,1))
    dr = 0;
    if(max(test_set_20(i,2:end))==0)
        dr=1;
    else
        dr = max(test_set_20(i,2:end));
    end
    test_set_20(i,2:end) = test_set_20(i,2:end)/dr;
end



horz = 1:length(train_set_20(1,:));
vert = 0:length(train_set_20(:,1));
train_set_20 = [horz;train_set_20];
train_set_20 = [vert',train_set_20];

horz = 1:length(test_set_20(1,:));
vert = 0:length(test_set_20(:,1));
test_set_20 = [horz;test_set_20];
test_set_20 = [vert',test_set_20];

dlmwrite('3and6_Train_set.txt',train_set_20,' ');
dlmwrite('3and6_Test_set.txt',test_set_20,' ');
