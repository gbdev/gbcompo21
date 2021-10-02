# Simple makefile for assembling and linking a GB program.
rwildcard		=	$(foreach d,$(wildcard $1*), $(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

#RGBDS programs
ASSEMBLER		:=	rgbds/rgbasm
LINKER			:=	rgbds/rgblink
FIXER			:=	rgbds/rgbfix

#Project name/directory
PROJECT_NAME	:=	$(shell basename "$(CURDIR)")

#Output files/directory
BUILD_DIR		:=	build
OUTPUTF			:=	$(BUILD_DIR)/build
OUTPUT			:=	$(BUILD_DIR)/build
SYMBOL			:=	$(BUILD_DIR)/build

#Project file directories
INC_DIR			:=	include/
SRC_DIR			:=	source
SRC_Z80			:=	$(call rwildcard, $(SRC_DIR)/, *.asm)

#Macros for assembly/linking
OBJ_FILES		:=	$(addprefix $(BUILD_DIR)/obj/, $(SRC_Z80:source/%.asm=%.o))
OBJ_DIRS 		:=	$(sort $(addprefix $(BUILD_DIR)/obj/, $(dir $(SRC_Z80:source/%.asm=%.o))))

#Flags for various programs
ASSEMBLER_FLAGS	:=	-p 255 -i $(INC_DIR)
FIXER_FLAGS		:=  -p 255 -v -C -t "SPELUNKY TWO" -j -m 1



#Reserve task names
.PHONY: all clean fix

#Runs at the start
all: fix

#Runs fourth
fix: $(OUTPUTF).gb $(OUTPUTF).sym
	$(FIXER) $(FIXER_FLAGS) $(OUTPUTF).gb

#Runs third
$(OUTPUTF).gb: $(OBJ_FILES)
	$(LINKER) -p 255 -m "$(OUTPUT).map" -n "$(SYMBOL).sym" -o $(OUTPUTF).gb $(OBJ_FILES)

#Runs second?
$(BUILD_DIR)/obj/%.o : $(SRC_DIR)/%.asm | $(OBJ_DIRS)
	$(ASSEMBLER) $(ASSEMBLER_FLAGS) -o $@ $<

#Runs first
$(OBJ_DIRS):
	mkdir -p $@

#It's a cleanup function
clean:
	rm -rf $(BUILD_DIR)

print-%  : ; @echo $* = $($*)