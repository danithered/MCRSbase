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

# Compile and prepare for simulations

```
make
mkdir IN
mkdir OUT
chmod +x generalas.sh
chmod +x start.sh
```

# Usage

-  Change the parameters with a text editor in `./generalas.sh`
-  Generate input file to *IN* directory by running command `./generalas.sh`
-  Change simulation parameters in `.start.sh` (e.g. number of threads used) if needed
-  Start simulations with command `./start.sh` or `nohup ./start.sh SIMULATIONNAME &`

