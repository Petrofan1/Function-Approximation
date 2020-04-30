QT += widgets 
QMAKE_CXXFLAGS+= -O3 -march=native -ffast-math -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format
HEADERS       = header.hpp window.h 
SOURCES       = functions.cpp main.cpp drawing.cpp approximation_0.cpp approximation_1.cpp calculations.cpp actions.cpp
TARGET        = a.out
