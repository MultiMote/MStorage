SOURCES	= example.cpp ./../MStorage.cpp

LDFLAGS_RELEASE = -s
CDFLAGS_RELEASE = -O2 -Wall

CFLAGS		?= -g -Wall
LDPATH		?= -L. 
LDFLAGS		?= 
INCLUDES	= -I. -I./..

OBJECTS		= $(SOURCES:.cpp=.o)
OBJECTS		:= $(OBJECTS:.c=.o)
DEPS		= $(OBJECTS:.o=.d)

TARGET	= $(TARGET_BINARY)


all: debug

release: LDFLAGS=$(LDFLAGS_RELEASE)
release: CFLAGS=$(CDFLAGS_RELEASE)
release: debug
	

debug: $(SOURCES) $(TARGET)


$(TARGET): $(OBJECTS)
	$(CXX) $(LDPATH) -o $@ $(OBJECTS) $(LDFLAGS)


.cpp.o: 
	$(CXX) -c -MMD -MP $(CFLAGS) $(INCLUDES) $< -o $@

.c.o:
	$(CC) -c -MMD -MP $(CFLAGS) $(INCLUDES) $< -o $@

.PHONY: clean debug release

clean:
	$(RM) *.o $(TO_NULL) 
	$(RM) *.d $(TO_NULL) 
	$(RM) .$(OS_SEPARATOR)..$(OS_SEPARATOR)*.o $(TO_NULL) 
	$(RM) .$(OS_SEPARATOR)..$(OS_SEPARATOR)*.d $(TO_NULL) 
	$(RM) $(TARGET_BINARY) $(TO_NULL)
	$(RM) data.mst $(TO_NULL)

-include $(DEPS)
