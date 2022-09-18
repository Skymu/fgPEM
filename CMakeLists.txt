cmake_minimum_required(VERSION 3.21)
project(PEM)

set(CMAKE_CXX_STANDARD 17)

add_executable(PEM main.cpp enmat.cpp enmat.h enmatp.cpp enmatp.h generator.cpp generator.h printer.cpp printer.h opIndex.cpp opIndex.h rein.cpp rein.h tama.cpp tama.h util.cpp util.h chrono_time.h data_structure.h enmatp_fg.cpp enmatp_fg.h adarein.cpp adarein.h)

set_target_properties(PEM PROPERTIES LINK_FLAGS -Wl,--stack,67108864)