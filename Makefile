# Executable names:
EXE = generate_graph
TEST = test

# Add all object files needed for compiling:
EXE_OBJ = main.o
OBJS = graph.o main.o pageRank.o BFS.o

# Use the cs225 makefile template:
include cs225/make/cs225.mk
