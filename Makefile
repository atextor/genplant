# genplant Makefile
# A. Textor
GCC=gcc
FLAGS=-Iinclude -Wall -pedantic -g -ansi `sdl-config --cflags` -DVRML
PROG=genplant
LIBS=`sdl-config --libs` -lm
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

default: all

all: program

%.o: %.c
	$(GCC) $(FLAGS) -c -o $@ $<

clean:
	-rm -f $(OBJS) $(PROG)

program: $(OBJS)
	$(GCC) $(FLAGS) -o $(PROG) $(OBJS) $(LIBS)

