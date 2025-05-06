

sta::define_cmd_args "placer3d_run" {
    -solution_file {solution_file ""}
}
proc placer3d_run {args} {
    sta::parse_key_args "placer3d_run" args \
    keys {-solution_file} \
    flags {}

    set solution_file $keys(-solution_file)
    gpl3d::placer3d_run $solution_file
}