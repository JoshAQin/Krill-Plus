#=============================================================================
# run_base.tcl 
#=============================================================================
# @brief: A Tcl script for synthesizing the design.

# Project name
set hls_prj [lindex $argv 2]_csim.prj

# Open/reset the project
open_project ${hls_prj} -reset

# Top function of the design is "top"
set_top test

# Add design and testbench files
if { $argc < 4 } {
    add_files template/main.cpp -cflags "-I ./[lindex $argv 2]"
    add_files -tb template/test.cpp -cflags "-I ./[lindex $argv 2]"
    add_files -tb ./datasets/testGraph
} else {
    add_files template/main.cpp -cflags "-I ./[lindex $argv 2] -DWEIGHTED"
    add_files -tb template/test.cpp -cflags "-I ./[lindex $argv 2] -DWEIGHTED"
    add_files -tb ./datasets/testGraph_w
}

open_solution "solution1"
# Use Zynq device
set_part {xczu3eg-sbva484-1-e}

# Target clock period is 10ns
create_clock -period 10

# Directives 

############################################

# Simulate the C++ design
csim_design -O
# Synthesize the design
#csynth_design
# Co-simulate the design
#cosim_design
# Implement the design
#export_design -format ip_catalog

exit
