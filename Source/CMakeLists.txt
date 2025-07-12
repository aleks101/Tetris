file(GLOB_RECURSE SRC_FILES *.cpp)

add_executable(Tetris ${SRC_FILES})

target_include_directories(Tetris
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../Build/_deps/sdlengine-src/Include
)

target_link_libraries(Tetris
    #   SDL2::SDL2main
    #   SDL2::SDL2
    #   SDL2_image::SDL2_image
    #   ${FREETYPE}
    #   SDL2_ttf::SDL2_ttf
    #   ${LIBTTF}
    #   SDL2_mixer::SDL2_mixer
    SDLEngine
)