#!/bin/bash

mpiexec -hostfile mpi.host -n $1 $2
