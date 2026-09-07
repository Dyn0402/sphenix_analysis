#!/bin/bash

# Macro to run
macro="Fun4All_direct_photon.C"

# Base directory for input files
base_dir="/sphenix/lustre01/sphnxpro/physics/slurp/calophysics/ana437_2024p007"

# List of runs
runs=(49061 49435 49127 50552 49434 49248 49117 49110 49456 51178 \
      48989 50465 50464 50671 50669 49438 49433 49267 49265 49229 \
      49137 49136)

# Additional macro parameters
param3=0
param4=100000
param5="2024p007"

# Function to calculate run directory
get_run_directory() {
    local run_number=$1
    local lower_bound=$(( (run_number / 100) * 100 ))
    local upper_bound=$(( lower_bound + 100 ))
    printf "run_%08d_%08d" "$lower_bound" "$upper_bound"
}

# Loop over runs
for run in "${runs[@]}"; do
    # Calculate the run directory dynamically
    run_directory=$(get_run_directory "$run")

    # Construct the input file path based on the run number and directory
    input_file="${base_dir}/${run_directory}/DST_TRIGGERED_EVENT_run2pp_ana437_2024p007-000${run}-00100.root"

    # Check if the input file exists
    if [[ -f "$input_file" ]]; then
        # Extract output file name from the run number
        output_name="${run}-100.root"

        echo "Running macro on file: $input_file"
        echo "Output name: $output_name"

        # Run the ROOT macro
        root -b -q "${macro}(\"${input_file}\", \"${output_name}\", ${param3}, ${param4}, \"${param5}\")"
    else
        echo "File not found: $input_file"
    fi
done

echo "Processing complete."
