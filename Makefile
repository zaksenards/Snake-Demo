CC:= g++
LIB:= -lraylib -lgdi32 -lwinmm
INC:= C:\raylib\raylib\src

All:
	$(CC) main.cpp $(LIB) -I $(INC) -o snake
