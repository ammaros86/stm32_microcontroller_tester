################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/peripherals/AnalogDigitalConverter.cpp \
../src/peripherals/CurrentSensor.cpp \
../src/peripherals/DigitalAnalogConverter.cpp \
../src/peripherals/IoExpander.cpp \
../src/peripherals/LcdDisplay.cpp \
../src/peripherals/Multiplexers.cpp 

CPP_DEPS += \
./src/peripherals/AnalogDigitalConverter.d \
./src/peripherals/CurrentSensor.d \
./src/peripherals/DigitalAnalogConverter.d \
./src/peripherals/IoExpander.d \
./src/peripherals/LcdDisplay.d \
./src/peripherals/Multiplexers.d 

OBJS += \
./src/peripherals/AnalogDigitalConverter.o \
./src/peripherals/CurrentSensor.o \
./src/peripherals/DigitalAnalogConverter.o \
./src/peripherals/IoExpander.o \
./src/peripherals/LcdDisplay.o \
./src/peripherals/Multiplexers.o 


# Each subdirectory must supply rules for building sources it contributes
src/peripherals/%.o: ../src/peripherals/%.cpp src/peripherals/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-peripherals

clean-src-2f-peripherals:
	-$(RM) ./src/peripherals/AnalogDigitalConverter.d ./src/peripherals/AnalogDigitalConverter.o ./src/peripherals/CurrentSensor.d ./src/peripherals/CurrentSensor.o ./src/peripherals/DigitalAnalogConverter.d ./src/peripherals/DigitalAnalogConverter.o ./src/peripherals/IoExpander.d ./src/peripherals/IoExpander.o ./src/peripherals/LcdDisplay.d ./src/peripherals/LcdDisplay.o ./src/peripherals/Multiplexers.d ./src/peripherals/Multiplexers.o

.PHONY: clean-src-2f-peripherals

