#! /bin/bash
# get data from file & calculate load in percentage
awk -v old="$(awk '{print $0}' cpu.txt)" '
BEGIN{
	split(old,prev," ")
	curr_line=0
}
{
	if (match($1,'/cpu/')){
		curr_line++
		res=($2 + $4 - prev[2 * curr_line - 1])/($2 + $4 + $5 - prev[2 * curr_line - 1] - prev[2 * curr_line]) 
		res*=100
		printf "%.2f\t",res
	}
}
' /proc/stat >> data.txt

# allData=$(awk '{print $0}' data.txt)
# echo $allData > data.txt
exit 0
