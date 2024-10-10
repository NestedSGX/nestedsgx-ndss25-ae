#!/bin/bash
outputs="$1"
tmp="temp.txt"
progs=("fio-seq-read.fio" "fio-seq-write.fio" "fio-seq-rw.fio" "fio-rand-read.fio" "fio-rand-write.fio" "fio-rand-rw.fio")

SGX_MODE=SIM ./download_and_build_fio.sh

for prog in "${progs[@]}"; do
    SGX_MODE=SIM ./run_fio_on_occlum.sh "$prog" > "$tmp"
    tail -n 1 "$tmp" >> "$1"
    rm $tmp
done