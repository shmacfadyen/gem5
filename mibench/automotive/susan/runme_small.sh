#!/bin/sh
susan_smooting input_small.pgm output_small.smoothing.pgm -s
susan_edges input_small.pgm output_small.edges.pgm -e
susan_corners input_small.pgm output_small.corners.pgm -c

