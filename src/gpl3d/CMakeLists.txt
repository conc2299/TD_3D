


include("openroad")

add_library(gpl3d_lib
    src/tryodb.cpp
    src/placer3d.cpp
)

find_package(Boost 1.80 REQUIRED)

target_link_libraries(gpl3d_lib
    PUBLIC
        odb
        OpenSTA
        utl_lib
        dbSta_lib
        Boost::boost
)

target_include_directories(gpl3d_lib
    PUBLIC
        include
)

swig_lib(
    NAME      gpl3d
    NAMESPACE gpl3d
    I_FILE    src/placer3d.i
    SCRIPTS   src/placer3d.tcl
)

target_sources(gpl3d
    PRIVATE
        src/MakePlacer3d.cpp
)

target_include_directories(gpl3d
    PUBLIC
        include
)

target_link_libraries(gpl3d
    PRIVATE
        gpl3d_lib
)