TARGET=choice

CC=g++
RM=rm -rf
CCFLAGS=

inc=

objs=\
	gaussrand.o \
	choice.o \
#	testgaussrand.o \

all:$(objs)
	$(CC) $(CCFLAGS) $(inc) -o $(TARGET) $(objs)

clean:
	$(RM) $(TARGET) $(objs)
	
