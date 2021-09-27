TARGET := libMCL.so

LIBS := -lOpenCL

CXXFLAGS := -W -Wall -Wextra -Werror -fPIC

SRCS := $(wildcard *.cpp)
HEADERS := $(wildcard *.h)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
INSTALL_PATH := $(DESTDIR)$(PREFIX)

.PHONY: all clean install

all: $(TARGET)

install: all
	install -d $(INSTALL_PATH)/include/MCL
	install -m0644 $(HEADERS) $(INSTALL_PATH)/include/MCL
	install -m0644 $(TARGET) $(INSTALL_PATH)/lib

$(TARGET): $(OBJS)
	$(CXX) --shared -o $@ $^ $(LIBS)

Platform.o: Platform.cpp Platform.h
Device.o: Device.cpp Device.h Platform.h
Context.o: Context.cpp Context.h Device.h

clean:
	$(RM) $(OBJS)