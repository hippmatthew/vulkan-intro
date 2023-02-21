import .env

# Configuration
CC = g++
CXXFLAGS = -O2 -std=c++17 `pkg-config --cflags glfw3`
INCLUDE = -I$(VULKAN_SDK_DIR)/include -I$(LIB_DIR) -I$(LOCAL_SRC)/include
LDFLAGS = -L$(VULKAN_SDK_DIR)/lib -lvulkan `pkg-config --static --libs glfw3`

# Files
SRCS = $(wildcard $(LOCAL_SRC)/*.cpp)
OBJS = $(patsubst $(LOCAL_SRC)/%.cpp, $(LOCAL_OBJ)/%.o, $(SRCS))
BINS = $(LOCAL_BIN)/graphics

all: $(BINS)

$(LOCAL_BIN)/graphics: $(OBJS)
	$(CC) -o $@ $(CXXFLAGS) $(INCLUDE) $(OBJS) $(LDFLAGS)

$(LOCAL_OBJ)/%.o: $(LOCAL_SRC)/%.cpp
	$(CC) -o $@ $(CXXFLAGS) $(INCLUDE) -c $< $(LDFLAGS)

clean:
	rm -r $(LOCAL_BIN)/* $(LOCAL_OBJ)/*