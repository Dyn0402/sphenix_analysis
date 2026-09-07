cmake_minimum_required(VERSION 3.29)
project(sphenix_analysis)

set(CMAKE_CXX_STANDARD 14)

include_directories(direct_photon)

add_executable(sphenix_analysis
        direct_photon/macros/Fun4All_direct_photon.C
        direct_photon/autogen.sh
    direct_photon/configure.ac
    direct_photon/direct_photon.cpp
    direct_photon/direct_photon.h
    direct_photon/Makefile.am
    direct_photon/README.md
    direct_photon/SMDrelativegains.dat)
