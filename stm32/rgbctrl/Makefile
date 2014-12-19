TARGET=./build/main
FLASH=$(TARGET).hex
BINARY=$(TARGET).bin
EXECUTABLE=$(TARGET).elf

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump

BIN=$(CP) -O ihex 

DRIVERDIR=../../../stm32cubef0/Drivers
LDSCRIPT=./STM32F030K6_FLASH.ld
STARTUP = $(DRIVERDIR)/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f030x6.s

DEFS = -DUSE_HAL_DRIVER -DSTM32F030x6 -DHSE_VALUE=8000000

MCU = cortex-m0
MCFLAGS = -mcpu=$(MCU) -mthumb -mlittle-endian -mthumb-interwork -std=c99
STM32_INCLUDES = -I$(DRIVERDIR)/CMSIS/Device/ST/STM32F0xx/Include/ \
	-I$(DRIVERDIR)/CMSIS/Include/ \
	-I$(DRIVERDIR)/STM32F0xx_HAL_Driver/Inc/ \
	-I./Inc

OPTIMIZE       = -Os

CFLAGS	= $(MCFLAGS)  $(OPTIMIZE)  $(DEFS) -I. -I./ $(STM32_INCLUDES)  -Wl,-T,$(LDSCRIPT)
AFLAGS	= $(MCFLAGS) 

SRC = Src/main.c \
	Src/stm32f0xx_hal_msp.c \
	Src/stm32f0xx_it.c \
	Src/system_stm32f0xx.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_adc.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_adc_ex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_can.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cec.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_comp.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_crc.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_crc_ex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dac.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dac_ex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2s.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_irda.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_iwdg.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pcd.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pcd_ex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rtc.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rtc_ex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_smartcard.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_smartcard_ex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_smbus.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tsc.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart_ex.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_usart.c \
	$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_wwdg.c 
	#$(DRIVERDIR)/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_msp_template.c \
	 

OBJDIR = .
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) 
OBJ += Startup.o

all: $(FLASH) $(BINARY)

$(FLASH): $(EXECUTABLE)
	$(CP) -O ihex $^ $@

$(BINARY): $(EXECUTABLE)
	$(CP) -O binary $^ $@

$(EXECUTABLE): $(SRC) $(STARTUP)
	$(CC) $(CFLAGS) $^ -lm -lc -lnosys -o $@

clean:
	rm -f Startup.lst  $(FLASH) $(EXECUTABLE) $(TARGET).lst $(OBJ) $(AUTOGEN) $(TARGET).out $(TARGET).map \
	 $(TARGET).dmp
