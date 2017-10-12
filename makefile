OBJS = src/TestFork.o src/hashmap.o
LIB_PATH = -L/usr/local/lib
LIBS = -pthread
TARGET =	TestFork

$(TARGET):	$(OBJS)
	gcc -o $@ $^ $(LIB_PATH) $(LIBS)

# All Target
all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY : clean dep