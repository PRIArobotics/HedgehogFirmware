# name of the project, the binaries will be generated with this name
PROJ_NAME = HedgehogFirmware

# remote flashing target
REMOTE = pi@hedgehog5
REMOTE_BUNDLE = /home/pi/HedgehogBundle/firmware

#compiler
export CC = arm-none-eabi-gcc
#linker
export LD = arm-none-eabi-gcc
#objcopy
export OC = arm-none-eabi-objcopy
#size
export SZ = arm-none-eabi-size

#linker flags
export LDFLAGS = -mcpu=cortex-m4 -DSTM32F401xC -mlittle-endian -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -static -Wl,--gc-sections
#compiler flags
export CFLAGS = -mcpu=cortex-m4 -DSTM32F401xC -mlittle-endian -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -fno-common -ffunction-sections -fdata-sections -Wall -Og -g -c

###################################################

#root directory containing the makefile and user source files
ROOTDIR := $(CURDIR)

#directory containing system files
SYSTEMDIR := system

#directory containing user sources
SRCDIR := src

#directory containing HCP souce files
HCPDIR := src/hcp

#directory for binary output files
BUILDDIR := build

#directory for object files
OBJDIR := build/obj


#include directories
CFLAGS += -I$(SYSTEMDIR) -I$(SRCDIR) -I$(HCPDIR)

#linker file
LDFLAGS += -T$(SYSTEMDIR)/STM32F401XB_FLASH.ld

#libraries we want to link against
#LDFLAGS += -lc -lnosys #newlib (e.g. for printf)
#LDFLAGS += -lm #math

#system source files
SRC = startup_stm32f401xc.s system.c

#user source files
SRC += main.c gpio.c output.c digitalIO.c servo.c adc.c motor.c uart.c ringbuffer.c systick.c battery.c

#user hcp source files
SRC += hcp.c hcp_commands.c hcp_handler_analogReq.c hcp_handler_digitalReq.c hcp_handler_ioState.c hcp_handler_motor.c hcp_handler_servo.c hcp_handler_serial.c


#object files (with object directory --> $(OBJDIR)/name.o)
OBJS = $(addprefix $(OBJDIR)/,$(subst .c,.o,$(subst .s,.o,$(SRC))))

###################################################

.PHONY: all buildAll flash remote-flash flash-stlink size clean

#build and show size
all: buildAll size

#build all output formats
buildAll: $(BUILDDIR)/$(PROJ_NAME).elf $(BUILDDIR)/$(PROJ_NAME).hex $(BUILDDIR)/$(PROJ_NAME).bin
	@echo build finished

#flash locally via HedgehogFirmwareBundle
flash:
	sudo openocd -f openocd/openocd_hedgehog_swd.cfg -c "program $(BUILDDIR)/$(PROJ_NAME).bin 0x8000000 verify; reset run; exit"

#flash remotely via HedgehogFirmwareBundle
remote-flash:
	rsync -avz $(BUILDDIR)/$(PROJ_NAME).bin $(REMOTE):/tmp/
	ssh $(REMOTE) "cd $(REMOTE_BUNDLE) && make flash-tmp"

flash-tmp:
	sudo openocd -f openocd/openocd_hedgehog_swd.cfg -c "program /tmp/$(PROJ_NAME).bin 0x8000000 verify; reset run; exit"


#shows size of .elf
size: $(BUILDDIR)/$(PROJ_NAME).elf
	$(SZ) $<


#create .hex from .elf
$(BUILDDIR)/$(PROJ_NAME).hex: $(BUILDDIR)/$(PROJ_NAME).elf | $(BUILDDIR)
	@echo creating $@ from $<
	@$(OC) -Oihex $< $@

#create .bin from .elf
$(BUILDDIR)/$(PROJ_NAME).bin: $(BUILDDIR)/$(PROJ_NAME).elf | $(BUILDDIR)
	@echo creating $@ from $<
	@$(OC) -Obinary $< $@

#link objects to .elf
$(BUILDDIR)/$(PROJ_NAME).elf: $(OBJS) | $(OBJDIR) $(BUILDDIR)
	@echo linking $@ from $(OBJS)
	@$(LD) $(LDFLAGS) -o $@ $(OBJS)


#compile user .c file to .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@echo compiling $@ from $<
	@$(CC) $(CFLAGS) -o $@ $<

#compile user hcp .c file to .o
$(OBJDIR)/%.o: $(HCPDIR)/%.c | $(OBJDIR)
	@echo compiling $@ from $<
	@$(CC) $(CFLAGS) -o $@ $<

#compile system .c file to .o
$(OBJDIR)/%.o: $(SYSTEMDIR)/%.c | $(OBJDIR)
	@echo compiling $@ from $<
	@$(CC) $(CFLAGS) -o $@ $<

#compile system .s file to .o
$(OBJDIR)/%.o: $(SYSTEMDIR)/%.s | $(OBJDIR)
	@echo compiling $@ from $<
	@$(CC) $(CFLAGS) -o $@ $<


#create directory for objects
$(OBJDIR):
	mkdir -p $@

#create directory for output files
$(BUILDDIR):
	mkdir -p $@


#delete all build products
clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BUILDDIR)/$(PROJ_NAME).*
	@echo
