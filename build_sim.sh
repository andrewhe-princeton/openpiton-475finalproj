CALLER_PWD=$(pwd)
cd $PITON_ROOT/build
sims -sys=manycore -x_tiles=1 -y_tiles=1 -vcs_build -ariane
cd $CALLER_PWD
