BDIR = ../../../final_antasy

OBJS = server.cpp ../../anim.h ../../something.h ../../something.cpp ../../interaction.h

CC = g++

INCLUDE_PATHS = -I..\..\..\SDL2-2.0.9\i686-w64-mingw32\include\SDL2 -I..\..\..\SDL2_image-2.0.4\i686-w64-mingw32\include\SDL2

LIBRARY_PATHS = -L..\..\..\SDL2-2.0.9\i686-w64-mingw32\lib -L..\..\..\SDL2_image-2.0.4\i686-w64-mingw32\lib

COMPILER_FLAGS = -w -std=c++11

LINKER_FLAGS = -lmingw32 -lwsock32 -lWs2_32 -lSDL2main -lSDL2 -lSDL2_image

OBJ_NAME = $(BDIR)/host

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

#clean:
#	del /S $(BDIR)