sta::define_cmd_args "partitioner3d_run" {
    -num_parts {num_parts 2} \
    -balance_constraint {balance_constraint 2} \
    -seed {seed 42} \
    -top_n {top_n 10000} \
    -solution_file {solution_file ""}\
}
proc partitioner3d_run {args} {
    sta::parse_key_args "placer3d_run" args \
    keys {-num_parts \
        -balance_constraint \
        -seed \
        -top_n \
        -solution_file \
        } \
    flags {}

    set num_parts $keys(-num_parts)
    set balance_constraint $keys(-balance_constraint)
    set seed $keys(-seed)
    set top_n $keys(-top_n)
    set solution_file $keys(-solution_file)

    par3d::partitioner3d_run \
        $num_parts \
        $balance_constraint \
        $seed \
        $top_n \
        $solution_file
}

sta::define_cmd_args "partitioner3d_extract_parasitics" {
    -spef_file {spef_file ""} \
    -update_sta {update_sta 0}\
}
proc partitioner3d_extract_parasitics {args} {
    sta::parse_key_args "partitioner3d_extract_parasitics" args \
    keys {-spef_file \
        } \
    flags {}

    set spef_file $keys(-spef_file)

    par3d::partitioner3d_extract_parasitics \
        $spef_file 
}

sta::define_cmd_args "partitioner3d_report_timing" {
    -report_file {report_file ""} \
}
proc partitioner3d_report_timing {args} {
    sta::parse_key_args "partitoner3d_report_timing" args \
    keys {-report_file \
        } \
    flags {}

    set report_file $keys(-report_file)

    par3d::partitioner3d_report_timing \
        $report_file
}