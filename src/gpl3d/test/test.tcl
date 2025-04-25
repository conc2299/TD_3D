# set technology information
set ALL_LEFS {
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

# foreach lib_file ${EARLY_LIBS} {
#     read_lib $lib_file
# }
foreach lib_file ${LATE_LIBS} {
    read_lib $lib_file
}
foreach lef_file ${ALL_LEFS} {
  read_lef $lef_file
}
foreach def_file ${ALL_DEFS} {
  read_def $def_file -continue_on_error
}

link_design $top_design
read_sdc $sdc

##############################################################################################
## Try placer3d
##############################################################################################
puts "Start placer3d"

# set the debug level
set_debug_level GPL3D "run" 1

# call triton_part to partition the netlist

placer3d_run -argc 2