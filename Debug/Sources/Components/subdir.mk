################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Components/CONSOLE.c \
../Sources/Components/LM35.c \
../Sources/Components/MB7360.c \
../Sources/Components/MMA8451Q.c \
../Sources/Components/SIM800L.c \
../Sources/Components/UTILITIES.c 

OBJS += \
./Sources/Components/CONSOLE.o \
./Sources/Components/LM35.o \
./Sources/Components/MB7360.o \
./Sources/Components/MMA8451Q.o \
./Sources/Components/SIM800L.o \
./Sources/Components/UTILITIES.o 

C_DEPS += \
./Sources/Components/CONSOLE.d \
./Sources/Components/LM35.d \
./Sources/Components/MB7360.d \
./Sources/Components/MMA8451Q.d \
./Sources/Components/SIM800L.d \
./Sources/Components/UTILITIES.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/Components/%.o: ../Sources/Components/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"FSL_OSA_BM_TIMER_CONFIG=2" -D"CPU_MKL25Z128VLK4" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/hal/inc" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/hal/src/sim/MKL25Z4" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/system/src/clock/MKL25Z4" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/system/inc" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/osa/inc" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/CMSIS/Include" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/devices" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/devices/MKL25Z4/include" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/utilities/src" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/utilities/inc" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/devices/MKL25Z4/startup" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/Generated_Code/SDK/platform/devices/MKL25Z4/startup" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/Sources" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/Sources/Components" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/Sources/Components/MMA8451Q_Files" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/Generated_Code" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/drivers/inc" -I"C:/Users/SoporteSEM/workspace.kds/MICR_KDS/SDK/platform/system/src/power" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


