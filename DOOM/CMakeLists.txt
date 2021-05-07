# CMakeList.txt: proyecto de CMake para DOOM, incluya el origen y defina
# la lógica específica del proyecto aquí.
#
cmake_minimum_required (VERSION 3.8)

# Agregue un origen al ejecutable de este proyecto.
#add_executable (DOOM "main.cpp" "DOOM.h"  "WAD/wad_loader.cpp" "WAD/wad_loader.h" "WAD/DataTypes.h" "WAD/wad_reader.cpp" "WAD/wad_reader.h" "maps/map.h" "maps/map.cpp" "maps/map_types.h")

# TODO: Agregue pruebas y destinos de instalación si es necesario.
#Pruebas para tener sfml

set(SFML_STATIC_LIBRARIES TRUE)
#look for SFML and add it
set (SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/../SFML/lib/cmake/SFML")
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)

#we add the executable of the program
# Agregue un origen al ejecutable de este proyecto.
add_executable (DOOM "main.cpp" "DOOM.h"  "WAD/wad_loader.cpp" "WAD/wad_loader.h" "WAD/DataTypes.h" "WAD/wad_reader.cpp" "WAD/wad_reader.h" "maps/map.h" "maps/map.cpp" "maps/map_types.h" "Engine/DoomEngine.h" "Engine/DoomEngine.cpp" "doomdef.h" "Game/Game.h" "Game/Game.cpp" "Player/Player.h" "Player/Player.cpp" "Misc/Angle.h" "Misc/Angle.cpp" "Engine/Renderer.h" "Engine/Renderer.cpp" "MainMenu/menu.h" "MainMenu/menu.cpp" "Engine/RenderTypes.h" "Game/GameStates.h" "PauseMenu/PauseMenu.h" "PauseMenu/PauseMenu.cpp" "HUD/HUD.h" "HUD/HUD.cpp" "DisplayManager/DisplayManager.cpp" "Patch/Patch.h" "Patch/Patch.cpp")
target_link_libraries(DOOM sfml-system sfml-window sfml-graphics sfml-network sfml-audio)