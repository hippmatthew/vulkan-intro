include .env

# Compiler Configuration
CC = g++
CFLAGS_DEFAULT := -O2 -std=c++17 `pkg-config --cflags glfw3`
INCLUDE := -I$(VULKAN_SDK_DIR)/include -I$(BREW_DIR)/include -I$(LOCAL_SRC)/include
LDFLAGS := -L$(VULKAN_SDK_DIR)/lib -lvulkan `pkg-config --static --libs glfw3`

# Code
SRCS := $(wildcard $(LOCAL_SRC)/*.cpp)
OBJS := $(patsubst $(LOCAL_SRC)/%.cpp, $(LOCAL_OBJ)/%.o, $(SRCS))
OBJS_DEBUG := $(patsubst $(LOCAL_SRC)/%.cpp, $(LOCAL_OBJ)/%.o.debug, $(SRCS))
BINS := $(LOCAL_BIN)/graphics

# Shader Information
SC = glslc
VERT_SHDRS := $(wildcard $(LOCAL_VERT)/*.vert)
FRAG_SHDRS := $(wildcard $(LOCAL_FRAG)/*.frag)
SPVS := $(patsubst $(LOCAL_VERT)/%.vert, $(LOCAL_SPV)/%.vert.spv, $(VERT_SHDRS)) $(patsubst $(LOCAL_FRAG)/%.frag, $(LOCAL_SPV)/%.frag.spv, $(FRAG_SHDRS))

# Functions
debug: CFLAGS := $(CFLAGS_DEFAULT)
debug: $(SPVS) $(BINS).debug

release: CFLAGS := $(CFLAGS_DEFAULT) -DNDEBUG
release: $(SPVS) $(BINS)

$(LOCAL_SPV)/%.vert.spv: $(LOCAL_VERT)/%.vert
	$(SC) $< -o $@

$(LOCAL_SPV)/%.frag.spv: $(LOCAL_FRAG)/%.frag
	$(SC) $< -o $@

$(LOCAL_BIN)/graphics: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) $(OBJS) $(LDFLAGS)

$(LOCAL_BIN)/graphics.debug: $(OBJS_DEBUG)
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) $(OBJS_DEBUG) $(LDFLAGS)

$(LOCAL_OBJ)/%.o: $(LOCAL_SRC)/%.cpp
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) -c $<

$(LOCAL_OBJ)/%.o.debug: $(LOCAL_SRC)/%.cpp
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) -c $<

clean:
	rm -r $(LOCAL_BIN)/* $(LOCAL_OBJ)/* $(LOCAL_SPV)/*

clean_debug:
	rm -r $(LOCAL_BIN)/*.debug $(LOCAL_OBJ)/*.debug $(LOCAL_SPV)/*