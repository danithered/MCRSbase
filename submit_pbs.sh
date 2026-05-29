#!/bin/bash

indirect=IN
simid=mcrs_mapping1
infile=param2

# count simulations
n=$(($(wc -l < "${indirect}/${infile}") - 1))

# load the modules
module load gsl
module load gcc

# compile
make

# submit
qsub -J 1-${n} -v INFILE="${infile}" SID="${simid}" -N "${simid}" start_pbs.sh

