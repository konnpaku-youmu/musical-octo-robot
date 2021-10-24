################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/hw_accelerator.c \
../src/main.c \
../src/montgomery.c \
../src/mp_arith.c \
../src/testvector.c \
../src/warmup.c 

S_UPPER_SRCS += \
../src/asm_func.S 

OBJS += \
./src/asm_func.o \
./src/hw_accelerator.o \
./src/main.o \
./src/montgomery.o \
./src/mp_arith.o \
./src/testvector.o \
./src/warmup.o 

S_UPPER_DEPS += \
./src/asm_func.d 

C_DEPS += \
./src/hw_accelerator.d \
./src/main.d \
./src/montgomery.d \
./src/mp_arith.d \
./src/testvector.d \
./src/warmup.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O3 -funroll-loops -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../hw_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O3 -funroll-loops -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../hw_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

