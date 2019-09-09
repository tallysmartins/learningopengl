# Bundly Makefile
#-----------------------------------------------------------------------------

# Name of the project
PROJ_NAME=application

# Compiler used
CC=g++

# Flags for compiler
CC_FLAGS=-Iinclude \
         -W \
         -c \

#PKGS=libglade-2.0
#`pkg-config --cflags --libs $(PKGS)`

LD_FLAGS=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lshp
LD_PATHS=-rpath=/usr/local/lib/

# Clean command
RM=rm -rf

# .cpp and c files
CPP_SOURCE=$(wildcard ./src/*.cpp)
C_SOURCE=$(wildcard ./src/*.c)

# .hpp files
HPP_SOURCE=$(wildcard ./include/*.hpp)
H_SOURCE=$(wildcard ./include/*.h)

# Object files
OBJ=$(subst .cpp,.o,$(subst src,objects,$(CPP_SOURCE)))
C_OBJ=$(subst .c,.o,$(subst src,objects,$(C_SOURCE)))

###########################
# Compilation and linking #
###########################
all: objFolder $(PROJ_NAME)

full: clean all

$(PROJ_NAME): $(OBJ) $(C_OBJ)
	@ echo '(0) Building binary using GCC linker: $@'
	$(CC) $^ -Wl,$(LD_PATHS) -o $@ $(LD_FLAGS)  
	@ echo 'Finished building binary: $@' 
	@ echo ' '

./objects/%.o: ./src/%.cpp ./include/%.hpp
	@ echo '(1) Building target file using GCC compiler: $<'
	$(CC) -c $< $(CC_FLAGS) -o $@ 
	@ echo ' '

./objects/%.o: ./src/%.c ./include/%.h
	@ echo '(2) Building target file using GCC compiler: $<'
	gcc -c $< $(CC_FLAGS) -o $@ 
	@ echo ' '

./objects/%.o:
	@ echo '(3) Building target file using GCC compiler: $<'
	$(CC) -c $< $(CC_FLAGS) -o $@ 
	@ echo ' '

./objects/main.o: ./src/main.cpp $(HPP_SOURCE)
	@ echo '(4) Building main target using GCC compiler: $<'
	$(CC) -c $< $(CC_FLAGS) -o $@ 
	@ echo ' '

objFolder:
	@ mkdir -p objects

run:
	./$(PROJ_NAME)

clean:
	@ echo 'Cleaning object files'
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects

.PHONY: all clean
