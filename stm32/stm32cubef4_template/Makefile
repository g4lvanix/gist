TARGET=./build/main
FLASH=$(TARGET).hex
EXECUTABLE=$(TARGET).elf

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump

DRIVERDIR=../../../Drivers
LDSCRIPT=./STM32F401CE_FLASH.ld
STARTUP=$(DRIVERDIR)/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f401xe.s

DEFS = -DUSE_HAL_DRIVER -DSTM32F401xE -DHSE_VALUE=8000000

MCU = cortex-m4
MCFLAGS = -mcpu=$(MCU) -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork -std=c99
STM32_INCLUDES = -I$(DRIVERDIR)/CMSIS/Device/ST/STM32F4xx/Include/ \
	-I$(DRIVERDIR)/CMSIS/Include/ \
	-I$(DRIVERDIR)/STM32F4xx_HAL_Driver/Inc/ \
	-I./Inc

OPTIMIZE       = -Os

CFLAGS	= $(MCFLAGS)  $(OPTIMIZE)  $(DEFS) -I. -I./ $(STM32_INCLUDES)  -Wl,-T,$(LDSCRIPT)
AFLAGS	= $(MCFLAGS) 
#-mapcs-float use float regs. small increase in code size

SRC = Src/main.c \
	Src/stm32f4xx_hal_msp.c \
	Src/stm32f4xx_it.c \
	Src/system_stm32f4xx.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp_ex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac_ex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma2d.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash_ex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_irda.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nand.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nor.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pccard.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sai.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_smartcard.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fsmc.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c \
	$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
	#$(DRIVERDIR)/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_msp_template.c \
	 

OBJDIR = .
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) 
OBJ += Startup.o

all: $(FLASH)

$(FLASH): $(EXECUTABLE)
	$(CP) -O ihex $^ $@

$(EXECUTABLE): $(SRC) $(STARTUP)
	$(CC) $(CFLAGS) $^ -lm -lc -lnosys -o $@

clean:
	rm -f Startup.lst  $(FLASH) $(EXECUTABLE) $(TARGET).lst $(OBJ) $(AUTOGEN) $(TARGET).out $(TARGET).map \
	 $(TARGET).dmp
