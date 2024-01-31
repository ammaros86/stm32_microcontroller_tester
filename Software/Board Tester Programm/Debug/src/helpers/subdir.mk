################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/helpers/Helper.cpp \
../src/helpers/Initializer.cpp \
../src/helpers/Logger.cpp 

CPP_DEPS += \
./src/helpers/Helper.d \
./src/helpers/Initializer.d \
./src/helpers/Logger.d 

OBJS += \
./src/helpers/Helper.o \
./src/helpers/Initializer.o \
./src/helpers/Logger.o 


# Each subdirectory must supply rules for building sources it contributes
src/helpers/%.o: ../src/helpers/%.cpp src/helpers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-helpers

clean-src-2f-helpers:
	-$(RM) ./src/helpers/Helper.d ./src/helpers/Helper.o ./src/helpers/Initializer.d ./src/helpers/Initializer.o ./src/helpers/Logger.d ./src/helpers/Logger.o

.PHONY: clean-src-2f-helpers

