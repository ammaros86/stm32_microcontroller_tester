################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/interfaces/I2cHandler.cpp \
../src/interfaces/SpiHandler.cpp \
../src/interfaces/UartHandler.cpp \
../src/interfaces/UsbPortsHandler.cpp 

CPP_DEPS += \
./src/interfaces/I2cHandler.d \
./src/interfaces/SpiHandler.d \
./src/interfaces/UartHandler.d \
./src/interfaces/UsbPortsHandler.d 

OBJS += \
./src/interfaces/I2cHandler.o \
./src/interfaces/SpiHandler.o \
./src/interfaces/UartHandler.o \
./src/interfaces/UsbPortsHandler.o 


# Each subdirectory must supply rules for building sources it contributes
src/interfaces/%.o: ../src/interfaces/%.cpp src/interfaces/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-interfaces

clean-src-2f-interfaces:
	-$(RM) ./src/interfaces/I2cHandler.d ./src/interfaces/I2cHandler.o ./src/interfaces/SpiHandler.d ./src/interfaces/SpiHandler.o ./src/interfaces/UartHandler.d ./src/interfaces/UartHandler.o ./src/interfaces/UsbPortsHandler.d ./src/interfaces/UsbPortsHandler.o

.PHONY: clean-src-2f-interfaces

