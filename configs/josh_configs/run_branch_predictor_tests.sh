#!/usr/bin/env bash

executables=(
    "mibench/automotive/basicmath/basicmath_small"
    "mibench/automotive/qsort/qsort_small mibench/automotive/qsort/input_small.dat"
    "mibench/automotive/susan/susan_smoothing mibench/automotive/susan/input_small.pgm mibench/results/output_small.smoothing.pgm -s"
    "mibench/automotive/susan/susan_edges mibench/automotive/susan/input_small.pgm mibench/results/output_small.edges.pgm -e"
    "mibench/automotive/susan/susan_corners mibench/automotive/susan/input_small.pgm mibench/results/output_small.corners.pgm -c"
    "mibench/network/dijkstra/dijkstra_small mibench/network/dijkstra/input.dat"
    "mibench/office/stringsearch/search_small"
    "mibench/security/blowfish/bf_encode e mibench/security/blowfish/input_small.asc mibench/security/blowfish/output_small.enc 1234567890abcdeffedcba0987654321"
    "mibench/security/blowfish/bf_decode d mibench/security/blowfish/output_small.enc mibench/results/output_small.asc 1234567890abcdeffedcba0987654321"
    "mibench/security/rijndael/rijndael_encode mibench/security/rijndael/input_small.asc mibench/security/rijndael/output_small.enc e 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321"
    "mibench/security/rijndael/rijndael_decode mibench/security/rijndael/output_small.enc mibench/results/output_small.dec d 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321"
    "mibench/security/sha/sha mibench/security/sha/input_small.asc"
    "mibench/telecomm/CRC32/crc mibench/telecomm/CRC32/../adpcm/data/small.pcm"
    "mibench/telecomm/FFT/fft_forwards 4 4096"
    "mibench/telecomm/FFT/fft_inverse 4 8192 -i"
    "mibench/consumer/jpeg/jpeg-6a/cjpeg -dct int -progressive -opt -outfile mibench/results/output_small_encode.jpeg mibench/consumer/jpeg/input_small.ppm"
    "mibench/consumer/jpeg/jpeg-6a/djpeg -dct int -ppm -outfile mibench/results/output_small_decode.ppm mibench/consumer/jpeg/input_small.jpg"
)

bp_types=(
    "GAg" 
    "GAp" 
    "PAg" 
    "PAp" 
    "Bimodal" 
    "GShare" 
    "Perceptron"
)

if [[ "$(basename "$PWD")" == "josh_configs" ]]
then
    cd ../..
fi

for ex in "${executables[@]}"
do
    # Get the title of the executable
    IFS=' '
    read -ra ex_args <<< "$ex"
    ex_title="$(basename "${ex_args[0]}" | cut -d. -f1)"

    for type in "${bp_types[@]}"
    do
        echo "Running $ex_title for branchPred $type"
        ./build/ALL/gem5.opt -d m5out/$type/$ex_title configs/josh_configs/simple-arm.py $type $ex
        echo "Finished $ex_title for branchPred $type!"
        echo
        echo
    done
done


