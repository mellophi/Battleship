set(SOURCES
    HMM.cpp
    utils.cpp
    grid.cpp
    game.cpp
)

add_library(${BATTLESHIP_LIB} STATIC ${SOURCES})

target_link_libraries(${BATTLESHIP_LIB} PRIVATE fmt::fmt)