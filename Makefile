CC = g++
CFLAG=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -g
LIB = ponglib.h

OBJ = main.o function.o

%.o: %.cpp $(LIB)
	$(CC) -c -o $@ $< $(CFLAG)

program: $(OBJ)
	$(CC) -o $@ $^ $(CFLAG)  

clean: 
	rm $(OBJ)
