ADD_EXECUTABLE(
    fft_test
    fft_test.cpp
    ${PROJECT_SOURCE_DIR}/FFT/fft.cpp
    ${PROJECT_SOURCE_DIR}/FFT/fftsg2d.cpp
    ${PROJECT_SOURCE_DIR}/FFT/fftsg3d.cpp
    ${PROJECT_SOURCE_DIR}/FFT/fftsg.cpp
)
INCLUDE_DIRECTORIES (
    ${PROJECT_SOURCE_DIR}/FFT
)

target_link_libraries(fft_test "-lgomp -lpthread -lm -ldl -lX11")