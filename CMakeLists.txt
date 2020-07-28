cmake_minimum_required(VERSION 3.13)
include_directories(${CMAKE_SOURCE_DIR})

project(fury_of_dracula)

add_executable(fury_of_dracula view/testGameView.c
        view/testUtils.c
        view/DraculaView.c
        view/GameView.c
        view/HunterView.c
        view/Map.c
        view/Places.c view/Players.h view/binary_heap.c view/binary_heap.h view/hash_table.c view/hash_table.h view/path_finding.c view/path_finding.h view/Queue.c view/Queue.h view/Players.c)


project(fury_of_dracula_hunt)

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
        view/Places.c view/Players.h view/binary_heap.c view/binary_heap.h view/hash_table.c view/hash_table.h view/path_finding.c view/path_finding.h view/Queue.c view/Queue.h view/Players.c)