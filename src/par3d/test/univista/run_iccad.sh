#!/bin/bash

BENCH_FOLDER="$PWD/src/par3d/test"
OPENROAD="$PWD/build/src/openroad"


for folder in $(ls $BENCH_FOLDER); do
    if [ -d "$BENCH_FOLDER/$folder" ]; then
        echo "Running ICCAD test for $folder"
        TCL_FILE="$BENCH_FOLDER/$folder/$folder.tcl"
        LOG_FILE="$BENCH_FOLDER/$folder/$folder.10.log"
        if [ -f "$TCL_FILE" ]; then
            # run benchmark
            $OPENROAD $TCL_FILE -log $LOG_FILE
            if [ ${PIPESTATUS[0]} -ne 0 ]; then
                echo "Error running $TCL_FILE"
            fi
        else
            echo "\tTCL file not found: $TCL_FILE"
        fi
    fi
done