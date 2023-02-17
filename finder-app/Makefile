CC=$(CROSS_COMPILE)gcc            #  is this the compiler?
CFLAGS=-I.			# options for compiler
DEPS = 				# header files
OBJ = writer.o		# c files turn into object files somehow
# CROSS_COMPILE = aarch64-none-linux-gnu-

# target: and rule are same thing

%.o: %.c $(DEPS)                    # use c file and dependancy to creat an objejct file
	$(CC) -c -o $@ $< $(CFLAGS)			
# something about compilation 
# $@ is the name of the target being generated, and $< the first prerequisite (usually a source file).

writer: $(OBJ)							# make writer
	$(CC) -o $@ $^ $(CFLAGS)       		
# compile something

.PHONY: clean
# clean will always execute

clean:
	rm -f writer *.o