#!/bin/bash

module load gnu8/8.3.0

srun --time 00:30:00 --exclusive --nodelist=compute-1-$1 --partition compute1 --pty bash -i

