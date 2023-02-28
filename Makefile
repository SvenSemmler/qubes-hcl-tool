# -------------------------------------------------------------------------
# Qubes OS Hardware Compatibility List Tool
# Reads HCL reports and exports CSV.
# 
# Module: Makefile
# Author: Sven Semmler (Sven@SvenSemmler.org)
# -------------------------------------------------------------------------

UNAME_S         := $(shell uname -s)

DIR_SRC          = src
DIR_BUILD        = build
DIR_OBJ          = $(DIR_BUILD)/obj
DIR_BIN          = $(DIR_BUILD)/bin

TARGET           = qubes-hcl-tool

CC               = clang
LINK             = clang
LINT             = splint
RM               = rm -rf
MD               = mkdir -p

CC_STD           = -ansi -pedantic -std=c17
CC_INC           = -I $(DIR_SRC)
CC_DBG           = -g3
CC_WRN           = -Weverything -Werror

ifeq ($(UNAME_S),Darwin)
  CC_WRN_EXCLUDE = -Wno-padded -Wno-poison-system-directories
  LINK_FLAGS     =
else
  CC_WRN_EXCLUDE = -Wno-padded
  LINK_FLAGS     = -static
endif

CC_FLAGS         = $(CC_STD) $(CC_INC) $(CC_DBG) $(CC_WRN) $(CC_WRN_EXCLUDE)
LINT_FLAGS       = $(CC_INC) +quiet

SOURCES         := $(wildcard $(DIR_SRC)/*.c)
OBJECTS         := $(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SOURCES))
DEPENDENCIES    := $(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.d, $(SOURCES))

.PHONY : all clean default lint

all : default lint

default : $(DIR_BIN)/$(TARGET)

clean : 
	@$(RM) $(DIR_BUILD)

lint : 
	@$(LINT) $(LINT_FLAGS) $(SOURCES)

$(DIR_BIN)/$(TARGET) : $(OBJECTS)
	@$(MD) $(@D)
	@$(LINK) $(OBJECTS) $(LINK_FLAGS) -o $@

$(DIR_OBJ)%.o: $(DIR_SRC)%.c
	@echo $<
	@$(MD) $(@D)
	@$(CC) $(CC_FLAGS) -MD -MF "$(@:%.o=%.d)" -o $@ -c $<

include $(wildcard $(DEPENDENCIES))
