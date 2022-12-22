lazyReg = 'Lazy Set Test --- No_of_Threads: (?<thd>\d*) cntPercent: (?<cnt>\d*) duration: (?<dur>[\d\.]*)';
optReg = 'Optimistic Set Test --- No_of_Threads: (?<thd>\d*) cntPercent: (?<cnt>\d*) duration: (?<dur>[\d\.]*)';
fineReg = 'Fine Set Test --- No_of_Threads: (?<thd>\d*) cntPercent: (?<cnt>\d*) duration: (?<dur>[\d\.]*)';

text = fileread('listTest.txt');

lazyMatch = regexp(text, lazyReg, 'names');
optMatch = regexp(text, optReg, 'names');
fineMatch = regexp(text, fineReg, 'names');

format long g

for cnt = 1:length(lazyMatch)
    if cnt>15
        lazyMatch(cnt).max = '1024';
        optMatch(cnt).max = '1024';
        fineMatch(cnt).max = '1024';
    else
        lazyMatch(cnt).max = '8';
        optMatch(cnt).max = '8';
        fineMatch(cnt).max = '8';
    end
    lazyMatch(cnt).max = str2double(lazyMatch(cnt).max);
    lazyMatch(cnt).thd = str2double(lazyMatch(cnt).thd);
    lazyMatch(cnt).cnt = str2double(lazyMatch(cnt).cnt);
    lazyMatch(cnt).dur = str2double(lazyMatch(cnt).dur);

    optMatch(cnt).max = str2double(optMatch(cnt).max);
    optMatch(cnt).thd = str2double(optMatch(cnt).thd);
    optMatch(cnt).cnt = str2double(optMatch(cnt).cnt);
    optMatch(cnt).dur = str2double(optMatch(cnt).dur);

    fineMatch(cnt).max = str2double(fineMatch(cnt).max);
    fineMatch(cnt).thd = str2double(fineMatch(cnt).thd);
    fineMatch(cnt).cnt = str2double(fineMatch(cnt).cnt);
    fineMatch(cnt).dur = str2double(fineMatch(cnt).dur);
end

%%