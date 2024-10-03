#!/bin/bash

#automatically generate data that we want.
HTSIZE=("500000" "600000" "700000" "800000" "900000" "1000000")
NLOOKUPS=("100" "250")

Macro_HTSIZE="CONFIG_DEFAULT_OUTER_SIZE"
Macro_NLOOKUPS="CONFIG_DEFAULT_NUM_LOOKUPS"

file="Enclave/Core-Library/config.h"

outputs="$1"

for i in "{!NLOOKUPS[@]}"; do
    for j in "{!HTSIZE[@]}"; do
        nlookup="${NLOOKUPS[$i]}"
        htsize="${HTSIZE[$j]}"

        sed -i "s/^#define $Macro_HTSIZE .*/#define $Macro_HTSIZE $nlookup/" $file
        sed -i "s/^#define $Macro_HTSIZE .*/#define $Macro_HTSIZE $nlookup/" $file
        SGX_MODE=SIM make
        ./app 2>&1 | grep -oP '\d+(?= us)' >> "$outputs"
        make clean
    done
done