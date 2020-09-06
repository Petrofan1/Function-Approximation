QT += widgets 
QT += opengl
QMAKE_CXXFLAGS+= -g
LIBS += -lGLU
HEADERS       = widget.h header.hpp triangle.hpp 
SOURCES       = drawing.cpp actions.cpp calculations.cpp system.cpp functions.cpp triangle.cpp main.cpp
TARGET        = a.out
