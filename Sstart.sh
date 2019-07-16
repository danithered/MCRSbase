#!/bin/bash

jobnamestart="tr3E2"
file="param"
indirect="IN"
outdirect="OUT"
num=$(wc -l < $indirect/$file)

if [ ! -d  $outdirect ]; then
	mkdir $outdirect
fi

maxsize=1024

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
echo "#SBATCH --array=2-"$num >>$jobname.sh
echo "#SBATCH --no-requeue" >>$jobname.sh
echo "#SBATCH --mail-type=ALL" >>$jobname.sh
echo "#SBATCH --mail-user=danithered@live.com" >>$jobname.sh
echo >>$jobname.sh 
echo 'srun ./mcrs $(sed "${SLURM_ARRAY_TASK_ID}q;d"' $indirect/$file')' $jobname'_$SLURM_ARRAY_TASK_ID' >>$jobname.sh

chmod +x $jobname.sh
#sbatch ./$jobname.sh
#rm $jobname.sh

 <$indirect/$file
