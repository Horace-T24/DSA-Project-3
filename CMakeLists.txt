cmake_minimum_required(VERSION 3.29)
project(DSA_Project_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(DSA_Project_3
    Formations.h
    main.cpp
    Player_Organizer.h
    Requirements.h
    SquadOptimizer.h
        SquadHelper.h
        )
