export HDF5_CXX=mpiCC
export HDF5_CLINKER=mpiCC
export CPATH=/usr/include/hdf5/serial/

module purge
module load powertools
module load intel/2018a
module load OpenMPI/2.1.2
module load HDF5/1.8.16
