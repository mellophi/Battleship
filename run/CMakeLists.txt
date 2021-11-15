set(SOURCES 
    main.cpp
)
#FetchContent_MakeAvailable(fmt)
add_executable(${BATTLESHIP_EXE} ${SOURCES})
target_link_libraries(${BATTLESHIP_EXE} PRIVATE ${BATTLESHIP_LIB} fmt::fmt)

add_executable(test test.cpp)
target_link_libraries(test PRIVATE fmt::fmt)