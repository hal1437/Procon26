
CC = g++

NAME   = Procon26-Hal

CFLAGS = -std=c++11

HEADER = ../Structure/Heuristic.h \
		 ../Structure/Block.h     \
		 ../Structure/Field.h     \
		 ../Structure/Problem.h   \
		 ../Structure/Answer.h    \

SOURCES= main.cpp      \
		 Heuristics/Density.cpp     \
		 Heuristics/DensityAround.cpp\
		 Heuristics/Cavity.cpp      \
		 Heuristics/WeightComposit.cpp\
		 Solver/BestFirst.cpp       \
		 Solver/AStar.cpp           \
		 ../Structure/Hand.cpp      \
		 ../Structure/Point.cpp     \
		 ../Structure/Heuristic.cpp \
		 ../Structure/Block.cpp     \
		 ../Structure/Field.cpp     \
		 ../Structure/Problem.cpp   \
		 ../Structure/Answer.cpp    \


BUILD  = $(CC) -g -O0 -o $(NAME) $(CFLAGS)  $(SOURCES)

RUN    = ./$(NAME)

ALL:build run

build:
	$(BUILD)

run:
	$(RUN)

