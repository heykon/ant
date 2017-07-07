#OBJS specifies which files to compile as part of the project
OBJS = new.cpp anim.h something.h something.cpp ant.h ant.cpp interaction.h

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:\Users\ajax\Desktop\otherstuff\SDL2-2.0.5\i686-w64-mingw32\include\SDL2 -IC:\Users\ajax\Desktop\otherstuff\SDL2_image-2.0.1\include

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\Users\ajax\Desktop\otherstuff\SDL2-2.0.5\i686-w64-mingw32\lib -LC:\Users\ajax\Desktop\otherstuff\SDL2_image-2.0.1\lib\x86

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = antantant

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)