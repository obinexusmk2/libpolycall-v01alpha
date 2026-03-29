# Cross-platform GNU Makefile for libpolycall

# Toolchain
CC ?= gcc
AR ?= ar

# Project layout
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
LIB_DIR := $(BUILD_DIR)/lib
BIN_DIR := $(BUILD_DIR)/bin

# Sources
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
MAIN_SRC := main.c
MAIN_OBJ := $(OBJ_DIR)/main.o

# Base flags
CPPFLAGS ?= -I./$(INC_DIR)
CFLAGS ?= -Wall -Wextra
LDFLAGS ?=
LDLIBS ?= -pthread -lssl -lcrypto
PLATFORM_LIBS :=
LINK_LIBS = $(LDLIBS) $(PLATFORM_LIBS)

# Platform-specific settings
UNAME_S := $(shell uname -s 2>/dev/null)
IS_WINDOWS := 0
ifeq ($(OS),Windows_NT)
    IS_WINDOWS := 1
endif
ifneq (,$(findstring MINGW,$(UNAME_S)))
    IS_WINDOWS := 1
endif
ifneq (,$(findstring MSYS,$(UNAME_S)))
    IS_WINDOWS := 1
endif
ifneq (,$(findstring CYGWIN,$(UNAME_S)))
    IS_WINDOWS := 1
endif

ifeq ($(IS_WINDOWS),1)
    EXE_EXT := .exe
    SHARED_EXT := dll
    SHARED_FLAG := -shared
    PLATFORM_LIBS += -lws2_32
    CPPFLAGS += -D_WIN32 -DPOLYCALL_DLL_EXPORT

    # cmd.exe-friendly commands
    MKDIR_P = if not exist "$(subst /,\\,$(1))" mkdir "$(subst /,\\,$(1))"
    RM_RF = if exist "$(subst /,\\,$(1))" rmdir /S /Q "$(subst /,\\,$(1))"
else
    EXE_EXT :=
    SHARED_EXT := so
    SHARED_FLAG := -shared

    MKDIR_P = mkdir -p "$(1)"
    RM_RF = rm -rf "$(1)"
endif

# Outputs
# Intended artifacts:
# - Static library: reusable archive for static linking.
# - Shared library: dynamically loadable library for runtime linking.
# - Executable: CLI binary linked against the static library.
LIB_NAME := libpolycall
STATIC_LIB := $(LIB_DIR)/$(LIB_NAME).a
SHARED_LIB := $(LIB_DIR)/$(LIB_NAME).$(SHARED_EXT)
EXECUTABLE := $(BIN_DIR)/polycall$(EXE_EXT)

# Build variants
DEBUG_FLAGS := -g -DDEBUG
RELEASE_FLAGS := -O2 -DNDEBUG

.PHONY: all debug release static shared dll bin clean distclean dirs help

all: dirs static shared bin

debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

release: CFLAGS += $(RELEASE_FLAGS)
release: all

# static output: build/lib/libpolycall.a
static: dirs $(STATIC_LIB)

# shared output: build/lib/libpolycall.$(SHARED_EXT)
shared: dirs $(SHARED_LIB)

# Windows convenience alias
dll: shared

# bin output: build/bin/polycall$(EXE_EXT)
bin: dirs $(EXECUTABLE)

dirs:
	@$(call MKDIR_P,$(BUILD_DIR))
	@$(call MKDIR_P,$(OBJ_DIR))
	@$(call MKDIR_P,$(LIB_DIR))
	@$(call MKDIR_P,$(BIN_DIR))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(MAIN_OBJ): $(MAIN_SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(STATIC_LIB): $(OBJS)
	$(AR) rcs $@ $^

$(SHARED_LIB): $(OBJS)
	$(CC) $(SHARED_FLAG) -o $@ $^ $(LDFLAGS) $(LINK_LIBS)

$(EXECUTABLE): $(MAIN_OBJ) $(STATIC_LIB)
	$(CC) -o $@ $(MAIN_OBJ) $(STATIC_LIB) $(LDFLAGS) $(LINK_LIBS)

clean:
	@$(call RM_RF,$(BUILD_DIR))

distclean: clean

-include $(DEPS) $(MAIN_OBJ:.o=.d)

help:
	@echo "Available targets:"
	@echo "  all     - Build static lib, shared lib, and executable"
	@echo "  debug   - Build all targets with debug flags"
	@echo "  release - Build all targets with release flags"
	@echo "  static  - Build static library ($(STATIC_LIB))"
	@echo "  shared  - Build shared library ($(SHARED_LIB))"
	@echo "  dll     - Alias of shared for Windows builds"
	@echo "  bin     - Build executable ($(EXECUTABLE))"
	@echo "  clean   - Remove build artifacts"
