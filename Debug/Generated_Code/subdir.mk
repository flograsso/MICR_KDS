################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/ADC_0.c \
../Generated_Code/Cpu.c \
../Generated_Code/GPIO_0.c \
../Generated_Code/LPSCI_0.c \
../Generated_Code/LPTMR_0.c \
../Generated_Code/UART_1.c \
../Generated_Code/clockMan1.c \
../Generated_Code/gpio1.c \
../Generated_Code/hardware_init.c \
../Generated_Code/i2cCom1.c \
../Generated_Code/osa1.c \
../Generated_Code/pin_init.c \
../Generated_Code/pwrMan1.c 

OBJS += \
./Generated_Code/ADC_0.o \
./Generated_Code/Cpu.o \
./Generated_Code/GPIO_0.o \
./Generated_Code/LPSCI_0.o \
./Generated_Code/LPTMR_0.o \
./Generated_Code/UART_1.o \
./Generated_Code/clockMan1.o \
./Generated_Code/gpio1.o \
./Generated_Code/hardware_init.o \
./Generated_Code/i2cCom1.o \
./Generated_Code/osa1.o \
./Generated_Code/pin_init.o \
./Generated_Code/pwrMan1.o 

C_DEPS += \
./Generated_Code/ADC_0.d \
./Generated_Code/Cpu.d \
./Generated_Code/GPIO_0.d \
./Generated_Code/LPSCI_0.d \
./Generated_Code/LPTMR_0.d \
./Generated_Code/UART_1.d \
./Generated_Code/clockMan1.d \
./Generated_Code/gpio1.d \
./Generated_Code/hardware_init.d \
./Generated_Code/i2cCom1.d \
./Generated_Code/osa1.d \
./Generated_Code/pin_init.d \
./Generated_Code/pwrMan1.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"FSL_OSA_BM_TIMER_CONFIG=2" -D"CPU_MKL25Z128VLK4" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/hal/inc" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/hal/src/sim/MKL25Z4" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/system/src/clock/MKL25Z4" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/system/inc" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/osa/inc" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/CMSIS/Include" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/devices" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/devices/MKL25Z4/include" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/utilities/src" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/utilities/inc" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/devices/MKL25Z4/startup" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/Generated_Code/SDK/platform/devices/MKL25Z4/startup" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/Sources" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/Sources/Components" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/Sources/Components/MMA8451Q_Files" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/Generated_Code" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/drivers/inc" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/system/src/power" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


