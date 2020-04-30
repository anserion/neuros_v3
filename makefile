PROG= kernel
OBJS= kernel.o addr.o alg.o f.o commutator.o neuron.o nweb.o sweb.o drv.o debug.o

FLAGS= -I/usr/X11R6/include
LIBS= -L/usr/X11R6/lib -lm -lGL -lGLU -lglut

CC= gcc

all: $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LIBS)
	rm *.o
	cp $(PROG) ../RELEASE

kernel.o:
	$(CC) $(FLAGS) -c kernel.c
neuron.o:
	$(CC) $(FLAGS) -c neuron.c
commutator.o:
	$(CC) $(FLAGS) -c commutator.c
drv.o:
	$(CC) $(FLAGS) -c drv.c
nweb.o:
	$(CC) $(FLAGS) -c nweb.c
sweb.o:
	$(CC) $(FLAGS) -c sweb.c
f.o:
	$(CC) $(FLAGS) -c f.c
addr.o:
	$(CC) $(FLAGS) -c addr.c
alg.o:
	$(CC) $(FLAGS) -c alg.c
debug.o:
	$(CC) $(FLAGS) -c debug.c