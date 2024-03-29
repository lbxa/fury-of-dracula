cmake_minimum_required(VERSION 3.13)
include_directories(${CMAKE_SOURCE_DIR})

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -o3")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm")

set (CMAKE_CXX_FLAGS "-o3")

project(fury_of_dracula C)
project(fury_of_dracula_hunt C)
project(fury_of_dracula_player_hunt C)

add_executable(fury_of_dracula hunt/testHunterView.c
        hunt/testUtils.c
        hunt/testUtils.h
        hunt/DraculaView.c
        hunt/GameView.h
        hunt/GameView.c
        hunt/HunterView.c
        hunt/Map.c
        hunt/Utilities.h
        hunt/Utilities.c
        hunt/Places.c hunt/Players.h hunt/BinaryHeap.c hunt/BinaryHeap.h hunt/HashTable.c hunt/HashTable.h hunt/PathFinding.c hunt/PathFinding.h hunt/Queue.c hunt/Queue.h hunt/Players.c)

add_executable(fury_of_dracula_hunt hunt/Game.c
        hunt/dracula.c
        hunt/dracula.h
        hunt/hunter.c
        hunt/hunter.h
        hunt/DraculaView.c
        hunt/GameView.h
        hunt/GameView.c
        hunt/HunterView.c
        hunt/Map.c
        hunt/Utilities.h
        hunt/Utilities.c
        hunt/Places.c hunt/Players.h hunt/BinaryHeap.c hunt/BinaryHeap.h hunt/HashTable.c hunt/HashTable.h hunt/PathFinding.c hunt/PathFinding.h hunt/Queue.c hunt/Queue.h hunt/Players.c hunt/minimax.c hunt/minimax.h)

add_executable(fury_of_dracula_player_hunt hunt/player.c
        hunt/dracula.c
        hunt/dracula.h
        hunt/hunter.c
        hunt/hunter.h
        hunt/DraculaView.c
        hunt/GameView.h
        hunt/GameView.c
        hunt/HunterView.c
        hunt/Map.c
        hunt/Utilities.h
        hunt/Utilities.c
        hunt/Places.c hunt/Players.h hunt/BinaryHeap.c hunt/BinaryHeap.h hunt/HashTable.c hunt/HashTable.h hunt/PathFinding.c hunt/PathFinding.h hunt/Queue.c hunt/Queue.h hunt/Players.c hunt/minimax.c hunt/minimax.h)

#target_compile_options(fury_of_dracula_player_hunt PRIVATE -O3)

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(fury_of_dracula_hunt PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(fury_of_dracula_hunt "${CMAKE_THREAD_LIBS_INIT}" m)
    target_link_libraries(fury_of_dracula_player_hunt "${CMAKE_THREAD_LIBS_INIT}" m)
endif()