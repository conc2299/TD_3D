

include("openroad")

add_library(par3d_lib
    src/partitioner3d.cpp
    src/PartResult.cpp
    src/odbStats.cpp
    src/staStats.cpp
    src/timing.cpp
    src/EstimatePartitionParasitics.cpp
)

target_link_libraries(par3d_lib
    PUBLIC
        odb
        OpenSTA
        utl_lib
        dbSta_lib
        par
)

target_include_directories(par3d_lib
    PUBLIC
        include
)

swig_lib(
    NAME      par3d
    NAMESPACE par3d
    I_FILE    src/partitioner3d.i
    SCRIPTS   src/partitioner3d.tcl
)

target_sources(par3d
    PRIVATE
        src/MakePartitioner3d.cpp
)

target_include_directories(par3d
    PUBLIC
        include
)

target_link_libraries(par3d
    PRIVATE
        par3d_lib
)