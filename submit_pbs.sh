#!/bin/bash

indirect=IN
outdirect=OUT
simid=mcrs2
infile=param

# count simulations
n=$(($(wc -l < "${indirect}/${infile}") - 1))

# load the modules
module load gsl
module load gcc

# compile
make
mkdir -p "${outdirect}"

# submit
qsub -J 1-${n} -v INFILE="${infile}",SIMID="${simid}" -N "${simid}" start_pbs.sh

