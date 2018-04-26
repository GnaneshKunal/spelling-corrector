CC = g++

CXXFLAGS = -g -std=c++11

SRCS = spell.cpp

OBJS = $(SRCS:.cpp=.o)

MAIN = spell

.PHONY: depend clean

TARGET = main

all: $(MAIN)
		@echo  Spelling-corrector has been compiled

$(MAIN): $(OBJS)
		$(CC) $(CXXFLAGS) -o $(MAIN) $(OBJS) 

.c.o:
		$(CC) $(CXXFLAGS)  -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)
