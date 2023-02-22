include .env

# Configuration
CC = g++
CFLAGS = -O2 -std=c++17 `pkg-config --cflags glfw3`
INCLUDE = -I$(VULKAN_SDK_DIR)/include -I$(BREW_DIR)/include -I$(LOCAL_SRC)/include
LDFLAGS = -L$(VULKAN_SDK_DIR)/lib -lvulkan `pkg-config --static --libs glfw3`

# Files
SRCS = $(wildcard $(LOCAL_SRC)/*.cpp)
OBJS = $(patsubst $(LOCAL_SRC)/%.cpp, $(LOCAL_OBJ)/%.o, $(SRCS))
BINS = $(LOCAL_BIN)/graphics

# Functions
all: $(BINS)

$(LOCAL_BIN)/graphics: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) $(OBJS) $(LDFLAGS)

$(LOCAL_OBJ)/%.o: $(LOCAL_SRC)/%.cpp
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) -c $< $(LDFLAGS) 

clean:
	rm -r $(LOCAL_BIN)/* $(LOCAL_OBJ)/*

test: test.cpp
	$(CC) -o test $(CFLAGS) $(INCLUDE) $< $(LDFLAGS)

clean_test:
	rm -r test test.cpp