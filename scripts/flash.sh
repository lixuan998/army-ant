#!/bin/bash

set -e

XFEL=xfel

make clean && make -j

$XFEL ddr d1

$XFEL write 0x40000000 army-ant.bin

$XFEL exec 0x40000000
