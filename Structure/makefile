
CC = g++

NAME   = SutructureTest

CFLAGS = -std=c++11

HEADER = Heuristic.h \
		 Block.h     \
		 Field.h     \

SOURCES= test.cpp      \
		 Heuristic.cpp \
		 Block.cpp     \
		 Field.cpp     \


BUILD  = $(CC) -o $(NAME) $(CFLAGS)  $(SOURCES)

RUN    = ./$(NAME)

ALL:build run

build:
	$(BUILD)

run:
	$(RUN)

