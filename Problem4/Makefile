#File names
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o); 

#Target
problem4.exe: $(OBJ)
	gcc -o problem4.exe $(OBJ) 

#Generate object files
%.o: %.c 
	gcc -c $< -o $@

#Remove generated files
clean:
	rm -f problem4.exe $(OBJ)
	rm *.stackdump
