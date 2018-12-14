
CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

# Links together files needed to create executable
project: project.o ppmIO.o imageManip.o
	$(CC) -o project project.o ppmIO.o imageManip.o -lm

# Compiles project.c to create project.o
project.o: project.c ppmIO.h imageManip.h
	$(CC) $(CFLAGS) -c project.c

#Compiles test_search_functions.c to create test_search_functions.o
#test_search_functions.o: test_search_functions.c search_functions.h
#	$(CC) $(CFLAGS) -c test_search_functions.c

# Compiles functions.c to create functions.o
ppmIO.o: ppmIO.c ppmIO.h
	$(CC) $(CFLAGS) -c ppmIO.c

# Compiles functions.c to create functions.o
imageManip.o: imageManip.c imageManip.h
	$(CC) $(CFLAGS) -c imageManip.c

# Removes all object files and the executable named main,
# so we can start fresh
clean:
	rm -f *.o project
