#OBJS specifies which files to compile as part of the project
OBJS = Source.cpp

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -ID:\Konstantinos\Programming\libraries\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LD:\Konstantinos\Programming\libraries\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -Wall

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf Worm.res

#OBJ_NAME specifies the name of our executable
OBJ_NAME = Build/Worm.exe

#This is the target that compiles our executable
all : build
	./$(OBJ_NAME)

build : $(OBJS) Worm.res
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	powershell "Remove-Item -ErrorAction Ignore Worm.res; $$null"

Worm.res :
	windres Worm.rc -O coff -o Worm.res

clean:
	powershell "Remove-Item -ErrorAction Ignore $(OBJ_NAME); $$null"