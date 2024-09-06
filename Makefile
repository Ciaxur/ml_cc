TARGET := app
CC := g++
COMPILER_MACROS := -D SPDLOG_COMPILED_LIB
OPTIMIZATIONS := -O3 -g
FLAGS := -std=c++20 -lglfw -lGLEW -lGL -Wall -Wextra -Wno-unused-parameter $(COMPILER_MACROS) $(OPTIMIZATIONS)
INCLUDES := $(patsubst %,-I%, \
	./vendors/ \
	./vendors/spdlog/include \
	./vendors/imgui \
	./vendors/implot \
	./ \
	$(wildcard ./**/) \
)

# ImGUI backend source paths
IMGUI_PATH := ./vendors/imgui
IMGUI_BACKEND := $(IMGUI_PATH)/backends/imgui_impl_glfw.cpp $(IMGUI_PATH)/backends/imgui_impl_opengl3.cpp
IMGUI_SRCS := $(IMGUI_BACKEND) $(wildcard $(IMGUI_PATH)/*.cpp)

# implot source paths
IMPLOT_PATH := ./vendors/implot
IMPLOT_SRCS := $(wildcard $(IMPLOT_PATH)/*.cpp)

# spdlog source paths
SPDLOG_PATH := ./vendors/spdlog
SPDLOG_SRCS := $(wildcard $(SPDLOG_PATH)/src/*.cpp)

# Source files
SRCS 	:= $(wildcard ./src/**/*.cc ./src/**/**/*.cc ./src/*.cc) $(IMGUI_SRCS) $(SPDLOG_SRCS) $(IMPLOT_SRCS)
_OBJS := $(SRCS:.cc=.o)
OBJS 	:= $(_OBJS:.cpp=.o)

all: $(TARGET)

# Main build binary target.
# $@ -> $(TARGET)
# $^ -> All pre-requisites, which are $(OBJS)
$(TARGET): $(OBJS)
	$(CC) $(INCLUDES) $(FLAGS) $^ -o $@

# Rule to compile source files to object files. This allows re-compiling modified source files.
# $< -> The %.cc's resolved filename
%.o: %.cc
	$(CC) $(INCLUDES) $(FLAGS) -c -o $@ $<
%.o: %.cpp
	$(CC) $(INCLUDES) $(FLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

debug:
	$(info Sources:)
	$(foreach src,$(SRCS),$(info - $(src)))

	$(info Objects:)
	$(foreach obj,$(OBJS),$(info - $(obj)))

	$(info Includes:)
	$(foreach file,$(INCLUDES),$(info - $(file)))

	$(info Flags:)
	$(foreach flag,$(FLAGS),$(info - $(flag)))

help:
	@echo "Available targets:"
	@echo "  all   				: Builds everything"
	@echo "  clean  			: Cleans up all built objects, cache, and binaries"
	@echo "  debug  			: Prints debug menu, which contains resolved variables"
	@echo "  help   			: Prints this menu"
