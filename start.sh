#!/bin/bash

maxnum=9 # maximum number of simulations to run at the same time, should be less than the number of cores in the computer
file="param" # naem of the file with parameters for simulations, should be in $indirect
indirect="IN" # name of the directory where the file with parameters is, should be in the same directory as this script
outdirect="OUT" # name of the directory where the output files will be, should be in the same directory as this script


pids=(0)


if [ "$#" -ne 1 ]
then
	echo "Give name to simulations!"
	read -r jobnamestart
else
	jobnamestart=$1
fi

num=$(wc -l < $indirect/$file)

if [ ! -d  $outdirect ]; then
	mkdir $outdirect
fi

try=1
jobname=$jobnamestart.$try
while [ -e ${jobname}_output_1.txt ]
do
	try=$((try+1))
	jobname=$jobnamestart.$try
done

#for i in {2..10}
#for i in {2..${num}}
for ((i=2; i <= ${num}; i+=1))
do
	./mcrs $(sed "${i}q;d" $indirect/$file) ${jobname}_$(( ${i} - 1 )) ./IN/rngseed.bin >> ${outdirect}/${jobname}_output_$(( ${i} - 1 )).txt &
	pid[ $(( i - 2)) ]=$!
	#echo pid no $(( i - 2)) is ${pid[ $(( i - 2)) ]}
	#echo pids are ${pid[@]}
		
	echo pids are ${pid[@]} numrun is $numrun maxnum is $maxnum
		
	numrun=$(( maxnum + 1 ))
	
	while [ "$numrun" -ge "$maxnum" ]
	do
		#echo pid indexes are ${!pid[@]} numrun is $numrun maxnum is $maxnum
		sleep 1
		numrun=0
		for c in ${!pid[@]} 
		do
			#echo c is ${c}
			if kill -0 ${pid[c]}; then
				numrun=$(( numrun + 1 ))
			fi
		done
		#echo numrun is $numrun
	done
done

echo "My work here is done" $jobname
