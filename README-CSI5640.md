# CSI 5640 Final Project - Branch Prediction Techniques for Speculative Loop Execution

By: Shane MacFadyen, Joshua Duncan, and Matthew Zaitouna

Repository: https://github.com/shmacfadyen/gem5

## Description

The objective of this project is to recreate the experiments performed in the paper "[Comparative Study of Path Prediction Method for Speculative Loop Execution](https://ieeexplore.ieee.org/document/6424578)". These experiments involved utilizing a simulation tool to emulate a processor using different branch predictors to evaluate their performance through obtaining their miss-prediction rates on a set of benchmark programs. The different branch predictors that were evaluated are Bimodal, GAg, GAp, PAg, PAp, GShare, and Perceptron.

Although the paper used [SimpleScalar](https://github.com/toddmaustin/simplesim-3.0) to emulate a processor, our team was unable to get any programs to compile and run utilizing this emulation tool. Therefore, we instead made use of the [Gem5](https://www.gem5.org/) for processor emulation and implementing these additional branch prediction techniques.

The paper used [SPEC CINT2000](https://www.spec.org/cpu2000/CINT2000/) as their benchmark programs for analyzing the branch prediction methods. However, as this benchmark is not free to use, [MiBench](https://vhosts.eecs.umich.edu/mibench/) was utilized instead for the set of benchmark programs in this project.

## Contributions/Work Completed

The first step of this project was implementing the branch prediction methods that were analyzed in the paper. Only the perceptron branch predictor was already implemented within the Gem5 software. Therefore, the other 6 branch predictors were implemented by the team and added to the software system. The files listed below were created or edited to implement these branch predictors:

1. src/cpu/pred/2level_gag.hh
2. src/cpu/pred/2level_gag.cc
3. src/cpu/pred/2level_gap.hh
4. src/cpu/pred/2level_gap.cc
5. src/cpu/pred/2level_pag.hh
6. src/cpu/pred/2level_pag.cc
7. src/cpu/pred/2level_pap.hh
8. src/cpu/pred/2level_pap.cc
9. src/cpu/pred/Bimodal.hh
10. src/cpu/pred/Bimodal.cc
11. src/cpu/pred/gshare.hh
12. src/cpu/pred/gshare.cc
13. src/cpu/pred/BranchPredictor.py (edited to add in the new branch predictors as python classes)
14. src/cpu/pred/SConscript (edited to build new branch predictor C++ and Python code)

After setting up the branch predictor software, the processor configuration and evaluation scripts were created. The configuration python file identified the configuration of the processor and the components that were instantiated with it such as the chosen branch predictor. An evaluation script was created to run all the separate benchmark programs on the individual branch predictors. The last script extracts all the stats.txt files and places them in one folder for analysis. These files are listed below:

1. configs/josh_configs/simple-arm.py
2. configs/josh_configs/run_branch_predictor_tests.sh
3. configs/josh_configs/rename_stats_files.sh

Lastly, a python script was added to extract the branch prediction mis-prediction rate and accuracy from the stats.txt files, which is listed below:

1. src/cpu/pred/Efficiency.py

Our resulting `stats.txt` files for performing the branch prediction analysis are stored in the `branch_prediction_results/stats_files` folder within the fork of the Gem5 repository used for this project. They can also be seen at the following link: https://github.com/shmacfadyen/gem5/tree/project-develop/branch_prediction_results/stats_files

## Replication Instructions

### Building the Gem5 Software

The steps for building the Gem5 software containing the additional branch prediction methods implemented for this project are shown below. Gem5 is intended to run on a Linux environment (such as Ubuntu 20.04 or 22.04) and has not been thoroughly tested on Windows.

1. Clone the git repository

``` bash
git clone https://github.com/shmacfadyen/gem5.git
```

2. Install the dependencies

``` bash
sudo apt install build-essential scons python3-dev git pre-commit zlib1g zlib1g-dev \
    libprotobuf-dev protobuf-compiler libprotoc-dev libgoogle-perftools-dev \
    libboost-all-dev  libhdf5-serial-dev python3-pydot python3-venv python3-tk mypy \
    m4 libcapstone-dev libpng-dev libelf-dev pkg-config wget cmake doxygen
```

3. Build Gem5 for simulation

``` bash
cd gem5
scons build/ALL/gem5.opt -j 4
```

Now the Gem5 software has been built and can be used for processor emulation.

### MiBench Setup

The following procedure describes how to obtain and build the MiBench software applications. However, the binaries should already have been downloaded when the git repository (https://github.com/shmacfadyen/gem5.git) was cloned. Therefore, this section is only needed to obtain the MiBench data files to execute the application with or if you intend to rebuild the benchmark software for the simulated architecture.

The next step is obtaining the MiBench software and building for an ARM architecture. The steps are shown below.

1. Go the the MiBench website (https://vhosts.eecs.umich.edu/mibench/) and download the following files:
    - automotive.tar.gz
    - consumer.tar.gz
    - network.tar.gz
    - office.tar.gz
    - security.tar.gz
    - telecomm.tar.gz

2. Place the downloaded tar.gz files in the mibench folder of the cloned gem5 repository.

3. Extract all the tar.gz files into that directory, WITHOUT replacing any existing files. This is due to needing to manually update many of the make files to cross-compile for the ARM architecture. These updated Makefiles should already exist and should not be replaced if the target architecture is still an ARM system.

4. If you want to recompile all the program binaries, individually traverse to each subdirectory of the downloaded tar.gz files, running `make` to generate the binaries for the benchmark software. The programs that were used during the project are listed below:
    - automotive/basicmath
    - automotive/qsort
    - automotive/susan
    - network/dijkstra
    - office/stringsearch
    - security/blowfish
    - security/rijndael
    - security/sha
    - telecomm/CRC32
    - telecomm/FFT
    - consumer/jpeg

### Running the Simulation Tests

Individual simulations can be run with the architecture utilized for this project by executing the command shown below from the gem5 directory:

``` bash
./build/ALL/gem5.opt -d <statistics_output_directory> configs/josh_configs/simple-arm.py <branch_predictor_type> <executable_path> <executable_args>
```

- <statistics_output_directory> should be replaced with the directory the performance results are expected to be placed in.
- <branch_predictor_type> should be one of the following branch predictors:
    - Bimodal
    - GAg
    - GAp
    - PAg
    - PAp
    - GShare
    - Perceptron
- <executable_path> should be the path to the executable to be run on the architecture.
- <executable_args> should be any arguments that need to be passed to the executable. Can be left blank if there are no arguments.

An example of running the gem5 simulation is shown below:

``` bash
./build/ALL/gem5.opt -d m5out/GAg/qsort_small configs/josh_configs/simple-arm.py GAg mibench/automotive/qsort/qsort_small mibench/automotive/qsort/input_small.dat
```

The full suite of benchmark programs that were run during this project can be executed by running the `run_branch_predictor_tests.sh` script within the configuration directory. This script will execute all the MiBench programs identified above for each of the branch predictors and output the results to corresponding directories within the m5out folder of the gem5 directory. The following command sequence is used to execute this script from the home gem5 directory:

``` bash
cd configs/josh_configs
./run_branch_predictor_tests.sh
```

After running the above script, the resulting `stats.txt` files will be in many different subdirectories of the m5out folder. The following script can be used to consolidate those `stats.txt` files into a single folder within the m5out directory called combined:

``` bash
./rename_stats_files.sh ../../m5out
```

### Evaluating the Results

The resulting mis-prediction rate can be obtained from the `stats.txt` files through running the `Efficiency.py` script within the `src/cpu/pred` folder. However, the individual file name needs to be manually updated in the script to obtain the corresponding hit/miss rate. The results can then be separately graphed to produce resulting plots.

## Emulated CPU Specs

| Trait | Value |
| ----------- | --------- |
| CPU | ArmAtomicSimpleCPU |
| Clock Frequency | 2 GHz |
| RAM Type | DDR4_2400_8x8 |
| RAM Amount | 8 GiB |
