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
CFLAGS = -pg -O2

ALL:
	$(CC) -fPIC -c $(SHAREDC) $(CFLAGS)
	$(CC) -pg -fPIC -shared -o $(SHAREDLIB) *.o
	$(CC) -pg -o shell $(MAINC) -L. $(LIBS) 
	mv $(SHAREDLIB) /usr/lib

