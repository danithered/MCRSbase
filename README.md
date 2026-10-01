[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.23085738.svg)](https://doi.org/10.5281/zenodo.23085738)

# General description

This repository contains a basic implementation of the Metabolically Coupled Replicator Systems model [(Czárán & Szathmáry, 2000)](https://www.academia.edu/download/46088678/Coexistence_of_metabolically_co-operatin20160531-13532-1y4jzbp.pdf), [(Czárán et al., 2015)](https://doi.org/10.1016/j.jtbi.2015.06.002). The main difference to the Czárán & Szatmáry 2000 article is that in this program the size of metabolic / replicator neighbourhoods can be manually set and the function that calculates the efficiency of the local metabolism can be switched as well (see details below). For a detailed description of the model see the original articles.

# Install dependencies

Install gsl [GNU Scientific Library](https://www.gnu.org/software/gsl/). On Debian based systems:

```
wget https://mirror.ibcp.fr/pub/gnu/gsl/gsl-latest.tar.gz
tar -xfv gsl-latest.tar.gz
cd gsl-XX
./configure
make
sudo make install
```

# Download

Download either the .zip file from the Releases / Zenodo and unpack it, or clone it with git:

```
git clone git@github.com:danithered/MCRSbase.git
```

# Compile and prepare for simulations

```
make
mkdir IN
mkdir OUT
chmod +x generalas.sh
chmod +x start.sh
```

# Usage

Simulations can be started with the `./mcrs` executable file with a fixed argument set. The arguments are as follows:

- number of columns of the grid
- maximum length of the simulation (generations)
- size of the metabolic neighbourhood: in units of square radius of the circle drawn around the focal cell - e.g., value 1 yields a circle that includes the vonNeumann neighbourhood with 5 cells 
- size of replication neighbourhood: units as above
- probability of degradation
- claim of an empty cell to remain empty
- frequency of replicator mobility steps
- frequency of writing the output
- frequency	of saving the whole grid
- metabolic function applied - an integer of 1 - 11 meaning:
    - 1: geometric mean
	- 2: minimum
    - 3: harmonic mean
    - 4: flat (if any activitiy is 0, M=0, else M=1)
    - 5: random uniform U(0,1)
    - 6: linear flux
    - 7: Monod metabolic function
    - 8: geometric mean maximized to 1
    - 9: minimum maximized to 1
    - 10: linear flux maximized to 1
    - 11: antifitness maximized to 1
- number of enzimatic activities
- *i* blocks of arguments describing the initial status for replicator type *i* 	 
    - each block contains 2 values in the following order: initial frequency of type *i*, replication rate of type *i*
    - the number of blocks is: number of enzimatic activities + 1
    - first block is for the parasitic species
- simulation ID

To make it easier to parametrize the simulations, convenience scripts `generalas.sh` and `start.sh` are provided. 

-  Change the parameters with a text editor in `./generalas.sh`
-  Generate input file to *IN* directory by running command `./generalas.sh`
-  Change simulation parameters in `.start.sh` (e.g. number of threads used) if needed
-  Start simulations with command `./start.sh` or `nohup ./start.sh SIMULATIONNAME &`

