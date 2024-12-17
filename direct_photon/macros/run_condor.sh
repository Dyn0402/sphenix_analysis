#!/bin/bash

export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${LOGNAME}

source ${HOME}/.bashrc
 
#print the environment - needed for debugging
#printenv

# Macro to run
macro_name="Fun4All_direct_photon.C"

ana_version = "ana437"
dbtag = "2024p007"

# Base directory for input files
base_dir="/sphenix/lustre01/sphnxpro/physics/slurp/calophysics/${ana_version}_${dbtag}"

# Root number, for now just run one root file per run
root_number = 100

start_event = 0
end_event = 10000  # Default number of runs in a file

# Function to calculate run directory
get_run_directory() {
    local run_number=$1
    local lower_bound=$(( (run_number / 100) * 100 ))
    local upper_bound=$(( lower_bound + 100 ))
    printf "run_%08d_%08d" "$lower_bound" "$upper_bound"
}

input_file=$(printf "DST_TRIGGERED_EVENT_run2pp_${ana_version}_${dbtag}-%08d-%05d.root" "${1}" "${root_number}")
output_name=$(printf "run_%08d_file_%05d.root" "${1}" "${root_number}")

#=================== Run with condor =========================#
echo "Running macro on file: $input_file"
echo "Output name: $output_name"

# Run the ROOT macro
root -b -q "${macro}(\"${input_file}\", \"${output_name}\", ${start_event}, ${end_event}, \"${dbtag}\")"

#=========================================================#
