include .env

# Configuration
CC = g++
CFLAGS_DEFAULT = -O2 -std=c++17 `pkg-config --cflags glfw3`
INCLUDE = -I$(VULKAN_SDK_DIR)/include -I$(BREW_DIR)/include -I$(LOCAL_SRC)/include
LDFLAGS = -L$(VULKAN_SDK_DIR)/lib -lvulkan `pkg-config --static --libs glfw3`

# Files
SRCS = $(wildcard $(LOCAL_SRC)/*.cpp)
OBJS = $(patsubst $(LOCAL_SRC)/%.cpp, $(LOCAL_OBJ)/%.o, $(SRCS))
OBJS_DEBUG = $(patsubst $(LOCAL_SRC)/%.cpp, $(LOCAL_OBJ)/%.o.debug, $(SRCS))
BINS = $(LOCAL_BIN)/graphics

# Functions
debug: CFLAGS = $(CFLAGS_DEFAULT)
debug: $(BINS).debug

release: CFLAGS = $(CFLAGS_DEFAULT) -DNDEBUG
release: $(BINS)

$(LOCAL_BIN)/graphics: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) $(OBJS) $(LDFLAGS)

$(LOCAL_BIN)/graphics.debug: $(OBJS_DEBUG)
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) $(OBJS_DEBUG) $(LDFLAGS)

$(LOCAL_OBJ)/%.o: $(LOCAL_SRC)/%.cpp
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) -c $< $(LDFLAGS)

$(LOCAL_OBJ)/%.o.debug: $(LOCAL_SRC)/%.cpp
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) -c $< $(LDFLAGS)

run:
	bin/graphics

run_debug:
	bin/graphics.debug

clean:
	rm -r $(LOCAL_BIN)/* $(LOCAL_OBJ)/*