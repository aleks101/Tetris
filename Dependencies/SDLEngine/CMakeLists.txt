include(FetchContent)

FetchContent_Declare(
    SDLEngine
    GIT_REPOSITORY https://github.com/aleks101/SDLEngine_V1.git
    GIT_TAG main
)

FetchContent_MakeAvailable(SDLEngine)