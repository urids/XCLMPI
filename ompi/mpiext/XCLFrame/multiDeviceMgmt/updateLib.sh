#!/bin/bash

host1=192.168.1.101
host2=192.168.1.103
host3=192.168.1.104

make && make install

declare -a hosts=($host1 $host2 $host3)

pathdr='/home/uriel/Dev/mpisrc/XCLMPI/ompi/mpiext/XCLFrame/multiDeviceMgmt'

for i in "${hosts[@]}"
do
rsync -r $(pwd) uriel@$i:$(pwd)/../
rsh uriel@$i 'cd /home/uriel/Dev/mpisrc/XCLMPI/ompi/mpiext/XCLFrame/multiDeviceMgmt && make && make install' 
done


