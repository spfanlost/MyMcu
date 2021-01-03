
#
# expected macroes in this file
#
#	TARGET			; make_lib or make_image (used in this file)
#
#	EXTRA_C_DEFINES	; e.g. -DSYS_SATA -DPROG_ROOT_PATH=\"$(ROOT_PATH)\" -DUSER_NAME=\"$(USERNAME)\"
#

.SUFFIXES : .s.o .c.o .s.d .c.d .c .s .mk

OBJ_DIR = obj

C_OBJS  = $(addprefix $(OBJ_DIR)/,$(C_SRC:.c=.c.o))
ASM_OBJS= $(addprefix $(OBJ_DIR)/,$(ASM_SRC:.s=.s.o))
OBJS    = $(C_OBJS) $(ASM_OBJS)

C_DEPS  = $(addprefix $(OBJ_DIR)/,$(C_SRC:.c=.c.d))
ASM_DEPS= $(addprefix $(OBJ_DIR)/,$(ASM_SRC:.s=.s.d))
DEPS    = $(C_DEPS) $(ASM_DEPS)

########################
### Compiler Parameters
########################

#--------------------------------------------------
# Include directories
#--------------------------------------------------
C_INCLUDE_DIRS   = $(foreach dir, . $(INCLUDE_MODULES),-I$(ROOT_PATH)/$(dir))
ASM_INCLUDE_DIRS = $(foreach dir, . $(INCLUDE_MODULES),-I$(ROOT_PATH)/$(dir))

#--------------------------------------------------
# compiler flags
#--------------------------------------------------

ifeq ($(MAKE_DEBUG), 1)
C_FLAGS = --diag_error=63,47,223,14,940,186,3017 --bss_threshold=0 --littleend --cpu=Cortex-R5 --apcs=interwork --signed_chars --no_multifile --c99
else
C_FLAGS = --diag_error=63,47,223,14,940,186 --diag_suppress=3017 --bss_threshold=0 --littleend --cpu=Cortex-R5 --apcs=interwork --signed_chars --no_multifile --c99
endif

ifeq ($(MAKE_ONE_BIN), 1)
C_DEFINES += -DONE_BIN
endif

#--------------------------------------------------
# MAKE_DEBUG = 1: optimization off
#              0: optimization on
#--------------------------------------------------
ifeq ($(MAKE_DEBUG), 1)
C_FLAGS += -g -O0
else
#for Corxtex-R5, must use "-g", or has problem
C_FLAGS += -Ospace -O2 -g
endif



#--------------------------------------------------
# MAKE_TARGET = 0: make firmware for ASIC
#               1: make firmware for FPGA
#               2: make firmware for COSIM
#--------------------------------------------------
ifeq ($(MAKE_TARGET), 0)
endif

ifeq ($(MAKE_TARGET), 1)
C_DEFINES += -DFPGA_MODE
endif

ifeq ($(MAKE_TARGET), 2)
C_DEFINES += -DCO_SIM_MODE
endif


#--------------------------------------------------
# Enabled for making loader
#--------------------------------------------------
ifeq ($(MAKE_LOADER_ONLY), 1)
C_DEFINES += -DMAKE_LOADER_ONLY
endif


#--------------------------------------------------
# Enabled for making JMF608 f/w
#--------------------------------------------------
ifeq ($(MAKE_BUILD_JMF608), 1)
C_DEFINES += -DMAKE_JMF608
endif



#
# Place user specific compile options after this line
#


#--------------------------------------------------
# Enabled for RTL simulation only (default is disabled)
#--------------------------------------------------
ifeq ($(MAKE_HW_SIM), 1)
C_DEFINES += -DRTL_SIM
endif


#--------------------------------------------------
# Reserved for new compile option.
#   1. Duplicate this section
#   2. Add compile option after symbol '+=' for 'C_DEFINES' or 'C_FLAGS'
#   3. Uncomment new option(s).
#   4. Modify comment for new option(s) especially
#      its(their) default state be enabled or disabled
#  *5. Make sure new compile option state be correct (eabled or disable)
#      before checking-in your code.
#--------------------------------------------------
#C_DEFINES +=
#C_FLAGS +=


########################
### Assembler Parameters
########################

#ASM_FLAGS = --apcs interwork -g --cpu=Cortex-R5
ASM_FLAGS = --apcs=interwork -g --cpu=Cortex-R5

########################
### Linker Parameters
########################

LINK_FLAGS = --map --info totals --debug --userlibpath $(PUBLIC_LIB_PATH) --list_mapping_symbols --info libraries --strict --no_remove

########################
### LINKER_LIBS
########################

LINKER_LIBS = $(PUBLIC_LIB_PATH)/link.d


########################
### Common Rules 1
########################

ifeq ($(TARGET),)

all dep clean_dep :
ifneq ($(SUB_DIRS),)
	@for x in $(SUB_DIRS); do cd $$x; make $@; cd $(CURDIR); done
endif

else

dep : $(OBJ_DIR)/depend.mk

clean_dep:
	@$(DEL) $(DEPS) $(OBJ_DIR)/*.mk

endif


clean:
	@rm -rf $(OBJ_DIR)
ifneq ($(SUB_DIRS),)
	@for x in $(SUB_DIRS); do cd $$x; make $@; cd $(CURDIR); done
endif

########################
### Common Rules 2
########################

$(OBJ_DIR)/depend.mk : | $(OBJ_DIR) $(C_DEPS) $(ASM_DEPS)

$(C_DEPS) : $(OBJ_DIR)/c_deps.mk

$(ASM_DEPS) : $(OBJ_DIR)/asm_deps.mk

$(OBJ_DIR)/c_deps.mk ::
	@$(CC) $(C_INCLUDE_DIRS) $(C_DEFINES) $(C_FLAGS) $(C_MODULE_FLAG) $(EXTRA_C_DEFINES) -c -M --depend_format=unix_escaped --no_code_gen $(notdir $(C_OBJS:.c.o=.c)) > $@
	@sed -i -e "s/\(.*\)\.o/$(OBJ_DIR)\/\1.c.o/" $@
	@sed -i -e "/c_deps/d" $(OBJ_DIR)/depend.mk
	@echo include $@ >> $(OBJ_DIR)/depend.mk

$(OBJ_DIR)/asm_deps.mk ::
	@- > $(OBJ_DIR)/asm_deps.mk
	@for x in $(ASM_SRC); do $(ASM) -I./ $(ASM_FLAGS) -m --depend_format=unix_escaped --no_code_gen $$x >> $(OBJ_DIR)/asm_deps.mk; done
	@sed -i -e "s/\(.*\)\.o/$(OBJ_DIR)\/\1.s.o/" $@
	@sed -i -e "/asm_deps/d" $(OBJ_DIR)/depend.mk
	@echo include $(OBJ_DIR)/asm_deps.mk >> $(OBJ_DIR)/depend.mk

########################
### Common Rules 3
########################

$(OBJ_DIR)/%.c.o : %.c 
	@$(CC) $(C_INCLUDE_DIRS) $(C_DEFINES) $(C_FLAGS) $(C_MODULE_FLAG) $(EXTRA_C_DEFINES) -c $< -o $@


$(OBJ_DIR)/%.s.o : %.s
	@$(ASM) $(ASM_INCLUDE_DIRS) $(ASM_FLAGS) $< -o $@

########################
### Common Rules 4
########################

$(OBJS) : | $(OBJ_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@touch $(OBJ_DIR)/depend.mk

########################
### Include Extra Rules
########################

ifeq ($(TARGET), make_lib)
include $(ROOT_PATH)/make/rules_lib.mk
endif

ifeq ($(TARGET), make_image)
include $(ROOT_PATH)/make/rules_image.mk
endif



