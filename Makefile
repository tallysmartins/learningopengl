# Bundly Makefile
#-----------------------------------------------------------------------------

# Name of the project
PROJ_NAME=bundly

# Compiler used
CC=g++

# Flags for compiler
CC_FLAGS=-Iinclude \
         -W \
         -c \

PKGS=libglade-2.0
LD_FLAGS=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

# Clean command
RM=rm -rf

# .cpp files
CPP_SOURCE=$(wildcard ./src/*.cpp)

# .hpp files
HPP_SOURCE=$(wildcard ./include/*.hpp)

# Object files
OBJ=$(subst .cpp,.o,$(subst src,objects,$(CPP_SOURCE)))

###########################
# Compilation and linking #
###########################
all: objFolder $(PROJ_NAME)

full: clean all

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ $(LD_FLAGS) `pkg-config --cflags --libs $(PKGS)`
	@ echo 'Finished building binary: $@' 
	@ echo ' '

./objects/%.o: ./src/%.cpp ./include/%.hpp
	@ echo 'Building target file using GCC compiler: $<'
	$(CC) -c $< $(CC_FLAGS) -o $@ `pkg-config --cflags --libs $(PKGS)`
	@ echo ' '

./objects/%.o:
	@ echo 'Building target file using GCC compiler: $<'
	$(CC) -c $< $(CC_FLAGS) -o $@ `pkg-config --cflags --libs $(PKGS)`
	@ echo ' '

./objects/main.o: ./src/main.cpp $(HPP_SOURCE)
	@ echo 'Building main target using GCC compiler: $<'
	$(CC) -c $< $(CC_FLAGS) -o $@ `pkg-config --cflags --libs $(PKGS)`
	@ echo ' '

objFolder:
	@ mkdir -p objects

run:
	optirun ./bundly

clean:
	@ echo 'Cleaning object files'
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects

.PHONY: all clean
