#!/bin/bash
# CHANGE THIS!
dir_testt=2025_04_22_0 # CHANGE
report_name=report3.txt #CHANGE


# IS THERE A WAY TO PARAMETRIZE BY CACHE RP BY THIS SCRIPT?



export SCRIPTS_DIR=/scratch/network/ah7226/openpiton-475-cache-replacement/scripts
export PITON_ROOT=/scratch/network/ah7226/openpiton-475-cache-replacement

source /home/ee475/env_spr2022.sh
source $PITON_ROOT/piton/piton_settings.bash
export DEBUG=1

#export CONFIG_L2_SIZE=131072
#export CONFIG_L2_ASSOCIATIVITY=8
cd $PITON_ROOT/build
#sims -sim_type=vcs -group=tile1_mini
#sims -sys=manycore      -x_tiles=1      -y_tiles=1      -vcs_run     mibench_dijkstra.c      -ariane      -rtl_timeout 10000000 > testinglog.txt
cd $dir_testt # parametrize this
#regreport -debug $PWD > $SCRIPTS_DIR/$report_name # gives Cycles/Sec

cd $SCRIPTS_DIR
# run andrew python script (feed it 2025_04_21_0 location)
python3 parse_l2_file.py "../build/sims.log" > $report_name

# now report.txt has the stats - get overall numbers
# Call the Python script and capture its output into Bash vars
eval $(python3 extract_stats.py "$report_name")

# Now you can use the variables
echo "Exec Cycles: $CYCLES_SEC"
echo "Average L2 Miss Rate: $L2_MISS_RATE"
# python3 generate_graphs_final