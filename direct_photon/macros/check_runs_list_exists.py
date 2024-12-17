#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on December 17 6:33 PM 2024
Created in CLion
Created as sphenix_analysis/check_runs_list_exists

@author: Dylan Neff, Dylan
"""

import os

# Configuration parameters
ana_version = "ana437"
dbtag = "2024p007"
base_dir = f"/sphenix/lustre01/sphnxpro/physics/slurp/calophysics/{ana_version}_{dbtag}"
root_number = 100

def main():
    runs_list_file = "runs.list"  # File containing run numbers

    # Check if the runs list file exists
    if not os.path.isfile(runs_list_file):
        print(f"Error: {runs_list_file} does not exist.")
        return

    # Read run numbers from file
    with open(runs_list_file, "r") as file:
        run_numbers = [line.strip() for line in file if line.strip().isdigit()]

    if not run_numbers:
        print("No valid run numbers found in the file.")
        return

    # Check for each run number if the input file exists
    existing_runs = []
    for run_number in run_numbers:
        run_number = int(run_number)
        input_file = build_input_file(run_number)
        if os.path.isfile(input_file):
            print(f"File exists: {input_file}")
            existing_runs.append(run_number)
        else:
            print(f"File NOT found: {input_file}")
    print('\nExisting runs:')
    for run in existing_runs:
        print(run)
    print('donzo')


# Function to calculate the run directory
def get_run_directory(run_number):
    lower_bound = (run_number // 100) * 100
    upper_bound = lower_bound + 100
    return f"run_{lower_bound:08d}_{upper_bound:08d}"

# Function to build input file path
def build_input_file(run_number):
    run_directory = get_run_directory(run_number)
    input_file = os.path.join(
        base_dir,
        run_directory,
        f"DST_TRIGGERED_EVENT_run2pp_{ana_version}_{dbtag}-{run_number:08d}-{root_number:05d}.root"
    )
    return input_file


if __name__ == '__main__':
    main()
