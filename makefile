EXECUTEABLE=asmjmp

CC=g++

$(EXECUTEABLE): main.cpp
	$(CC) -o $(EXECUTEABLE) main.cpp
