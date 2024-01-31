################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/test_processes/AnalogInputTestProcess.cpp \
../src/test_processes/ContinuityTestProcess.cpp \
../src/test_processes/DigitalInputTestProcess.cpp \
../src/test_processes/DigitalOutputTestProcess.cpp \
../src/test_processes/FlashProcess.cpp \
../src/test_processes/PowerGndPinsTestProcess.cpp \
../src/test_processes/ShortGpioTestProcess.cpp \
../src/test_processes/TemperatureProcess.cpp 

CPP_DEPS += \
./src/test_processes/AnalogInputTestProcess.d \
./src/test_processes/ContinuityTestProcess.d \
./src/test_processes/DigitalInputTestProcess.d \
./src/test_processes/DigitalOutputTestProcess.d \
./src/test_processes/FlashProcess.d \
./src/test_processes/PowerGndPinsTestProcess.d \
./src/test_processes/ShortGpioTestProcess.d \
./src/test_processes/TemperatureProcess.d 

OBJS += \
./src/test_processes/AnalogInputTestProcess.o \
./src/test_processes/ContinuityTestProcess.o \
./src/test_processes/DigitalInputTestProcess.o \
./src/test_processes/DigitalOutputTestProcess.o \
./src/test_processes/FlashProcess.o \
./src/test_processes/PowerGndPinsTestProcess.o \
./src/test_processes/ShortGpioTestProcess.o \
./src/test_processes/TemperatureProcess.o 


# Each subdirectory must supply rules for building sources it contributes
src/test_processes/%.o: ../src/test_processes/%.cpp src/test_processes/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-test_processes

clean-src-2f-test_processes:
	-$(RM) ./src/test_processes/AnalogInputTestProcess.d ./src/test_processes/AnalogInputTestProcess.o ./src/test_processes/ContinuityTestProcess.d ./src/test_processes/ContinuityTestProcess.o ./src/test_processes/DigitalInputTestProcess.d ./src/test_processes/DigitalInputTestProcess.o ./src/test_processes/DigitalOutputTestProcess.d ./src/test_processes/DigitalOutputTestProcess.o ./src/test_processes/FlashProcess.d ./src/test_processes/FlashProcess.o ./src/test_processes/PowerGndPinsTestProcess.d ./src/test_processes/PowerGndPinsTestProcess.o ./src/test_processes/ShortGpioTestProcess.d ./src/test_processes/ShortGpioTestProcess.o ./src/test_processes/TemperatureProcess.d ./src/test_processes/TemperatureProcess.o

.PHONY: clean-src-2f-test_processes

