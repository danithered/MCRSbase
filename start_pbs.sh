#!/bin/bash

# reserve 1 CPU, 1 GB disc space and 1 GB RAM
#PBS -l select=1:ncpus=1:mem=1gb:scratch_local=1gb  
#PBS -l walltime=60:00:00
#PBS -m ae

# define input/output file
outdirect=OUT
indirect=IN
infile=${INFILE}

baseID="${SIMID}_${PBS_ARRAY_INDEX}"
simulationID="${baseID}"
counter=1
while [ -d "${outdirect}/${simulationID}" ]; do
    simulationID="${baseID}_r${counter}"
    counter=$((counter + 1))
done

# Uncomment below, if you are running simulations with the same simID -> jobID will be used as dir name
#simulationID=${PBS_JOBID}

# name a job info file
infofile=${outdirect}/job_info.${PBS_JOBID}

# copy the files to the scratch directory
cd "${PBS_O_WORKDIR}"
cp mcrs "${SCRATCHDIR}/"
cp -r "${indirect}" "${SCRATCHDIR}/"

# go to the scratchdir 
cd "${SCRATCHDIR}"

# create the job info file
mkdir -p "${outdirect}"
touch "${infofile}"

# save a basic info about the job 
echo -e "$PBS_JOBID (${simulationID}) starting at `date` from user ${USER}\n" >> ${infofile}
echo -e "$PBS_JOBID is running on node `hostname -f` in a scratch directory ${SCRATCHDIR}\n" >> ${infofile}

# execute simulation
params=$(sed "$((PBS_ARRAY_INDEX+1))q;d" $indirect/$infile)
./mcrs ${params} ${simulationID} 

# report end
echo -e "${PBS_JOBID} finished at `date`\n" >> ${infofile}

# copy the outputs back to where qsub was called
cp "${infofile}" "${PBS_O_WORKDIR}/"
cp -r "${outdirect}/." "${PBS_O_WORKDIR}/${outdirect}/"
#cp -r "${outdirect}/${simulationID}" "${PBS_O_WORKDIR}/${outdirect}/"

# apply a scratch automatic cleanup utility
#clean_scratch

