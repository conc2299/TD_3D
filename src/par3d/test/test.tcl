# set technology information
set ALL_LEFS {
    /home/TritonPart/TD_3D/test/Nangate45/Nangate45_tech.lef
    /home/testcase/iccad2015.ot/superblue1/superblue1.lef
}
set ALL_DEFS {
    /home/testcase/iccad2015.ot/simple/simple_QQ.def
}
set EARLY_LIBS {
    /home/testcase/iccad2015.ot/simple/simple_Early.lib
}
set LATE_LIBS {
    /home/testcase/iccad2015.ot/simple/simple_Late.lib
}
# /home/testcase/iccad2015.ot/superblue1/superblue1_Early.lib
# set design information
set design "simple"
set top_design "simple"
set netlist "/home/testcase/iccad2015.ot/simple/simple.v"
set sdc "/home/testcase/iccad2015.ot/simple/simple.sdc" 

# proc set_all_input_output_delays {{clk_period_factor .2}} {
#   set clk [lindex [all_clocks] 0]
#   set period [get_property $clk period]
#   set delay [expr $period * $clk_period_factor]
#   set_input_delay $delay -clock $clk [delete_from_list [all_inputs] [all_clocks]]
#   set_output_delay $delay -clock $clk [delete_from_list [all_outputs] [all_clocks]]
# }

read_verilog $netlist

foreach lib_file_e ${EARLY_LIBS} {
    read_lib -min $lib_file_e
}
foreach lib_file_l ${LATE_LIBS} {
    read_lib -max $lib_file_l
}
foreach lef_file ${ALL_LEFS} {
  read_lef $lef_file
}

link_design $top_design

# foreach def_file ${ALL_DEFS} {
#   read_def $def_file -continue_on_error
# }

puts "Design linked"

read_sdc $sdc

##############################################################################################
## Try placer3d
##############################################################################################
puts "Start partitioner3d"

# set the debug level
set_debug_level PAR3D "run" 1
set_debug_level STA "parasitic_reduce" 2
# set_debug_level PAR "multilevel_partitioning" 1
# set_debug_level PAR "v_cycle_refinement" 1
# set_debug_level PAR "coarsening" 1
# set_debug_level PAR "netlist" 1
# set_debug_level PAR "refinement" 1
# set_debug_level PAR "cut_overlay_clustering" 1
# set_debug_level PAR "evaluation" 1
# set_debug_level PAR "partitioning" 1

# call triton_part to partition the netlist

partitioner3d_run -solution_file /home/TritonPart/TD_3D/src/par3d/test/simple_partition_solution.txt\
                    -spef_file /home/TritonPart/TD_3D/src/par3d/test/simple_partition_solution.spef