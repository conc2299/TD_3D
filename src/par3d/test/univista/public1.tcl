set BENCH_FOLDER "/home/testcase/univista/"
set NETLIST "$BENCH_FOLDER/Public/public_case1/input.v"
set SDC "$BENCH_FOLDER/Public/public_case1/input.sdc"
set DEF  "$BENCH_FOLDER/Public/public_case1/input.def"
set LIBS [glob -directory $BENCH_FOLDER/pdk/lib *]
set LEFS "
    $BENCH_FOLDER/pdk/lef/NangateOpenCellLibrary.tech.lef
    $BENCH_FOLDER/pdk/lef/NangateOpenCellLibrary.macro.mod.lef
    $BENCH_FOLDER/pdk/lef/chip_bridge.lef
    $BENCH_FOLDER/pdk/lef/chipset.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_1024x32.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_2048x39.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_256x16.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_256x34.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_256x95.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_256x96.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_32x64.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_512x64.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_64x15.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_64x21.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_64x32.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_64x7.lef
    $BENCH_FOLDER/pdk/lef/fakeram45_64x96.lef
    $BENCH_FOLDER/pdk/lef/tilebb.lef
"

set TOP "top"

read_verilog $NETLIST
puts "finish reading verilog"

foreach LIB_FILE ${LIBS} {
    read_lib $LIB_FILE
}
puts "finish reading libs"

foreach LEF_FILE ${LEFS} {
    read_lef $LEF_FILE
}

puts "finish reading lefs"

link_design $TOP -hier
read_sdc $SDC
# read_def $DEF

##############################
# print statistics
##############################
puts "finish reading design"

partitioner3d_print_module_hierarchy
# partitioner3d_report_timing -num_path 10

exit