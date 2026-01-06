CC = gcc

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    PLATFORM = LINUX
    INSTALL_CMD = sudo apt-get update && sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
else ifeq ($(UNAME_S), Darwin)
    PLATFORM = OSX
    INSTALL_CMD = brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf
else
    $(error Unsupported platform)
endif

SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LFLAGS := $(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer -lSDL2_ttf

SRCDIR = .
ENGINE_GRAPHICS_DIR = engine/core/graphics
ENGINE_MATH_DIR = engine/core/math
ENGINE_INPUT_DIR = engine/core/input
ENGINE_AUDIO_DIR = engine/core/audio
ENGINE_TIME_DIR = engine/core/time
ENGINE_UTILS_DIR = engine/core/utils
ENGINE_MEMORY_DIR = engine/core/memory
ENGINE_EVENTS_DIR = engine/core/events

GAME_MAIN_DIR = game/src/main
GAME_STAGES_DIR = game/src/stages
GAME_ENTITIES_DIR = game/src/entities
GAME_CONTROLLERS_DIR = game/src/controllers
GAME_COLLISION_DIR = game/src/collision
GAME_RENDERING_DIR = game/src/rendering
GAME_MANAGERS_DIR = game/src/managers
GAME_SCORING_DIR = game/src/scoring
GAME_EVENTS_DIR = game/src/events

# Find all C source files in game directories only (engine is now a library)
SRC = $(wildcard $(GAME_MAIN_DIR)/*.c) $(wildcard $(GAME_STAGES_DIR)/*.c) $(wildcard $(GAME_ENTITIES_DIR)/*.c) $(wildcard $(GAME_CONTROLLERS_DIR)/*.c) $(wildcard $(GAME_COLLISION_DIR)/*.c) $(wildcard $(GAME_RENDERING_DIR)/*.c) $(wildcard $(GAME_MANAGERS_DIR)/*.c) $(wildcard $(GAME_SCORING_DIR)/*.c) $(wildcard $(GAME_EVENTS_DIR)/*.c)

HEADERS = $(wildcard $(SRCDIR)/*.h) \
          $(wildcard $(ENGINE_GRAPHICS_DIR)/*.h) $(wildcard $(ENGINE_MATH_DIR)/*.h) $(wildcard $(ENGINE_INPUT_DIR)/*.h) $(wildcard $(ENGINE_AUDIO_DIR)/*.h) $(wildcard $(ENGINE_TIME_DIR)/*.h) $(wildcard $(ENGINE_UTILS_DIR)/*.h) $(wildcard $(ENGINE_MEMORY_DIR)/*.h) $(wildcard $(ENGINE_EVENTS_DIR)/*.h) \
          $(wildcard $(GAME_MAIN_DIR)/*.h) $(wildcard $(GAME_STAGES_DIR)/*.h) $(wildcard $(GAME_ENTITIES_DIR)/*.h) $(wildcard $(GAME_CONTROLLERS_DIR)/*.h) $(wildcard $(GAME_COLLISION_DIR)/*.h) $(wildcard $(GAME_RENDERING_DIR)/*.h) $(wildcard $(GAME_MANAGERS_DIR)/*.h) $(wildcard $(GAME_SCORING_DIR)/*.h) $(wildcard $(GAME_EVENTS_DIR)/*.h)

OBJ = $(SRC:.c=.o)

# Add include paths
INCLUDES = -I. \
           -I$(ENGINE_GRAPHICS_DIR) -I$(ENGINE_MATH_DIR) -I$(ENGINE_INPUT_DIR) -I$(ENGINE_AUDIO_DIR) -I$(ENGINE_TIME_DIR) -I$(ENGINE_UTILS_DIR) -I$(ENGINE_MEMORY_DIR) -I$(ENGINE_EVENTS_DIR) \
           -I$(GAME_MAIN_DIR) -I$(GAME_STAGES_DIR) -I$(GAME_ENTITIES_DIR) -I$(GAME_CONTROLLERS_DIR) -I$(GAME_COLLISION_DIR) -I$(GAME_RENDERING_DIR) -I$(GAME_MANAGERS_DIR) -I$(GAME_SCORING_DIR) -I$(GAME_EVENTS_DIR)

CFLAGS := -ggdb3 -Ofast --std=c99 -Wall -Wextra -pedantic-errors $(INCLUDES) $(SDL2_CFLAGS)
ENGINE_LIB = engine/libsdl2d.a
LFLAGS := $(SDL2_LFLAGS) -lm

TARGET = deadly-duck

.PHONY: all install clean run

all: $(TARGET)

$(TARGET): $(OBJ) $(ENGINE_LIB)
	$(CC) -o $@ $(OBJ) $(ENGINE_LIB) $(LFLAGS)

$(ENGINE_LIB):
	$(MAKE) -C engine

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

install:
	$(INSTALL_CMD)

clean:
	rm -f $(OBJ) $(TARGET)
	$(MAKE) -C engine clean

run: $(TARGET)
	./$(TARGET)