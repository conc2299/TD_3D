
sta::define_cmd_args "partitioner3d_run" {
    -solution_file {solution_file ""}\
    -spef_file {spef_file ""}
}
proc partitioner3d_run {args} {
    sta::parse_key_args "placer3d_run" args \
    keys {-solution_file -spef_file} \
    flags {}

    set solution_file $keys(-solution_file)
    set spef_file $keys(-spef_file)
    par3d::partitioner3d_run $solution_file $spef_file
}