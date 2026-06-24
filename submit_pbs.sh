#!/bin/bash

indirect=IN
outdirect=OUT
simid=mcrs7_2
infile=param

# count simulations
n=$(($(wc -l < "${indirect}/${infile}") - 1))

# load the modules
module load gcc
module load gsl/2.5-gcc-10.2.1-jeumekk

# compile
make clean
make
mkdir -p "${outdirect}"

# submit
qsub -J 1-${n} -v INFILE="${infile}",SIMID="${simid}" -N "${simid}" start_pbs.sh

