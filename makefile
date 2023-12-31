CC = gcc
OBJ = filetree.o directoryControl.o fileControl.o adminControl.o main.o utils.o 

EXE = run
all: $(EXE)
.c.o: ; $(CC) -c $*.c

$(EXE): $(OBJ)
	$(CC) -o $@ $(OBJ)

clean:
	rm -rf $(EXE) *.o *.d core

