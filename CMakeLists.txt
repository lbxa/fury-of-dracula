cmake_minimum_required(VERSION 3.13)
include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm")
project(fury_of_dracula C)
project(fury_of_dracula_hunt)

add_executable(fury_of_dracula view/testGameView.c
        view/testUtils.c
        view/DraculaView.c
        view/GameView.c
        view/HunterView.c
        view/Map.c
        view/Places.c view/Players.h view/BinaryHeap.c view/BinaryHeap.h view/HashTable.c view/HashTable.h view/PathFinding.c view/PathFinding.h view/Queue.c view/Queue.h view/Players.c view/Utilities.c view/Utilities.h)




add_executable(fury_of_dracula_hunt hunt/Game.c
        hunt/dracula.c
        hunt/dracula.h
        hunt/hunter.c
        hunt/hunter.c
        view/testUtils.c
        view/DraculaView.c
        view/GameView.h
        view/GameView.c
        view/HunterView.c
        view/Map.c
        view/Places.c view/Players.h view/BinaryHeap.c view/BinaryHeap.h view/HashTable.c view/HashTable.h view/PathFinding.c view/PathFinding.h view/Queue.c view/Queue.h view/Players.c hunt/minimax.c hunt/minimax.h)

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(fury_of_dracula_hunt PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(fury_of_dracula_hunt "${CMAKE_THREAD_LIBS_INIT}" m)
endif()
