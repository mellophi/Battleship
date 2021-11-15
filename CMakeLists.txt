project(
    Battleship
    LANGUAGES C CXX
)

include(FetchContent)

set(CMAKE_BUILD_TYPE "RELEASE")
set(CMAKE_EXPORT_COMPILE_COMANDS 1)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(/w)
set(BATTLESHIP_EXE "run")
set(BATTLESHIP_LIB "battleship")

FetchContent_Declare(
    fmt
    GIT_REPOSITORY "https://github.com/fmtlib/fmt"
    GIT_TAG 094b66e81d43e5b2762e4a371c6237586cd93724
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(fmt)

#if(MSVC)
#    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
#    set(BUILD_SHARED_LIBS TRUE)
#endif()

include_directories(${Battleship_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(run)






