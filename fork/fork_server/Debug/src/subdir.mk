################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Game.c \
../src/Sockets.c \
../src/hangserver_iter.c 

OBJS += \
./src/Game.o \
./src/Sockets.o \
./src/hangserver_iter.o 

C_DEPS += \
./src/Game.d \
./src/Sockets.d \
./src/hangserver_iter.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


