#! /bin/bash

# Setup edm environment
source /reg/g/pcds/setup/epicsenv-3-14-12.sh

edm -x -eolc		\
 -m "P=THZ:RGA"		\
 -m "R=:01"	\
 asynScreens/asynRecord.edl	\
 opi/edm/asynRecord.edl \
 &
