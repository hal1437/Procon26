
CC = g++

NAME   = SutructureTest

CFLAGS = -std=c++11

HEADER = Heuristic.h \
		 Block.h     \
		 Field.h     \
		 Problem.h   \
		 Answer.h    \

SOURCES= test.cpp      \
		 Hand.cpp      \
		 Heuristic.cpp \
		 Block.cpp     \
		 Field.cpp     \
		 Problem.cpp   \
		 Answer.cpp    \
		 Point.cpp     \


BUILD  = $(CC) -g -O0 -o $(NAME) $(CFLAGS)  $(SOURCES)

RUN    = ./$(NAME)

ALL:build run

build:
	$(BUILD)

run:
	$(RUN)

