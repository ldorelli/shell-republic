CC = g++
SHAREDC = Executor.cpp \
		Builtin.cpp \
		Command.cpp \
		CommandLine.cpp \
		Executor.cpp \
		Handlers.cpp \
		Parser.cpp \
		MyTypo.cpp \
		Program.cpp 
MAINC = main.cpp
SHAREDLIB = libshell.so
LIBS = -lshell
CFLAGS = -O2
OBJS = Executor.o \
		Builtin.o \
		Command.o \
		CommandLine.o \
		Executor.o \
		Handlers.o \
		Parser.o \
		MyTypo.o \
		Program.o 

ALL:
	$(CC) -fPIC -c $(SHAREDC) $(CFLAGS)
	$(CC) -fPIC -shared -o $(SHAREDLIB) *.o
	$(CC) -o shell $(MAINC) -L. $(LIBS) 
	mv $(SHAREDLIB) /usr/lib

