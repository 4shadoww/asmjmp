EXECUTEABLE=asmjmp

CC=g++

$(EXECUTEABLE): main.cpp
	$(CC) -o asmjmp main.cpp
