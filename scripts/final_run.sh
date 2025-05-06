#!/bin/bash

export SCRIPTS_DIR=/scratch/network/ah7226/openpiton-475-cache-replacement/scripts
export PITON_ROOT=/scratch/network/ah7226/openpiton-475-cache-replacement
export ANDREW_CUSTOM_TEST=/scratch/network/ah7226/openpiton-475-cache-replacement/build/andrew_custom_test
export POLICY="LRU" # CHANGE
export TEST="binsearch"

# source /home/ee475/env_spr2022.sh
# source $PITON_ROOT/piton/piton_settings.bash
source /scratch/network/ah7226/env_piton.rc > /dev/null 2>&1

# build_sim.sh
# cd into andrew test
# loop through each config
# cd into the test (128kb_4way)
# command.sh
# sim_result
# after binsearch
# remove_log.sh

# save final results into 128_4way in binsearch
export DEBUG=1

declare -a configs=(
  "4 65536"
  "4 131072"
  "4 262144"
  "4 524288"
  "8 65536"
  "8 131072"
  "8 262144"
  "8 524288"
)


for config in "${configs[@]}"; do
    set -- $config
    assoc=$1
    size=$2

    export CONFIG_L2_ASSOCIATIVITY=$assoc
    export CONFIG_L2_SIZE=$size

    echo "=== Running config: ${assoc}-way, ${size} bytes ==="

    cd "$ANDREW_CUSTOM_TEST/$POLICY/$TEST"

    kb=$((size / 1024))
    folder="${kb}kb_${assoc}way"
    echo "cd into: $folder"
    cd "$folder" || { echo "Failed to cd into $folder"; exit 1; }

    build_sim.sh > /dev/null 2>&1

    # call command.sh
    ../command.sh
    
    sim_result.sh
    ../remove_log.sh

    
done
