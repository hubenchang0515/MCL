TARGET := test.out
LIBS := -lMCL -lOpenCL
CXXFLAGS := -W -Wall -Wextra -Werror -std=c++14

SRCS := $(wildcard *.cpp)
HEADERS := $(wildcard *.h)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

.PHONY: all clean 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

clean:
	$(RM) $(OBJS)