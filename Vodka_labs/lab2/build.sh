#!/bin/bash

gcc -O2 -o $1 -fopenmp $1.c -lm
