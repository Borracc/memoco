CC = g++
CPPFLAGS = -g -Wall -O2
LDFLAGS =

OBJ = generatoreIstanze.o

%.o: %.cpp
		$(CC) $(CPPFLAGS) -c $^ -o $@

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o generatoreIstanze
		
clean:
		rm -rf $(OBJ) generatoreIstanze

.PHONY: clean
