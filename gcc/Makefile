INCLUDE_DIRS:= device/include cmsis/include ../include ../STM32F3xx_HAL_Driver/Inc ../STM32F3xx-Nucleo ../STM32_USB_Device_Library/Core/Inc ../STM32_USB_Device_Library/Class/HID/Inc
SOURCE_DIRS:=  device/source ../source ../STM32F3xx_HAL_Driver/Src ../STM32F3xx-Nucleo ../STM32_USB_Device_Library/Core/Src ../STM32_USB_Device_Library/Class/HID/Src

#All the HAL files
HAL_FILES:= $(notdir $(wildcard ../STM32F3xx_HAL_Driver/Src/*.c ))
#BOARD FILES
BOARD_FILES:= stm32f3xx_nucleo.c
#USB FILES
USB_FILES:= usbd_hid.c usbd_core.c usbd_ctlreq.c usbd_ioreq.c
#LOCAL SOURCE
LOCAL_SOURCE:= main.c stm32f3xx_it.c usbd_conf.c usbd_desc.c
#the C files to compile
CFILES:=system_stm32f3xx.c $(HAL_FILES) $(LOCAL_SOURCE) $(USB_FILES) $(BOARD_FILES)
#the assembly files assemble
ASM_FILES:=startup_stm32f302x8.s 
#the target microcontroller
TCHIP=STM32F302x8
#The compiler to use
CC=arm-none-eabi-gcc
#LINKER OPTIONS
#The linker script for the microcontroller 
LD_SCRIPTS:=STM32F302X8_FLASH.ld 
#the directories for linker files 
LIB_DIRS:= device/
#the libraries to link 
LIB_NAMES:=
#linker flags 
LFLAGS:= $(patsubst %,-L%,$(LIB_DIRS)) $(patsubst %,-l%,$(LIB_NAMES)) -T$(LD_SCRIPTS) 
#PREPROCESSOR OPTIONS
#Define macro for all files
DEFINES:= USE_HAL_DRIVER
DEFINE_FLAGS:=$(patsubst %,-D%,$(DEFINES))
INCLUDE_FLAGS:=$(patsubst %,-I%,$(INCLUDE_DIRS)) 
CFLAGS= -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=c99 -Wall -g -O1 -ffunction-sections -fdata-sections
CPPFLAGS= $(INCLUDE_FLAGS) $(DEFINE_FLAGS) -D$(TCHIP) 
vpath %.h $(INCLUDE_DIRS)
vpath %.c $(SOURCE_DIRS)
vpath %.s device/source
vpath %.o objects

COBJECTS:= $(patsubst %.c,%.o,$(CFILES)) 
ASMOBJECTS:= $(patsubst %.s,%.o,$(ASM_FILES))

all: salida.elf 
objects:
	mkdir objects
salida.elf: objects $(COBJECTS) $(ASMOBJECTS)
	$(CC) -Wl,--gc-sections -specs=nano.specs $(CFLAGS) objects/*.o $(LFLAGS) -o $@
	arm-none-eabi-size $@ ;
$(COBJECTS): %.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o objects/$@
$(ASMOBJECTS): %.o: %.s
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o objects/$@
.PHONY:clean rebuild burn
clean:
	rm -f  *.elf ;
	rm -fr objects ;
rebuild: clean all
burn: salida.elf
	openocd -f board/st_nucleo_f3.cfg -c"init" -c"sleep 300"  -c"program erase salida.elf" -c"sleep 300" -c"reset" -c"shutdown"
debug: salida.elf
	openocd -f board/st_nucleo_f3.cfg &>/dev/null & 
	sleep 1 ;
	! echo "reset" | telnet localhost 4444 ;
	! echo "reset halt" | telnet localhost 4444 ;
	sleep 1 ;
	arm-none-eabi-gdb --tui --eval-command="target remote localhost:3333" salida.elf ;	
	! echo "reset" | telnet localhost 4444 ;
	sleep 1 ;
	! echo "shutdown" | telnet localhost 4444 ;
