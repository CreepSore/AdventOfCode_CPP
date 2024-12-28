cmake_minimum_required(VERSION 3.28)
project(AdventOfCode_Cpp LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)
option(BUILD_SHARED_LIBS OFF)

include(FetchContent)

FetchContent_Declare(
	SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.6
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

FetchContent_Declare(
    ImPlot
    GIT_REPOSITORY https://github.com/epezent/implot
    GIT_TAG v0.16
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(ImPlot)
FetchContent_GetProperties(ImPlot SOURCE_DIR IMPLOT_DIR)

set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(
    ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(ImGui-SFML)

add_executable(
    main
    AdventOfCode.Bootstrap/AdventOfCode.Bootstrap.cpp
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_demo.cpp
    ${IMPLOT_DIR}/implot_items.cpp
	AdventOfCode.Shared/AocRegistry.cpp
	AdventOfCode.Shared/IAocDay.cpp
    AdventOfCode.Visualization/BaseWindow.cpp
    AdventOfCode.2024/Shared/Grid.cpp
    AdventOfCode.2024/Shared/Vec2.cpp
    AdventOfCode.2024/D01/Aoc2024D01.cpp
    AdventOfCode.2024/D02/Aoc2024D02.cpp
    AdventOfCode.2024/D03/Aoc2024D03.cpp
    AdventOfCode.2024/D03/Lexer.cpp
    AdventOfCode.2024/D04/Aoc2024D04.cpp
    AdventOfCode.2024/D05/Aoc2024D05.cpp
    AdventOfCode.2024/D06/Aoc2024D06.cpp
    AdventOfCode.2024/D07/Aoc2024D07.cpp
 "AdventOfCode.2024/D08/Aoc2024D08.cpp")

target_include_directories(
    main
    PRIVATE
    ${IMPLOT_DIR}
)

target_include_directories(
    main
    PRIVATE
    ${IMGUI_DIR}
)

target_link_libraries(
    main
    PRIVATE
    SFML::Graphics
    ImGui-SFML::ImGui-SFML
)

target_compile_features(main PRIVATE cxx_std_20)
