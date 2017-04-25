SRC= project.cpp
OBJ = $(SRC:.c=.o)
CC=g++
CFLAGS= -std=c++14 -Wall -W -g

project:	$(OBJ)
			$(CC) $(OBJ) $(CFLAGS) -o project

%.o:		%.c
			$(CC) $(CFLAGS) -c $<

clean:
	rm *.o *~ core