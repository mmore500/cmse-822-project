# cmse822-project

Synchronous (`mpi`) and asynchronous (`charm`) parallelizations of [DISHTINY platform](https://osf.io/ewvg8/).

## Compiling & Running

This code has two compile dependencies: Charm and Empirical.
You'll want to clone them next to the `cmse-822-project` file.

```
git clone github.com/mmore500/charm
git clone github.com/devosoft/Empirical
```

You'll need to set up Charm before proceeding.

In order to run locally, I found that the following was necessary.

```
export HDF5_CXX=mpiCC
export HDF5_CLINKER=mpiCC
export CPATH=/usr/include/hdf5/serial/
```

I used the following stack on the HPCC,

```
module purge
module load powertools
module load intel/2018a
module load OpenMPI/2.1.2
module load HDF5/1.8.16
```

Compiling is as simple as navigating to the correct directory (`charm/` or `mpi/`).
Then, `make`.

In order to run, `make run` for MPI and `make remoterun` or `make localrun` for Charm.

## Authorship

Matthew Andres Moreno

`m.more500@gmail.com`
