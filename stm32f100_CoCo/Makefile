PLATFORM				= stm32

OPTIMIZATION            = 0
CURRENT_DATE			= $(shell date +"%D %T")

#----------------------------------------------------------
CROSS                      = arm-none-eabi-
CROSS_COCO				   = csarm-none-eabi-

GCC                        = $(CROSS_COCO)gcc
AS                         = $(CROSS)as
SIZE                       = $(CROSS)size
OBJCOPY                    = $(CROSS)objcopy
OBJDUMP                    = $(CROSS)objdump
NM                         = $(CROSS)nm

#----------------------------------------------------------
MCU							= STM32F100RB

LIB_FOLDER					= STM32F10x/Include

$(info MCU: $(MCU))

TARGET_NAME                 = template_coco_$(MCU)
#----------------------------------------------------------
BUILD_DIR                   = build
OUTPUT_DIR                  = output
CORE_DIR					= core
BOARD_DIR					= board
LDS_SCRIPT                  = $(CORE_DIR)/script.ld.S
#----------------------------------------------------------
INCLUDE_FOLDERS 			= ./ ../ $(CORE_DIR) $(CORE_DIR)/lib $(BOARD_DIR)
INCLUDE_FOLDERS 			+= $(CORE_DIR)/$(PLATFORM) $(BOARD_DIR)/$(PLATFORM)
INCLUDE_FOLDERS 			+= $(CORE_DIR)/$(PLATFORM)/$(LIB_FOLDER) 
INCLUDE_FOLDERS				+= $(CORE_DIR)/lib $(CORE_DIR)/userspace

INCLUDES                    = $(INCLUDE_FOLDERS:%=-I%)
VPATH                      += $(INCLUDE_FOLDERS)
#----------------------------------------------------------
#core-dependent part
SRC_AS                      = startup_cortexm.S

SRC_C						= ucore.c ucore_lib.c lib_std.c pool.c stdlib.c

SRC_C						+= system.c
SRC_C						+= $(PLATFORM)_power.c $(PLATFORM)_uart.c $(PLATFORM)_pin.c

SRC_C						+= board.c crc.c
SRC_C                       += main.c
SRC_C 						+= dbg.c printf.c 

OBJ                         = $(SRC_AS:%.S=%.o) $(SRC_C:%.c=%.o)
#----------------------------------------------------------
DEFINES                     = -D$(MCU) -D__CORTEX -D'__MCU="$(MCU)"' -D'__BUILD_TIME="$(CURRENT_DATE)"'
MCU_FLAGS                   = -mcpu=cortex-m3 -mthumb -D__CORTEX_M3 -D__thumb2__=1 -mtune=cortex-m3 -msoft-float -mapcs-frame -std=gnu99
MCU_FLAGS					+= --cs-libgen=-mcpu=cortex-m3 --cs-libgen=-mthumb --cs-libgen=-mtune=cortex-m3 --cs-libgen=-msoft-float --cs-libgen=-mapcs-frame
NO_DEFAULTS                 = -fdata-sections -ffunction-sections -fno-hosted -fno-builtin  -nostdlib -nodefaultlibs
FLAGS_CC                    = $(INCLUDES) $(DEFINES) -I. -O$(OPTIMIZATION) -c -fmessage-length=0 $(MCU_FLAGS) $(NO_DEFAULTS)
FLAGS_LD                    = -Xlinker --gc-sections $(MCU_FLAGS)
#----------------------------------------------------------
all: $(TARGET_NAME).elf

%.elf: $(OBJ) $(LDS_SCRIPT)
	@$(GCC) $(INCLUDES) -I. $(DEFINES) -DLDS -E $(LDS_SCRIPT) -o $(BUILD_DIR)/script.ld.hash
	@awk '!/^(\ )*#/ {print $0}' $(BUILD_DIR)/script.ld.hash > $(BUILD_DIR)/script.ld
	@echo LD: $(OBJ)
	@$(GCC) $(FLAGS_LD) -T $(BUILD_DIR)/script.ld -o $(BUILD_DIR)/$@ $(OBJ:%.o=$(BUILD_DIR)/%.o)
	@echo '-----------------------------------------------------------'
	@$(SIZE) $(BUILD_DIR)/$(TARGET_NAME).elf
	@$(OBJCOPY) -O binary $(BUILD_DIR)/$(TARGET_NAME).elf $(BUILD_DIR)/$(TARGET_NAME).bin
	@$(OBJDUMP) -h -S -z $(BUILD_DIR)/$(TARGET_NAME).elf > $(BUILD_DIR)/$(TARGET_NAME).lss
	@$(NM) -n $(BUILD_DIR)/$(TARGET_NAME).elf > $(BUILD_DIR)/$(TARGET_NAME).sym
	@mkdir -p $(OUTPUT_DIR)
	@mv $(BUILD_DIR)/$(TARGET_NAME).bin $(OUTPUT_DIR)/$(TARGET_NAME).bin
ifeq ($(PLATFORM), lpc)
	python checksum_calc.py $(OUTPUT_DIR)/$(TARGET_NAME).bin
endif

.c.o:
	@-mkdir -p $(BUILD_DIR)
	@echo CC: $<
	@$(GCC) $(FLAGS_CC) -c ./$< -o $(BUILD_DIR)/$@

.S.o:
	@-mkdir -p $(BUILD_DIR)
	@echo AS_C: $<
	@$(GCC) $(INCLUDES) -I. $(DEFINES) -c -x assembler-with-cpp ./$< -o $(BUILD_DIR)/$@

program:
ifeq ($(PLATFORM), kinetis)
	JLink -device MK22FN256xxx12 -CommanderScript cmd_$(PLATFORM).jlink
else 
ifeq ($(PLATFORM), stm32)
	ST-LINK_CLI -c SWD -P $(OUTPUT_DIR)/$(TARGET_NAME).bin 0x08000000 -Rst
else 
	JLink -device LPC1837 -CommanderScript cmd_$(PLATFORM).jlink
endif
endif

clean:
	@echo '-----------------------------------------------------------'
	@rm -f build/*.*

test:
	@echo $(VPATH)

.PHONY : all clean program flash
