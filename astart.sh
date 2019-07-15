#!/bin/bash

#while read line           
#do           
#    echo -e $line
#done <file1

#sed '31q;d' file1

jobnamestart="tr3E2"
file="param"
indirect="IN"
outdirect="OUT"
savedirect="OUT/save"
log="OUT/log"
num=$(wc -l < $indirect/$file)

if [ ! -d  $outdirect ]; then
	mkdir $outdirect
fi

if [ ! -d  $savedirect ]; then
	mkdir $savedirect
fi

maxsize=1024

if [ -e $log ]; then
	actualsize=$(du -k "$log" | cut -f 1)
	if [ $actualsize -ge $maxsize ]; then
		cp $log $log$(date +"%T")
		rm $log
		touch $log
	fi
else
	touch $log
fi

try=1
jobname=$jobnamestart.$try
while [ -e $jobname.sh ]
do
	try=$((try+1))
	jobname=$jobnamestart.$try
done
touch $jobname.sh
echo "#!/bin/bash" >>$jobname.sh
echo "#SBATCH -A eletered" >>$jobname.sh
echo "#SBATCH --job-name="$jobname >>$jobname.sh
echo "#SBATCH --time=12-12:00:00" >>$jobname.sh
echo "#SBATCH --array=1-"$num >>$jobname.sh
echo "#SBATCH --no-requeue" >>$jobname.sh
echo "#SBATCH --mail-type=ALL" >>$jobname.sh
echo "#SBATCH --mail-user=danithered@live.com" >>$jobname.sh
echo >>$jobname.sh 
echo 'srun ./progi $(sed "${SLURM_ARRAY_TASK_ID}q;d"' $indirect/$file')' $jobname'_$SLURM_ARRAY_TASK_ID' >>$jobname.sh

chmod +x $jobname.sh
sbatch ./$jobname.sh
#rm $jobname.sh
echo start: $jobname.sh job submitted at `date +%m.%d_%H:%M:%S` >>$log
 <$indirect/$file