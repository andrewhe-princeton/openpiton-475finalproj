#!/bin/bash

export SCRIPTS_DIR=/scratch/network/ah7226/openpiton-475-cache-replacement/scripts
export PITON_ROOT=/scratch/network/ah7226/openpiton-475-cache-replacement

source /home/ee475/env_spr2022.sh
source $PITON_ROOT/piton/piton_settings.bash
export DEBUG=1

declare -a configs=(
  # "4 65536"
  "4 131072"
  "4 262144"
  "4 524288"
  # "8 65536"
  "8 131072"
  "8 262144"
  "8 524288"
)


timestamp=$(date +%Y%m%d_%H%M%S)
results_dir="$SCRIPTS_DIR/results/run_$timestamp"
mkdir -p "$results_dir"

for config in "${configs[@]}"; do
    set -- $config
    assoc=$1
    size=$2

    export CONFIG_L2_ASSOCIATIVITY=$assoc
    export CONFIG_L2_SIZE=$size

    echo "Running config: ${assoc}-way, ${size} bytes"

    cd $PITON_ROOT/build
    sims -sim_type=vcs -group=tile1_mini > log.txt

    today=$(date +%Y_%m_%d)
    run_number=0
    while [ -d "$PITON_ROOT/build/${today}_$run_number" ]; do
        ((run_number++))
    done
    ((run_number--))
    dir_testt="${today}_${run_number}"
    cd $dir_testt

    report_name="report_${assoc}w_${size}.txt"
    regreport -debug $PWD > "$results_dir/$report_name"

    cd $SCRIPTS_DIR
    python3 parse_l2_folder.py "../build/$dir_testt" >> "$results_dir/$report_name"

    eval $(python3 extract_stats.py "$results_dir/$report_name")

    echo "Result for ${assoc}-way, ${size} bytes:"
    echo "  Exec Cycles:       $CYCLES_SEC"
    echo "  Avg L2 Miss Rate:  $L2_MISS_RATE"
    echo ""
done
